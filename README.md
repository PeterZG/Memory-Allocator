
# Memory Allocator: A Simple Sub-Allocator

This project is a custom memory allocator that demonstrates basic memory management concepts. The allocator supports memory allocation, deallocation, and debugging, allowing a user-defined heap size for testing and experimentation.

## Features

1. **Heap Initialization**:
   - Initialize a heap of user-defined size using `init_heap`.
   - Simulate dynamic memory management in a fixed-size environment.

2. **Memory Management**:
   - `my_malloc(size)`: Allocates memory chunks from the heap.
   - `my_free(ptr)`: Frees allocated memory and merges adjacent free chunks.
   - Automatically updates and maintains a free list for efficient allocation.

3. **Debugging Tools**:
   - `dump_heap`: Prints the current state of the heap (e.g., allocated and free chunks).
   - `heap_offset`: Calculates offsets of pointers in the heap for debugging.

4. **Command Simulation**:
   - Interactive commands via `test3` allow dynamic memory operations (allocation, deallocation) with feedback.

## Files

### Core Files
- **`allocator.c`**: Implementation of the memory allocator functions.
- **`allocator.h`**: Interface header file for the allocator.

### Test Files
- **`test1.c`**: Basic tests for heap initialization and memory allocation.
- **`test2.c`**: Advanced tests including multiple allocations and deallocations.
- **`test3.c`**: Interactive test program for allocating and freeing memory using commands.
- **`test3_example_data.txt`**: Example command file for `test3`, simulating allocation and deallocation operations.

### Build Files
- **`Makefile`**: Automates compilation for all test programs and the allocator.

## Usage

### Compilation
Use the provided `Makefile` to compile the allocator and test programs:
```bash
make
```

### Running Tests
1. **Basic Test (`test1`)**:
   ```bash
   ./test1
   ```
   - Verifies heap initialization and simple memory allocation.

2. **Advanced Test (`test2`)**:
   ```bash
   ./test2
   ```
   - Tests complex scenarios with multiple allocations and deallocations.

3. **Interactive Test (`test3`)**:
   ```bash
   ./test3 <heap_size> < test3_example_data.txt
   ```
   - Simulates dynamic memory operations using commands in `test3_example_data.txt`.

### Example Output
Running `test3` with the provided example data file:
```bash
./test3 4096 < test3_example_data.txt
```

Generates output like:
```plaintext
a = allocate(100);
b = allocate(200);
c = allocate(300);
free b;
d = allocate(200);
free c;
free a;
```

Heap state will be displayed after each operation using `dump_heap`.

## Notes

- **Heap Alignment**: Allocations are aligned to 4 bytes to ensure compatibility with system requirements.
- **Error Handling**: Includes checks for invalid operations (e.g., double free, invalid pointer).
- **Customization**: Modify `test3_example_data.txt` to simulate different memory operation scenarios.

## Acknowledgments

This project is part of the COMP1521 course and provides hands-on experience with implementing custom memory allocators.
