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
# TCP Practice – Client/Server in C

## Objective
Implement client/server programs in C using **TCP sockets**, understanding how connections, data sending/receiving, and sequential handling of multiple clients work.

## Structure and functionalities of the files

### servidor.c
Basic TCP server:
- Listens on all interfaces at the port given as a parameter.
- Handles clients sequentially.
- Prints the client's IP and port.
- Sends a greeting message (in a single `send()` call).

### cliente.c
Basic TCP client:
- Receives IP and port as parameters.
- Connects to the server, optionally waits (`sleep`).
- Reads the message with `recv()` and prints the number of bytes and content.

### servidor2c.c
Server modified for experiments:
- Sends **two messages** to the client using two `send()` calls (instead of one).
- Allows testing if the client receives both messages in a single `recv()` call.
- Extra functionality for TCP stream fragmentation experiments.

### cliente2d.c
Client modified for experiments:
- Removes the `sleep` before receiving.
- Uses a **while loop** with `recv()` to receive data in blocks of configurable size (e.g., 5 or 10 bytes).
- Prints each block received and the total bytes.
- Allows observing how TCP delivers data in parts, depending on the buffer and sending timing.

### servidormay.c
Uppercase server:
- Receives text lines from a client.
- Converts each line to uppercase with `toupper()`.
- Returns the converted line to the client.

### clientemay.c
Uppercase client:
- Receives an input file name, server IP, and port as parameters.
- Sends the file **line by line** to the server.
- Receives the response in uppercase.
- Generates an output file with **name and extension in uppercase**.

### p1.c
Example of IP address manipulation and byte order conversion:
- Demonstrates usage of `inet_pton`, `inet_ntop`, `htons`, `ntohs`.
- Not a client/server, but for studying network functions.

## Differences between original and experimental files

- **servidor2c.c** vs **servidor.c**: servidor2c.c sends two separate messages with `send()`, while servidor.c sends everything in one. This allows testing how the client receives fragmented data.
- **cliente2d.c** vs **cliente.c**: cliente2d.c receives data in a loop, with configurable block size, and prints each block. cliente.c receives everything in one call and prints the final result. cliente2d.c allows observing TCP stream fragmentation.

## Experiments
1. **Two `send()` vs. one `recv()`**  
  - With `sleep`: client usually receives both sends together.  
  - Without `sleep`: client may receive only part of the message.  
  - **Conclusion:** TCP is a byte stream, not a message protocol.

2. **Using `while(recv() > 0)`**  
  - The client processes the stream in blocks of buffer size (`5`, `10`, etc.).  
  - The loop ends when `recv` returns **0** (server closed the connection).

3. **Uppercase server with multiple clients**  
  - Handles clients **sequentially**.  
  - If two clients connect, the second waits until the first finishes.

## Requirements met
- Only uses functions covered in class (`getaddrinfo`, `getnameinfo`, `recv`, `send`, etc.).
- Supports lines up to **1000 characters**.
- Error checking in all calls.
- Robust handling of invalid input.
- Memory and descriptors properly released.
- Code commented, formatted, and compiles without *warnings* using `-Wall`.
- Programs work on different machines (screenshot included).

## How to compile
```bash
gcc -Wall -Wextra -O2 servidor.c -o servidor
gcc -Wall -Wextra -O2 cliente.c -o cliente
gcc -Wall -Wextra -O2 servidormay.c -o servidormay
gcc -Wall -Wextra -O2 clientemay.c -o clientemay
```

## How to test the uppercase server and client

### Step by step:
1. Prepare a text file for testing, for example:
  ```bash
  echo "test line" > INPUT.TXT
  ```

2. Compile the programs:
  ```bash
  gcc -Wall servidormay.c -o servidormay
  gcc -Wall clientemay.c -o clientemay
  ```

3. Open two terminals:
  - **Terminal 1 (server):**
    ```bash
    ./servidormay 12345
    ```
  - **Terminal 2 (client):**
    ```bash
    ./clientemay INPUT.TXT 127.0.0.1 12345
    ```

### What should happen:
- The server will show the client's connection (IP and port).
- The client will read each line from the input file, send it to the server, receive the line converted to uppercase, and save it in the output file (with the name in uppercase, e.g., INPUT.TXT).
- At the end, the client will show: `Converted file saved as: INPUT.TXT`
- The output file will contain all lines from the original file, but in uppercase.