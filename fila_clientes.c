#include "restaurante.h"

#include <stdlib.h>

int fila_clientes_inicializar(FilaClientes *fila)
{
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->geracao_finalizada = 0;

    if (pthread_mutex_init(&fila->mutex, NULL) != 0) {
        return -1;
    }

    if (pthread_cond_init(&fila->cond_cliente_disponivel, NULL) != 0) {
        pthread_mutex_destroy(&fila->mutex);
        return -1;
    }

    return 0;
}

void fila_clientes_destruir(FilaClientes *fila)
{
    NoCliente *atual = fila->inicio;

    while (atual != NULL) {
        NoCliente *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    pthread_cond_destroy(&fila->cond_cliente_disponivel);
    pthread_mutex_destroy(&fila->mutex);
}

int fila_clientes_inserir(FilaClientes *fila, Cliente cliente)
{
    NoCliente *novo = malloc(sizeof(NoCliente));

    if (novo == NULL) {
        return -1;
    }

    novo->cliente = cliente;
    novo->prox = NULL;

    pthread_mutex_lock(&fila->mutex);

    if (fila->fim == NULL) {
        fila->inicio = novo;
        fila->fim = novo;
    } else {
        fila->fim->prox = novo;
        fila->fim = novo;
    }

    fila->tamanho++;
    pthread_cond_signal(&fila->cond_cliente_disponivel);

    pthread_mutex_unlock(&fila->mutex);

    return 0;
}

int fila_clientes_remover(FilaClientes *fila, Cliente *cliente)
{
    NoCliente *removido;

    pthread_mutex_lock(&fila->mutex);

    if (fila->inicio == NULL) {
        pthread_mutex_unlock(&fila->mutex);
        return 0;
    }

    removido = fila->inicio;
    fila->inicio = removido->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    fila->tamanho--;
    *cliente = removido->cliente;

    pthread_mutex_unlock(&fila->mutex);

    free(removido);
    return 1;
}

int fila_clientes_aguardar_remover(FilaClientes *fila, Cliente *cliente)
{
    NoCliente *removido;

    pthread_mutex_lock(&fila->mutex);

    while (fila->inicio == NULL && !fila->geracao_finalizada) {
        pthread_cond_wait(&fila->cond_cliente_disponivel, &fila->mutex);
    }

    if (fila->inicio == NULL && fila->geracao_finalizada) {
        pthread_mutex_unlock(&fila->mutex);
        return 0;
    }

    removido = fila->inicio;
    fila->inicio = removido->prox;

    if (fila->inicio == NULL) {
        fila->fim = NULL;
    }

    fila->tamanho--;
    *cliente = removido->cliente;

    pthread_mutex_unlock(&fila->mutex);

    free(removido);
    return 1;
}

void fila_clientes_marcar_geracao_finalizada(FilaClientes *fila)
{
    pthread_mutex_lock(&fila->mutex);
    fila->geracao_finalizada = 1;
    pthread_cond_broadcast(&fila->cond_cliente_disponivel);
    pthread_mutex_unlock(&fila->mutex);
}
