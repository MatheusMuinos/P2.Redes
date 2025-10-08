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
gcc -Wall -Wextra -O2 servidor2c.c -o servidor2c
gcc -Wall -Wextra -O2 cliente2d.c -o cliente2d
gcc -Wall -Wextra -O2 servidormay.c -o servidormay
gcc -Wall -Wextra -O2 clientemay.c -o clientemay
gcc -Wall -Wextra -O2 p1.c -o p1
```

## How to test each file

### 1. Basic TCP server and client (`servidor.c` and `cliente.c`)

**Step by step:**
1. Open terminal 1 and run:
  ```bash
  ./servidor 12345
  ```
2. Open terminal 2 and run:
  ```bash
  ./cliente 127.0.0.1 12345
  ```
**Expected:**
- The server prints the client's IP and port, and sends a greeting message.
- The client prints the received message and the number of bytes.

### 2. TCP server and client with fragmentation experiment (`servidor2c.c` and `cliente2d.c`)

**Step by step:**
1. Open terminal 1 and run:
  ```bash
  ./servidor2c 12345
  ```
2. Open terminal 2 and run:
  ```bash
  ./cliente2d 127.0.0.1 12345
  ```
  You can edit `cliente2d.c` to change the buffer size (e.g., 5, 10, 20) for each test.
**Expected:**
- The server sends two messages with two `send()` calls.
- The client receives data in blocks and prints each block and the total bytes received.
- You can observe how TCP delivers data in parts depending on buffer size and timing.

### 3. Uppercase server and client (`servidormay.c` and `clientemay.c`)

**Step by step:**
1. Prepare a text file for testing, for example:
  ```bash
  echo "test line" > INPUT.TXT
  ```
2. Open terminal 1 and run:
  ```bash
  ./servidormay 12345
  ```
3. Open terminal 2 and run:
  ```bash
  ./clientemay INPUT.TXT 127.0.0.1 12345
  ```
**Expected:**
- The server prints the client's IP and port.
- The client reads each line from the input file, sends it to the server, receives the line converted to uppercase, and saves it in the output file (with the name in uppercase, e.g., INPUT.TXT).
- At the end, the client prints: `Converted file saved as: INPUT.TXT`
- The output file contains all lines from the original file, but in uppercase.

---

### Experiment: Sequential clients with sleep in clientemay.c

To verify that the server attends multiple clients sequentially, you can add a `sleep()` inside the loop of `clientemay.c` (after writing each line to the output file). This gives you time to launch a second client in another terminal while the first client is still running.

**How to do it:**
1. In `clientemay.c`, add `sleep(10);` after `fputs(resposta, fout);` inside the main loop.
2. Compile the client again:
  ```bash
  gcc -Wall -Wextra -O2 clientemay.c -o clientemay
  ```
3. Start the server:
  ```bash
  ./servidormay 12345
  ```
4. In one terminal, run the first client:
  ```bash
  ./clientemay teste.txt 127.0.0.1 12345
  ```
5. While the first client is sleeping (between lines), open another terminal and run the second client:
  ```bash
  ./clientemay teste2.txt 127.0.0.1 12345
  ```

**What happens:**
- The server accepts the first client and processes its lines one by one, waiting 10 seconds between each line.
- The second client will try to connect, but will have to wait until the first client finishes and disconnects.
- Only after the first client closes the connection, the server will accept and process the second client.
- This demonstrates that the server handles clients sequentially: one at a time, in the order they connect.

---

## servidormay.c
**Functionality:**
- TCP server that receives text lines from a client, converts each line to uppercase, and sends it back.
- Handles one client at a time (sequentially).
- Prints the IP and port of each connected client.
- Usage:
  ```bash
  ./servidormay <port>
  ```
  Example:
  ```bash
  ./servidormay 12345
  ```

## clientemay.c
**Functionality:**
- TCP client that reads lines from an input file, sends each line to the server, receives the uppercase version, and writes it to an output file (with the name in uppercase).
- Prints the name of the output file when finished.
- Usage:
  ```bash
  ./clientemay <input_file> <server_ip> <port>
  ```
  Example:
  ```bash
  ./clientemay teste.txt 127.0.0.1 12345
  ```

**How to run:**
1. Start the server in one terminal:
   ```bash
   ./servidormay 12345
   ```
2. In another terminal, run the client with a text file:
   ```bash
   ./clientemay teste.txt 127.0.0.1 12345
   ```
3. The client will create an output file named TESTE.TXT (uppercase), containing the uppercase version of each line from the input file.
4. The server will display connection information for each client.

### 4. IP and byte order manipulation example (`p1.c`)

**Step by step:**
1. Run:
  ```bash
  ./p1
  ```
**Expected:**
- The program prints examples of IP address conversion between string and binary formats, and byte order conversions using `htons` and `ntohs`.
- Useful for understanding how network functions work in C.

### 5. Testing the server with netcat (alternative to the client in C)

**Step by step:**
1. Install netcat if needed:
  ```bash
  sudo apt update
  sudo apt install netcat-openbsd
  ```
2. Open terminal 1 and run:
  ```bash
  ./servidor 12345
  ```
3. Open terminal 2 and run:
  ```bash
  nc 127.0.0.1 12345
  ```
**Expected:**
- The server prints the client's IP and port.
- The netcat client displays the greeting message sent by the server.