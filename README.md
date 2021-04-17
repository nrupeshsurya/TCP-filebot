# TCP messenger

Sending and receiving messages to and from a server for receiving a file from the server. 
## Compiling

The files can be compiled traditionally using gcc
```bash
gcc -o server.out server.c #for producing ./server.out which is the server program
gcc -o client.out client.c #for producing ./client.out which runs the client program
```

## Usage

```bash
./server.out 18000 #runs the server on port 18000
./client.out 127.0.0.1 18000 #localhost port 18000
```