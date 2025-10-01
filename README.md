# Prática TCP – Cliente/Servidor em C

## Objetivo
Implementar programas cliente/servidor em C usando **TCP sockets**, entendendo o funcionamento de conexões, envio/recepção de dados e tratamento sequencial de múltiplos clientes.  


## Estrutura e funcionalidades dos arquivos

### servidor.c
Servidor TCP básico:
- Escuta em todas as interfaces na porta indicada por parâmetro.
- Aceita clientes sequencialmente.
- Imprime IP e porta do cliente.
- Envia uma mensagem de saudação (em uma chamada `send()`).

### cliente.c
Cliente TCP básico:
- Recebe IP e porta como parâmetros.
- Conecta ao servidor, aguarda opcionalmente (`sleep`).
- Lê a mensagem com `recv()` e imprime número de bytes e conteúdo.

### servidor2c.c
Servidor modificado para experimentos:
- Envia **duas mensagens** ao cliente usando duas chamadas `send()` (em vez de uma).
- Permite testar se o cliente recebe ambas as mensagens em uma única chamada de `recv()`.
- Funcionalidade extra para experimentos de fragmentação do fluxo TCP.

### cliente2d.c
Cliente modificado para experimentos:
- Remove o `sleep` antes de receber.
- Usa um **laço while** com `recv()` para receber dados em blocos de tamanho configurável (por exemplo, 5 ou 10 bytes).
- Imprime cada bloco recebido e o total de bytes.
- Permite observar como o TCP entrega os dados em partes, dependendo do buffer e do tempo de envio.

### servidormay.c
Servidor “maiúsculas”:
- Recebe linhas de texto de um cliente.
- Converte cada linha em maiúsculas com `toupper()`.
- Retorna a linha convertida ao cliente.

### clientemay.c
Cliente “maiúsculas”:
- Recebe um arquivo de entrada, IP e porta como parâmetros.
- Envia o arquivo **linha a linha** ao servidor.
- Recebe o retorno em maiúsculas.
- Gera um arquivo de saída com **nome e extensão em maiúsculas**.

### p1.c
Exemplo de manipulação de endereços IP e conversão de ordem de bytes:
- Demonstra uso de `inet_pton`, `inet_ntop`, `htons`, `ntohs`.
- Não é um cliente/servidor, mas serve para estudo das funções de rede.

## Diferenças entre os arquivos originais e os de experimento

- **servidor2c.c** vs **servidor.c**: O servidor2c.c envia duas mensagens separadas com `send()`, enquanto o servidor.c envia tudo em uma só. Isso permite testar como o cliente recebe dados fragmentados.
- **cliente2d.c** vs **cliente.c**: O cliente2d.c recebe dados em laço, com blocos de tamanho configurável, e imprime cada bloco. O cliente.c recebe tudo em uma chamada e imprime o resultado final. O cliente2d.c permite observar a fragmentação do fluxo TCP.

## Experimentos
1. **Dois `send()` vs. um `recv()`**  
   - Com `sleep`: cliente normalmente recebe os dois envios juntos.  
   - Sem `sleep`: cliente pode receber apenas parte da mensagem.  
   - **Conclusão:** TCP é fluxo de bytes, não de mensagens.

2. **Uso de `while(recv() > 0)`**  
   - O cliente processa o fluxo em blocos do tamanho do buffer (`5`, `10`, etc.).  
   - O loop termina quando `recv` retorna **0** (o servidor fechou a conexão).

3. **Servidor de maiúsculas com múltiplos clientes**  
   - Atende clientes **sequencialmente**.  
   - Se dois clientes conectam, o segundo aguarda até o primeiro terminar.

## Requisitos cumpridos
- Uso apenas de funções vistas em aula (`getaddrinfo`, `getnameinfo`, `recv`, `send`, etc.).  
- Suporte a linhas de até **1000 caracteres**.  
- Verificação de erros em todas as chamadas.  
- Tratamento robusto de entradas inválidas.  
- Memória e descritores liberados corretamente.  
- Código comentado, formatado e compilado sem *warnings* com `-Wall`.  
- Programas funcionam em máquinas diferentes (captura de tela incluída).  

## Como compilar
```bash
gcc -Wall -Wextra -O2 servidor.c -o servidor
gcc -Wall -Wextra -O2 cliente.c -o cliente
gcc -Wall -Wextra -O2 servidormay.c -o servidormay
gcc -Wall -Wextra -O2 clientemay.c -o clientemay
```

## Como testar apenas o servidor (usando netcat)

### Passos:
1. Abra o terminal 1 e execute:
  ```bash
  sudo apt update
  sudo apt install netcat-openbsd
  gcc -Wall servidor.c -o servidor
  ./servidor 12345
  ```
2. Abra o terminal 2 e execute:
  ```bash
  nc 127.0.0.1 12345
  # Você verá a mensagem enviada pelo servidor
  ```

## Como testar servidor e cliente em C

### Passos:
1. Compile ambos:
  ```bash
  gcc -Wall servidor.c -o servidor
  gcc -Wall cliente.c -o cliente
  ```
2. Abra o terminal 1 e execute:
  ```bash
  ./servidor 12345
  ```
3. Abra o terminal 2 e execute:
  ```bash
  ./cliente 127.0.0.1 12345
  # O cliente mostrará a mensagem recebida e o número de bytes
  ```