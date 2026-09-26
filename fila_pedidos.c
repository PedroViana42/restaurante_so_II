#include "restaurante.h"

#include <stdlib.h>

int fila_pedidos_inicializar(FilaPedidos *fila)
{
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
    fila->atendimento_finalizado = 0;

    if (pthread_mutex_init(&fila->mutex, NULL) != 0) {
        return -1;
    }

    if (pthread_cond_init(&fila->cond_pedido_disponivel, NULL) != 0) {
        pthread_mutex_destroy(&fila->mutex);
        return -1;
    }

    return 0;
}

void fila_pedidos_destruir(FilaPedidos *fila)
{
    NoPedido *atual = fila->inicio;

    while (atual != NULL) {
        NoPedido *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    pthread_cond_destroy(&fila->cond_pedido_disponivel);
    pthread_mutex_destroy(&fila->mutex);
}

int fila_pedidos_inserir(FilaPedidos *fila, Pedido pedido)
{
    NoPedido *novo = malloc(sizeof(NoPedido));

    if (novo == NULL) {
        return -1;
    }

    novo->pedido = pedido;
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
    pthread_cond_signal(&fila->cond_pedido_disponivel);

    pthread_mutex_unlock(&fila->mutex);

    return 0;
}

int fila_pedidos_remover(FilaPedidos *fila, Pedido *pedido)
{
    NoPedido *removido;

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
    *pedido = removido->pedido;

    pthread_mutex_unlock(&fila->mutex);

    free(removido);
    return 1;
}

void fila_pedidos_marcar_atendimento_finalizado(FilaPedidos *fila)
{
    pthread_mutex_lock(&fila->mutex);
    fila->atendimento_finalizado = 1;
    pthread_cond_broadcast(&fila->cond_pedido_disponivel);
    pthread_mutex_unlock(&fila->mutex);
}
