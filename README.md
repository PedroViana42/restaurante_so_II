# Restaurante com POSIX Threads

Simulação concorrente das etapas de chegada de clientes, atendimento por
garçons e criação de pedidos. O projeto utiliza threads POSIX, mutexes e
variáveis de condição para sincronizar filas compartilhadas.

## Funcionamento atual

- Uma thread geradora cria 10 clientes em intervalos aleatórios.
- Três threads de garçons retiram clientes da fila de espera em ordem FIFO.
- Cada garçom simula o atendimento e insere um pedido na fila da cozinha.
- O programa principal aguarda todas as threads e imprime os pedidos restantes.
- A etapa de cozinheiros, preparo dos pratos e entrega ainda não foi
  implementada; por isso, os pedidos permanecem na fila da cozinha.

## Estrutura do projeto

- `restaurante.h`: estruturas, estados, argumentos e assinaturas das threads.
- `fila_clientes.c`: fila encadeada de clientes com `pthread_mutex_t` e
  `pthread_cond_t`.
- `fila_pedidos.c`: fila encadeada de pedidos com sincronização própria.
- `gerador_clientes.c`: thread produtora de clientes.
- `garcons.c`: threads consumidoras de clientes e produtoras de pedidos.
- `main.c`: inicialização, criação e encerramento das threads.
- `RELATORIO.md`: relatório técnico sobre threads, estruturas, mutexes, testes
  e análise.
- `prompts.md`: roteiro de prompts usados para orientar a evolução do projeto.

## Compilação e execução

Em Linux ou WSL com GCC e POSIX Threads instalados:

```sh
make
./restaurante
```

Também é possível compilar diretamente:

```sh
gcc -Wall -Wextra -pedantic -std=c11 -pthread \
  main.c fila_clientes.c fila_pedidos.c gerador_clientes.c garcons.c \
  -o restaurante
```

Para remover os arquivos gerados pela compilação:

```sh
make clean
```

## Teste realizado

No WSL, o projeto foi compilado com `make` e executado com os parâmetros
padrão. A execução gerou 10 clientes, criou 10 pedidos, distribuiu o
atendimento entre os 3 garçons e encerrou todas as threads normalmente. Os 10
pedidos foram impressos ao final, aguardando a futura etapa da cozinha.

## Relatório

O relatório completo está em [`RELATORIO.md`](RELATORIO.md). Ele descreve o
modelo produtor-consumidor, as estruturas encadeadas, o uso de mutexes e
condições, os testes e as limitações atuais da simulação.
