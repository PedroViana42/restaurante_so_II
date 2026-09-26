# Restaurante com POSIX Threads

Este projeto esta nos primeiros passos da simulacao: geracao de clientes e
atendimento por garcons.

Arquivos:

- `restaurante.h`: structs principais, fila de clientes e assinaturas.
- `fila_clientes.c`: fila encadeada protegida por `pthread_mutex_t` e `pthread_cond_t`.
- `fila_pedidos.c`: fila da cozinha, tambem sincronizada com mutex e condicao.
- `gerador_clientes.c`: thread que cria clientes, insere na fila e sinaliza consumidores.
- `garcons.c`: threads que retiram clientes da fila e geram pedidos.
- `main.c`: programa pequeno para testar o fluxo clientes -> garcons -> pedidos.
- `prompts.md`: prompts para revisar e evoluir o trabalho por etapas.

Para compilar em Linux, WSL ou ambiente com POSIX Threads:

```sh
make
```

Ou diretamente:

```sh
gcc -Wall -Wextra -pedantic -std=c11 -pthread main.c fila_clientes.c fila_pedidos.c gerador_clientes.c garcons.c -o restaurante
```

Para executar:

```sh
./restaurante
```

Observacao: o `main.c` ainda nao cria cozinheiros. Por enquanto, ele inicia a
thread geradora, cria 3 garcons, espera o atendimento terminar e imprime os
pedidos que ficaram na fila aguardando a cozinha.
