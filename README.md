# Sudoku Solution Validator

This program validates a 9x9 Sudoku solution using multiple threads:
- One thread checks all rows
- One thread checks all columns
- Nine threads check each 3x3 subgrid

## Compilation

Compile the program using the provided Makefile:

```
make
```

This will produce an executable called `validator`.

## Usage

Run the program:

```
./validator
```

You will be prompted to enter the 9x9 Sudoku solution row by row. Enter each number separated by spaces.

Example input:

```
5 3 4 6 7 8 9 1 2
6 7 2 1 9 5 3 4 8
1 9 8 3 4 2 5 6 7
8 5 9 7 6 1 4 2 3
4 2 6 8 5 3 7 9 1
7 1 3 9 2 4 8 5 6
9 6 1 5 3 7 2 8 4
2 8 7 4 1 9 6 3 5
3 4 5 2 8 6 1 7 9
```

## Implementation Details

The program uses pthreads to validate the solution concurrently:
- Thread 0: Validates all rows
- Thread 1: Validates all columns
- Threads 2-10: Each validates one 3x3 subgrid

A mutex is used to ensure thread-safe updates to the shared validation status.