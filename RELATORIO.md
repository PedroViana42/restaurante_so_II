# Relatório: Simulação concorrente de um restaurante

## 1. Objetivo e uso de threads

O projeto implementa uma parte de uma simulação de restaurante utilizando
threads POSIX (`pthread`). O objetivo é representar a chegada de clientes e o
atendimento realizado por garçons de forma concorrente. A thread geradora cria
clientes em intervalos aleatórios e os coloca na fila de espera. Em paralelo,
três threads de garçons retiram clientes dessa fila, simulam o atendimento e
criam pedidos que são inseridos na fila da cozinha.

Esse modelo é um exemplo de produtor-consumidor. A thread geradora é produtora
de clientes, enquanto os garçons são consumidores da fila de clientes e, ao
mesmo tempo, produtores de pedidos. O uso de várias threads permite que a
chegada de novos clientes ocorra enquanto outros clientes estão sendo
atendidos. A função `pthread_join` é usada no programa principal para esperar
o término da thread geradora e de todos os garçons antes de destruir as filas.

## 2. Estruturas de dados

As entidades da simulação são representadas por duas estruturas principais.
`Cliente` armazena identificador, mesa, quantidade de pessoas, pedido
associado, instante de chegada e status. `Pedido` contém identificador,
cliente, mesa, tipo de prato, garçom responsável, cozinheiro e status. Os
status são definidos por enums, o que torna as transições do sistema mais
claras e evita o uso de valores numéricos sem significado.

As filas são implementadas como listas encadeadas. `NoCliente` e `NoPedido`
guardam um elemento e um ponteiro para o próximo nó. Cada fila possui ponteiros
para início e fim, além de um contador de elementos. Manter os dois ponteiros
permite inserir no final em tempo constante e remover do início também em
tempo constante, caracterizando uma fila FIFO.

`FilaClientes` possui ainda a flag `geracao_finalizada`, um mutex e uma
condição chamada `cond_cliente_disponivel`. `FilaPedidos` possui estrutura
equivalente, com a flag `atendimento_finalizado` e a condição de pedidos. As
estruturas de argumentos (`ArgsGeradorClientes` e `ArgsGarcom`) permitem passar
às threads as filas compartilhadas e os parâmetros da simulação.

## 3. Mutexes e condições de concorrência

Cada fila tem um `pthread_mutex_t` próprio. O mutex protege os ponteiros da
lista, o contador de tamanho e as flags de finalização. Tanto a inserção como
a remoção bloqueiam o mutex antes de alterar a fila e liberam-no depois que a
operação termina. Assim, dois garçons não podem remover o mesmo cliente nem
corromper simultaneamente os ponteiros `inicio` e `fim`.

Na fila de clientes, a função
`fila_clientes_aguardar_remover` usa `pthread_cond_wait` quando não há cliente
disponível. A espera ocorre com o mutex associado bloqueado; a própria função
libera o mutex durante a espera e o readquire antes de continuar. O `while` é
importante porque uma thread pode acordar e encontrar a fila vazia, inclusive
por causa de um despertar espúrio ou porque outra thread consumiu o cliente.

Quando um cliente é inserido, `pthread_cond_signal` acorda um garçom. Quando a
geração termina, `fila_clientes_marcar_geracao_finalizada` altera a flag sob
proteção do mutex e usa `pthread_cond_broadcast` para acordar todos os garçons
que ainda estejam esperando. Dessa forma, eles conseguem sair do laço sem
ficar bloqueados indefinidamente. A fila de pedidos usa o mesmo padrão de
sincronização para permitir uma futura implementação dos cozinheiros.

Os nós são alocados com `malloc` antes da região crítica e liberados após a
remoção. Essa escolha mantém o trecho protegido pelo mutex pequeno. Ao final,
as filas são percorridas e os nós restantes são liberados, seguidos da
destruição das condições e dos mutexes.

## 4. Testes realizados e propostos

Foi feita uma revisão estática do fluxo de criação, espera e encerramento das
threads. O programa cria uma thread geradora, três garçons, espera a geradora
terminar, espera os garçons terminarem, marca o atendimento como finalizado e
remove os pedidos restantes. Também foi verificado que a condição de
finalização é sinalizada depois que todos os clientes foram inseridos.

No WSL, a compilação foi realizada com sucesso usando `make`, sem mensagens de
erro ou avisos do compilador. Em seguida, o programa foi executado com os
parâmetros padrão:

```sh
make
./restaurante
```

Na execução observada, os 10 clientes foram gerados, cada cliente foi atendido
uma única vez, foram criados 10 pedidos e os três garçons encerraram
normalmente. Como esperado na versão atual, os 10 pedidos permaneceram na fila
aguardando a cozinha.

Os testes adicionais recomendados são: executar com intervalo mínimo e máximo
iguais a zero; testar poucos clientes, como 1 ou 2; e testar mais clientes que
garçons. Em todos os casos, deve-se verificar que cada cliente aparece uma
única vez, que cada pedido corresponde a um cliente e que todas as threads
terminam.

Também é útil executar repetidamente e compilar com as ferramentas de
detecção de problemas disponíveis no ambiente, como ThreadSanitizer. Logs com
identificador da thread, cliente e pedido ajudam a demonstrar que os garçons
executam de forma concorrente e que a ordem de atendimento pode variar.

## 5. Análise e limitações

O principal benefício do projeto é a separação entre dados, operações de fila
e funções das threads. O padrão produtor-consumidor evita que os garçons
precisem consultar a fila continuamente, reduzindo espera ocupada. O uso de um
mutex por fila também diminui a interferência entre operações independentes.

A implementação atual ainda representa somente as etapas de chegada,
atendimento e criação do pedido. Não há threads de cozinheiros, fila de pratos
prontos ou threads de entrega. Por isso, os pedidos permanecem na fila da
cozinha e são apenas impressos pelo `main` ao final. A flag
`atendimento_finalizado` está preparada para a futura espera de cozinheiros,
mas atualmente não é usada por uma thread consumidora.

Como evolução, seria necessário adicionar cozinheiros que aguardem pedidos,
simulem o preparo e encaminhem pratos prontos, além de uma etapa de entrega.
Também seria possível proteger contadores globais de pedidos, controlar a
ocupação das mesas e usar uma fonte de aleatoriedade independente por thread.
Mesmo com essas limitações, a parte implementada demonstra corretamente os
conceitos de thread, fila compartilhada, exclusão mútua, condição de espera e
encerramento coordenado.
