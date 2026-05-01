*This project has been created as part of the 42 curriculum by mobenais.*

# Description

Master concurrent programming in C through an intense simulation where coders race against burnout while competing for scarce USB dongles. Implement POSIX threads, mutexes, condition variables, and sophisticated scheduling algorithms (FIFO/EDF) to orchestrate resource sharing, prevent deadlocks, and ensure fair access—all while keeping your coders productive before the deadline strikes.

The program is a simple simulation where coders race against burnout while competing for scarce USB dongles.

**Deadlock Prevention**: The deadlock prevention relies on ordering the dongle acquisition: each coder always acquires the dongle with the smaller ID first, except the last coder who acquires the larger ID first. This alternating pattern breaks the circular waiting condition, preventing deadlock.

**Monitor Thread**: The monitor thread continuously checks the burnout status of all coders. When a coder burns out, the monitor sets the simulation state to false, which stops the entire simulation and allows the program to exit cleanly.

# Introduction

## Makefile Utils

In the project you can use:

```bash
make              # Compile and generate binary
make clean        # Clean object files
make fclean       # Clean object files and binary
make re           # Recompile everything
```

## Compilation

To compile, you need to provide the following parameters:

- `x_coders` - Number of coders
- `x_time_to_burnout` - Time until burnout (in milliseconds)
- `x_compilation` - Time to compile (in milliseconds)
- `x_refactor` - Time to refactor (in milliseconds)
- `x_debug` - Time to debug (in milliseconds)
- `x_routine` - Number of routines required
- `dongle_cooldown` - Dongle cooldown time (in milliseconds)
- `scheduler` - Scheduling algorithm (fifo or edf)

### Example of valid compilation:

```bash
./codexion 3 2000 200 200 200 3 10 fifo
```

# Resources

## AI Usage

Ask questions to be guided, correct syntaxes and logic, and learn about concepts when blocked.

## Documentation

- **Understand the problem**: [Dining Philosophers Problem](https://de.wikipedia.org/wiki/Philosophenproblem)
- **POSIX Thread Basics**: [CMU Introduction to Pthreads](https://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html)