#include "restaurante.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    enum { TOTAL_GARCONS = 3 };

    FilaClientes fila_clientes;
    FilaPedidos fila_pedidos;
    ArgsGeradorClientes args_gerador;
    ArgsGarcom args_garcons[TOTAL_GARCONS];
    pthread_t gerador;
    pthread_t garcons[TOTAL_GARCONS];
    int garcons_criados = 0;
    Pedido pedido;

    if (fila_clientes_inicializar(&fila_clientes) != 0) {
        fprintf(stderr, "Erro ao inicializar fila de clientes.\n");
        return EXIT_FAILURE;
    }

    if (fila_pedidos_inicializar(&fila_pedidos) != 0) {
        fprintf(stderr, "Erro ao inicializar fila de pedidos.\n");
        fila_clientes_destruir(&fila_clientes);
        return EXIT_FAILURE;
    }

    args_gerador.fila_clientes = &fila_clientes;
    args_gerador.total_clientes = 10;
    args_gerador.intervalo_min_ms = 200;
    args_gerador.intervalo_max_ms = 800;
    args_gerador.total_mesas = 6;

    for (int i = 0; i < TOTAL_GARCONS; i++) {
        args_garcons[i].id_garcom = i + 1;
        args_garcons[i].fila_clientes = &fila_clientes;
        args_garcons[i].fila_pedidos = &fila_pedidos;

        if (pthread_create(&garcons[i], NULL, thread_garcom, &args_garcons[i]) != 0) {
            fprintf(stderr, "Erro ao criar thread do garcom %d.\n", i + 1);
            fila_clientes_marcar_geracao_finalizada(&fila_clientes);
            for (int j = 0; j < garcons_criados; j++) {
                pthread_join(garcons[j], NULL);
            }
            fila_pedidos_destruir(&fila_pedidos);
            fila_clientes_destruir(&fila_clientes);
            return EXIT_FAILURE;
        }

        garcons_criados++;
    }

    if (pthread_create(&gerador, NULL, thread_geradora_clientes, &args_gerador) != 0) {
        fprintf(stderr, "Erro ao criar thread geradora de clientes.\n");
        fila_clientes_marcar_geracao_finalizada(&fila_clientes);
        for (int i = 0; i < garcons_criados; i++) {
            pthread_join(garcons[i], NULL);
        }
        fila_pedidos_destruir(&fila_pedidos);
        fila_clientes_destruir(&fila_clientes);
        return EXIT_FAILURE;
    }

    pthread_join(gerador, NULL);

    for (int i = 0; i < TOTAL_GARCONS; i++) {
        pthread_join(garcons[i], NULL);
    }

    fila_pedidos_marcar_atendimento_finalizado(&fila_pedidos);

    printf("\nPedidos que ficaram aguardando a cozinha:\n");
    while (fila_pedidos_remover(&fila_pedidos, &pedido)) {
        printf("- Pedido %d do cliente %d, mesa %d, criado pelo garcom %d\n",
               pedido.id,
               pedido.cliente_id,
               pedido.mesa,
               pedido.garcom_id);
    }

    fila_pedidos_destruir(&fila_pedidos);
    fila_clientes_destruir(&fila_clientes);

    return EXIT_SUCCESS;
}
