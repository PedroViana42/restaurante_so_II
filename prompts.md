# Prompts para evoluir o trabalho

## Revisar geradora

"Escrevi esta funcao da thread geradora. Revise procurando erros de concorrencia, acesso indevido a fila e uso incorreto de pthreads. Nao reescreva tudo; aponte o problema e me de uma dica de correcao."

## Fazer garcons

"Agora quero implementar as threads de garcons. Elas devem retirar clientes da fila de espera e gerar pedidos para a cozinha. Me ajude a definir o algoritmo em passos, sem fornecer a funcao pronta."

## Fazer cozinha

"As threads de cozinheiros precisam pegar pedidos da fila da cozinha, simular o preparo e colocar o prato na fila de prontos. Me explique onde podem ocorrer condicoes de corrida e quais trechos precisam de mutex."

## Fazer entrega

"Agora quero implementar as threads de entrega. Elas devem pegar pratos prontos e entregar aos clientes. Me ajude a raciocinar sobre o fluxo e sobre a sincronizacao necessaria, sem escrever a solucao completa."

## Entender mutex de verdade

"No meu simulador tenho fila de clientes, fila de pedidos e fila de pratos prontos. Explique, usando meu cenario, por que cada uma precisa de mutex e o que poderia acontecer se eu nao usasse."

## Cacar race condition

"Vou te enviar meu codigo completo. Analise apenas problemas de concorrencia: race conditions, acesso simultaneo a filas, contadores compartilhados e possiveis deadlocks. Nao altere o codigo ainda."

## Debugar travamento

"Meu programa as vezes trava. Quero aprender a descobrir o motivo. Analise a ordem dos pthread_mutex_lock e pthread_mutex_unlock e me ajude a identificar possiveis deadlocks, explicando o raciocinio."

## Validar se realmente esta concorrente

"Como posso provar que garcons, cozinheiros e entrega estao executando concorrentemente? Sugira logs e testes simples que eu possa adicionar ao programa."

## Roteiro de relatorio

"Preciso escrever um relatorio de ate 3 paginas explicando threads, estruturas usadas, mutexes, testes e analise. Faca apenas um roteiro com perguntas que eu devo responder em cada secao; nao escreva o relatorio pronto."
