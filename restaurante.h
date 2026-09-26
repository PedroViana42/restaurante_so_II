#ifndef RESTAURANTE_H
#define RESTAURANTE_H

#include <pthread.h>

typedef enum {
    CLIENTE_ESPERANDO = 0,
    CLIENTE_EM_ATENDIMENTO,
    CLIENTE_AGUARDANDO_PRATO,
    CLIENTE_FINALIZADO
} StatusCliente;

typedef struct {
    int id;
    int mesa;
    int qtd_pessoas;
    int pedido_id;
    long tempo_chegada_ms;
    StatusCliente status;
} Cliente;

typedef struct NoCliente {
    Cliente cliente;
    struct NoCliente *prox;
} NoCliente;

typedef enum {
    PEDIDO_CRIADO = 0,
    PEDIDO_EM_PREPARO,
    PEDIDO_PRONTO,
    PEDIDO_ENTREGUE
} StatusPedido;

typedef struct {
    int id;
    int cliente_id;
    int mesa;
    int tipo_prato;
    int garcom_id;
    int cozinheiro_id;
    StatusPedido status;
} Pedido;

typedef struct NoPedido {
    Pedido pedido;
    struct NoPedido *prox;
} NoPedido;

typedef struct {
    NoCliente *inicio;
    NoCliente *fim;
    int tamanho;
    int geracao_finalizada;
    pthread_mutex_t mutex;
    pthread_cond_t cond_cliente_disponivel;
} FilaClientes;

typedef struct {
    NoPedido *inicio;
    NoPedido *fim;
    int tamanho;
    int atendimento_finalizado;
    pthread_mutex_t mutex;
    pthread_cond_t cond_pedido_disponivel;
} FilaPedidos;

typedef struct {
    FilaClientes *fila_clientes;
    int total_clientes;
    int intervalo_min_ms;
    int intervalo_max_ms;
    int total_mesas;
} ArgsGeradorClientes;

typedef struct {
    int id_garcom;
    FilaClientes *fila_clientes;
    FilaPedidos *fila_pedidos;
} ArgsGarcom;

int fila_clientes_inicializar(FilaClientes *fila);
void fila_clientes_destruir(FilaClientes *fila);
int fila_clientes_inserir(FilaClientes *fila, Cliente cliente);
int fila_clientes_remover(FilaClientes *fila, Cliente *cliente);
int fila_clientes_aguardar_remover(FilaClientes *fila, Cliente *cliente);
void fila_clientes_marcar_geracao_finalizada(FilaClientes *fila);

int fila_pedidos_inicializar(FilaPedidos *fila);
void fila_pedidos_destruir(FilaPedidos *fila);
int fila_pedidos_inserir(FilaPedidos *fila, Pedido pedido);
int fila_pedidos_remover(FilaPedidos *fila, Pedido *pedido);
void fila_pedidos_marcar_atendimento_finalizado(FilaPedidos *fila);

void *thread_geradora_clientes(void *arg);
void *thread_garcom(void *arg);

#endif
