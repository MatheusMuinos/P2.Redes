# Prática TCP – Cliente/Servidor em C

## Objetivo
Implementar programas cliente/servidor em C usando **TCP sockets**, entendendo o funcionamento de conexões, envio/recepção de dados e tratamento sequencial de múltiplos clientes.  

## Estrutura
O projeto contém os seguintes arquivos:

- **servidor.c** – Servidor TCP básico que:
  - Escuta em todas as interfaces na porta indicada por parâmetro.
  - Aceita clientes sequencialmente.
  - Imprime IP e porta do cliente.
  - Envia uma mensagem de saudação (em duas chamadas `send()`).

- **cliente.c** – Cliente TCP básico que:
  - Recebe IP e porta como parâmetros.
  - Conecta ao servidor, aguarda opcionalmente (`sleep`).
  - Lê a mensagem com `recv()` e imprime número de bytes e conteúdo.

- **servidormay.c** – Servidor “maiúsculas” que:
  - Recebe linhas de texto de um cliente.
  - Converte cada linha em maiúsculas com `toupper()`.
  - Retorna a linha convertida ao cliente.

- **clientemay.c** – Cliente “maiúsculas” que:
  - Recebe um arquivo de entrada, IP e porta como parâmetros.
  - Envia o arquivo **linha a linha** ao servidor.
  - Recebe o retorno em maiúsculas.
  - Gera um arquivo de saída com **nome e extensão em maiúsculas**.

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
