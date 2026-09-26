#define _POSIX_C_SOURCE 200809L

#include "restaurante.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static long tempo_atual_ms(void)
{
    struct timespec ts;

    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_sec * 1000L) + (ts.tv_nsec / 1000000L);
}

static void dormir_ms(int ms)
{
    struct timespec tempo;

    if (ms <= 0) {
        return;
    }

    tempo.tv_sec = ms / 1000;
    tempo.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&tempo, NULL);
}

static int inteiro_aleatorio(int minimo, int maximo)
{
    if (maximo <= minimo) {
        return minimo;
    }

    return minimo + (rand() % (maximo - minimo + 1));
}

static Cliente criar_cliente(int id, int total_mesas)
{
    Cliente cliente;

    cliente.id = id;
    cliente.mesa = inteiro_aleatorio(1, total_mesas);
    cliente.qtd_pessoas = inteiro_aleatorio(1, 4);
    cliente.pedido_id = -1;
    cliente.tempo_chegada_ms = tempo_atual_ms();
    cliente.status = CLIENTE_ESPERANDO;

    return cliente;
}

void *thread_geradora_clientes(void *arg)
{
    ArgsGeradorClientes *args = (ArgsGeradorClientes *)arg;

    srand((unsigned int)time(NULL));

    for (int i = 1; i <= args->total_clientes; i++) {
        Cliente cliente = criar_cliente(i, args->total_mesas);

        if (fila_clientes_inserir(args->fila_clientes, cliente) != 0) {
            fprintf(stderr, "Erro ao inserir cliente %d na fila.\n", cliente.id);
            break;
        }

        printf("Cliente %d chegou: mesa %d, grupo de %d pessoa(s).\n",
               cliente.id,
               cliente.mesa,
               cliente.qtd_pessoas);

        dormir_ms(inteiro_aleatorio(args->intervalo_min_ms,
                                    args->intervalo_max_ms));
    }

    fila_clientes_marcar_geracao_finalizada(args->fila_clientes);
    printf("Geracao de clientes finalizada.\n");

    return NULL;
}
