*This project has been created as part of the 42 curriculum by mobenais.*

# Codexion

A multithreaded simulation in C: N coders share a limited pool of USB
dongles and must keep compiling, debugging and refactoring before they burn
out. Every coder is a POSIX thread; dongles are protected by mutexes and
condition variables; a scheduler (FIFO or EDF) decides who gets a dongle
first.

![codexion run](docs/demo.gif)

## Build and run

```sh
make
./codexion <coders> <burnout> <compile> <debug> <refactor> <cooldown_limit> <cooldown> <scheduler>
./codexion 4 1500 200 200 200 3 10 edf
```

| Argument         | Meaning                                              |
|------------------|------------------------------------------------------|
| `coders`         | number of coder threads                              |
| `burnout`        | time (ms) a coder can go without finishing a cycle   |
| `compile`        | duration (ms) of a compile step (needs 2 dongles)    |
| `debug`          | duration (ms) of a debug step                        |
| `refactor`       | duration (ms) of a refactor step                     |
| `cooldown_limit` | cycles a dongle can run before it needs a cooldown   |
| `cooldown`       | cooldown duration (ms)                               |
| `scheduler`      | `fifo` (first come, first served) or `edf` (earliest deadline first) |

Each line of output is `<timestamp_ms> <coder_id> <event>`. The simulation
stops as soon as a coder burns out, or when every coder has completed its
cycles.

A test script that checks argument validation, scheduling fairness, burnout
detection and data races lives in
[tester_codexion](https://github.com/simPly-invent/tester_codexion).

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

- `number_of_coders` - Number of coders
- `time_to_burnout` - Time until burnout (in milliseconds)
- `time_to_compile` - Time to compile (in milliseconds)
- `time_to_debug` - Time to debug (in milliseconds)
- `time_to_refactor` - Time to refactor (in milliseconds)
- `number_of_compiles_required` - Number of compile cycles required
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