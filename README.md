# MultiProcessWorkerSystem

This project demonstrates a multi-process system where a supervisor manages two workers (`worker1` and `worker2`) to process pairs of integers. The system uses inter-process communication (IPC) mechanisms like FIFOs, pipes, and shared memory to coordinate tasks and share results.

## Features
- **Supervisor**: Reads pairs of integers from a file and sends them to `worker1` via a FIFO.
- **Worker1**: Processes the pairs, calculates their greatest common divisor (GCD), and forwards pairs with a GCD of 1 to `worker2`.
- **Worker2**: Finds the pair with the largest absolute difference and stores the result in shared memory.
- **Shared Memory**: Used to store the final result, which is read and displayed by the supervisor.

## Files
- **`supervisor.c`**: The supervisor program that reads pairs from a file and coordinates the workers.
- **`worker1.c`**: The first worker program that processes pairs and forwards them to `worker2`.
- **`worker2.c`**: The second worker program that finds the pair with the largest difference and stores it in shared memory.
- **`starter.sh`**: A shell script that compiles and runs the worker and supervisor programs.
- **`pairs.txt`**: The input file containing pairs of integers to be processed.

## How to Use

### Prerequisites
- A Linux environment.
- GCC compiler (for compiling the C programs).
- Bash shell.

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/omerdikyol/MultiProcessWorkerSystem.git
   cd MultiProcessWorkerSystem
   ```
2. Compile the programs:
   ```bash
    gcc manager/supervisor.c -o manager/supervisor
    gcc workers/worker1.c -o workers/worker1
    gcc workers/worker2.c -o workers/worker2
   ```
3. Run the **`starter.sh`** script::
   ```bash
   ./starter.sh
   ```
## Example Input (**`pairs.txt`**)
   ```bash
    3 5
    7 11
    2 4
    8 12
   ```
## Example Output
   ```bash
   7 * 11 = 77
   ```
## Project Structure
- **`manager/:`**: Contains the **`supervisor.c`** program.
- **`workers/`**: Contains the **`worker1.c`** and **`worker2.c`** programs.
- **`starter.sh`**: The main script that compiles and runs the programs.

## License
This project is open-source and available under the MIT License.

