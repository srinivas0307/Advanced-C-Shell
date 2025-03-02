# Advanced C Shell

## Overview
The Advanced C Shell is a custom Unix-like shell implemented in C, providing a command-line interface for users to execute commands, manage processes, and perform input/output redirection and piping. It mimics features of traditional shells with added enhancements for improved usability.

## Features
- Command execution (foreground and background)
- Built-in commands (e.g., `cd`, `exit`, `jobs`, `fg`, `bg`)
- Input/output redirection (`>`, `>>`, `<`)
- Pipelining (`|` support for multiple commands)
- Signal handling (Ctrl+C, Ctrl+Z, and custom signal processing)
- Job control and process management
- Configuration file (`myshrc`) support for startup customization

## Installation

### Prerequisites
Ensure you have the following installed on your system:
- GCC (GNU Compiler Collection)
- Linux environment (recommended for execution and testing)

### Setup
1. Clone this repository:
   ```sh
   git clone <repository-url>
   cd Advanced-C-Shell-main
   ```
2. Compile the shell:
   ```sh
   make
   ```
3. Run the shell:
   ```sh
   ./mysh
   ```

## Usage
- Run shell commands normally:
  ```sh
  ls -l
  ```
- Execute background processes using `&`:
  ```sh
  ./script.sh &
  ```
- Redirect output/input:
  ```sh
  cat file.txt > output.txt
  sort < input.txt
  ```
- Use pipes for command chaining:
  ```sh
  ls -l | grep "txt" | wc -l
  ```
- Manage jobs:
  ```sh
  jobs        # List background jobs
  fg %1       # Bring job 1 to the foreground
  bg %2       # Resume job 2 in the background
  ```
- Exit the shell:
  ```sh
  exit
  ```

## File Structure
```
Advanced-C-Shell-main/
│── src/
│   ├── main.c               # Entry point of the shell
│   ├── commands.c           # Built-in command handling
│   ├── process.c            # Process and job control
│   ├── signals.c            # Signal handling functions
│── include/
│   ├── shell.h              # Header files
│── myshrc                   # Startup configuration file
│── Makefile                 # Compilation script
│── README.md                # Documentation
```

## Acknowledgments
- Inspired by Unix shells like Bash and Zsh
- Referenced Linux man pages and online shell programming resources

