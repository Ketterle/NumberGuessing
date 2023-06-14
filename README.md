# Number Guessing Game

The Number Guessing Game is a client-server application where the client attempts to guess a randomly generated number chosen by the server. The communication between the client and server is facilitated using semaphores and shared memory.

## Files

The program consists of the following files:

- `act3-1.c`: Server program responsible for generating a random number and coordinating the game.
- `act3-2.c`: Cleanup program for the server, used to remove semaphores and shared memory after the game.
- `act3-3.c`: Client program that makes guesses to find the correct number.
- `act3-4.c`: Cleanup program for the client, used to remove semaphores and shared memory after the game.

## Compilation

To compile the program, use the following commands:

gcc act3-1.c -o server -lrt -lpthread
gcc act3-2.c -o server_cleanup
gcc act3-3.c -o client -lrt -lpthread
gcc act3-4.c -o client_cleanup


## Usage

1. Start the server by running the `server` executable.
2. Start the client by running the `client` executable.
3. The client will make guesses to find the correct number chosen by the server.
4. Once the correct number is found, the client will display the result and the number of iterations.
5. After the game, run the `client_cleanup` executable to remove semaphores and shared memory associated with the client.
6. Run the `server_cleanup` executable to remove semaphores and shared memory associated with the server.

Note: Ensure that the cleanup programs (`server_cleanup` and `client_cleanup`) are executed after the respective client and server programs to avoid any resource leaks.

## Dependencies

The program requires the following dependencies:

- `gcc`: GNU Compiler Collection for compiling C programs.
- `librt`: Library for POSIX real-time extensions.
- `libpthread`: Library for multithreading support.

Make sure these dependencies are installed on your system before compiling and running the program.
