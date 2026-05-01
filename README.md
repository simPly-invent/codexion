*This project has been created as part of the 42 curriculum by mobenais.*

# Description

Master concurrent programming in C through an intense simulation where coders race against burnout while competing for scarce USB dongles. Implement POSIX threads, mutexes, condition variables, and sophisticated scheduling algorithms (FIFO/EDF) to orchestrate resource sharing, prevent deadlocks, and ensure fair access—all while keeping your coders productive before the deadline strikes.

The program is a simple simulation where coders race against burnout while competing for scarce USB dongles.

## Blocking cases handled

This implementation addresses several critical concurrency issues to ensure robust and fair resource sharing:

### Deadlock Prevention
The deadlock prevention relies on ordering the dongle acquisition: each coder always acquires the dongle with the smaller ID first, except the last coder who acquires the larger ID first. This alternating pattern breaks the circular waiting condition, preventing deadlock by ensuring no circular dependency chains can form.

### Starvation Prevention
The scheduling algorithms (FIFO and EDF) ensure fair access to dongles. FIFO provides first-come-first-served access, while EDF prioritizes coders with earlier deadlines, preventing any coder from being indefinitely blocked.

### Cooldown Handling
Dongles have cooldown periods after use. The implementation uses condition variables to signal when cooldowns expire, allowing waiting coders to proceed without busy-waiting.

### Precise Burnout Detection
The monitor thread continuously checks the burnout status of all coders using precise timestamp calculations. When a coder burns out, the monitor immediately sets the simulation state to false, stopping all threads cleanly.

### Log Serialization
All logging operations are protected by mutexes to ensure thread-safe output. Messages are serialized to prevent interleaved output from multiple threads, maintaining readable and consistent log sequences.

## Thread synchronization mechanisms

The implementation uses several POSIX threading primitives to coordinate access to shared resources:

### Mutexes (pthread_mutex_t)
- **Dongle Access**: Each dongle has its own mutex protecting the plugged state and cooldown priority queue
- **Simulation State**: A global mutex protects the simulation state variable shared between all threads
- **Logging**: Mutexes ensure atomic log message output preventing interleaved text

### Condition Variables (pthread_cond_t)
- **Dongle Availability**: Each dongle has a condition variable that threads wait on during cooldown periods
- **Broadcast Signaling**: When simulation ends or cooldowns expire, condition variables broadcast to wake all waiting threads

### Custom Event Implementation
- **Monitor Thread**: Continuously monitors coder burnout status using precise timing
- **Thread-safe Communication**: The monitor thread safely communicates state changes to worker threads through protected shared variables

### Race Condition Prevention
- All shared data structures (dongle queues, simulation state, timestamps) are protected by appropriate mutexes
- Atomic operations ensure consistent state transitions between threads
- Thread-safe communication between coders and monitor prevents data races

### Thread-safe Communication
- Coders signal their state changes through protected shared structures
- The monitor thread reads these states atomically to make decisions
- Clean shutdown coordination ensures all threads exit gracefully when simulation ends

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