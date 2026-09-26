#define _POSIX_C_SOURCE 200809L

#include "restaurante.h"

#include <stdio.h>
#include <time.h>

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

static Pedido criar_pedido(Cliente cliente, int id_garcom)
{
    Pedido pedido;

    pedido.id = cliente.id;
    pedido.cliente_id = cliente.id;
    pedido.mesa = cliente.mesa;
    pedido.tipo_prato = (cliente.id % 4) + 1;
    pedido.garcom_id = id_garcom;
    pedido.cozinheiro_id = -1;
    pedido.status = PEDIDO_CRIADO;

    return pedido;
}

void *thread_garcom(void *arg)
{
    ArgsGarcom *args = (ArgsGarcom *)arg;
    Cliente cliente;

    while (fila_clientes_aguardar_remover(args->fila_clientes, &cliente)) {
        Pedido pedido;

        printf("Garcom %d esta atendendo cliente %d na mesa %d.\n",
               args->id_garcom,
               cliente.id,
               cliente.mesa);

        dormir_ms(150 + (args->id_garcom * 50));

        pedido = criar_pedido(cliente, args->id_garcom);

        if (fila_pedidos_inserir(args->fila_pedidos, pedido) != 0) {
            fprintf(stderr,
                    "Garcom %d nao conseguiu inserir pedido do cliente %d.\n",
                    args->id_garcom,
                    cliente.id);
            break;
        }

        printf("Garcom %d enviou pedido %d do cliente %d para a cozinha.\n",
               args->id_garcom,
               pedido.id,
               pedido.cliente_id);
    }

    printf("Garcom %d encerrou atendimento.\n", args->id_garcom);
    return NULL;
}
