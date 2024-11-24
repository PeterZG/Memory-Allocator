
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "allocator.h"

// Constants
#define MIN_HEAP 4096          // Minimum heap size
#define MIN_CHUNK_SPLIT 32     // Minimum size to split a chunk
#define HEADER_SIZE (sizeof(struct header)) // Size of the chunk header
#define ALLOC 0x55555555       // Status for allocated chunks
#define FREE 0xAAAAAAAA        // Status for free chunks

// Type Definitions
typedef unsigned char byte;
typedef uintptr_t addr;

// Structure for chunk headers
typedef struct header {
    uint32_t status;      // Status: ALLOC or FREE
    uint32_t size;        // Total chunk size (including header)
    unsigned char data[]; // Data stored in the chunk
} header_type;

// Structure for heap management
typedef struct heap_information {
    byte *heap_mem;          // Pointer to the heap memory
    uint32_t heap_size;      // Total size of the heap
    byte **free_list;        // Array of pointers to free chunks
    uint32_t free_capacity;  // Maximum number of entries in free_list
    uint32_t n_free;         // Current number of free chunks
} heap_information_type;

// Global Variable
static heap_information_type my_heap; // Global heap information

// Helper Function Prototypes
static addr max_heap_addr(void); // Returns the maximum address of the heap
static void update_free_list(void); // Updates the list of free chunks
static void merge_chunks(header_type *prev, header_type *current, header_type *next); // Merges adjacent free chunks
static int compare_chunks(const void *chunk1, const void *chunk2); // Comparator for sorting free list

// Initialize the heap
int init_heap(uint32_t size) {
    // Ensure the heap size is at least MIN_HEAP and aligned to 4 bytes
    size = (size < MIN_HEAP) ? MIN_HEAP : size;
    size = (size + 3) & ~3;

    // Allocate heap memory
    my_heap.heap_mem = calloc(1, size);
    if (!my_heap.heap_mem) {
        fprintf(stderr, "Error: Failed to allocate heap memory.
");
        return -1;
    }

    // Initialize the first free chunk
    header_type *initial_chunk = (header_type *)my_heap.heap_mem;
    initial_chunk->status = FREE;
    initial_chunk->size = size;

    // Initialize heap metadata
    my_heap.heap_size = size;
    my_heap.free_capacity = size / HEADER_SIZE;
    my_heap.n_free = 1;

    // Allocate memory for the free list
    my_heap.free_list = calloc(my_heap.free_capacity, sizeof(byte *));
    if (!my_heap.free_list) {
        fprintf(stderr, "Error: Failed to allocate free list.
");
        return -1;
    }

    my_heap.free_list[0] = my_heap.heap_mem;
    return 0;
}

// Allocate memory from the heap
void *my_malloc(uint32_t size) {
    if (size < 1) return NULL; // Invalid size

    // Align size to 4 bytes and include header size
    size = (size + 3) & ~3;
    uint32_t required_size = size + HEADER_SIZE;

    header_type *best_fit = NULL; // Pointer to the best-fitting chunk
    addr curr = (addr)my_heap.heap_mem;

    // Search for the best-fitting free chunk
    while (curr < max_heap_addr()) {
        header_type *chunk = (header_type *)curr;

        if (chunk->status == FREE && chunk->size >= required_size) {
            if (!best_fit || chunk->size < best_fit->size) {
                best_fit = chunk;
            }
        }

        curr += chunk->size;
    }

    if (!best_fit) return NULL; // No suitable chunk found

    // Split the chunk if it is large enough
    if (best_fit->size >= required_size + HEADER_SIZE + MIN_CHUNK_SPLIT) {
        header_type *new_chunk = (header_type *)((addr)best_fit + required_size);
        new_chunk->status = FREE;
        new_chunk->size = best_fit->size - required_size;

        best_fit->size = required_size;
    }

    best_fit->status = ALLOC; // Mark chunk as allocated
    update_free_list();

    return best_fit->data;
}

// Free allocated memory
void my_free(void *ptr) {
    if (!ptr) {
        fprintf(stderr, "Error: Attempt to free a NULL pointer.
");
        exit(1);
    }

    addr curr = (addr)my_heap.heap_mem;
    header_type *prev = NULL;

    // Traverse the heap to find the chunk
    while (curr < max_heap_addr()) {
        header_type *chunk = (header_type *)curr;

        if ((void *)chunk->data == ptr) {
            if (chunk->status != ALLOC) {
                fprintf(stderr, "Error: Double free detected.
");
                exit(1);
            }

            // Merge with adjacent free chunks if possible
            header_type *next = (curr + chunk->size < max_heap_addr()) ? (header_type *)(curr + chunk->size) : NULL;
            merge_chunks(prev, chunk, next);
            update_free_list();
            return;
        }

        prev = chunk;
        curr += chunk->size;
    }

    fprintf(stderr, "Error: Pointer does not belong to the heap.
");
    exit(1);
}

// Free all heap resources
void free_heap(void) {
    free(my_heap.heap_mem);
    free(my_heap.free_list);
}

// Print the current state of the heap
void dump_heap(int verbosity) {
    uint32_t offset = 0;
    int chunk_index = 0;

    while (offset < my_heap.heap_size) {
        header_type *chunk = (header_type *)(my_heap.heap_mem + offset);
        printf("Chunk %d: [%s] Size: %u, Offset: %u
",
                chunk_index,
                (chunk->status == FREE) ? "Free" : "Allocated",
                chunk->size, offset);

        offset += chunk->size;
        chunk_index++;
    }
}

// Helper Functions

// Returns the maximum address of the heap
static addr max_heap_addr(void) {
    return (addr)my_heap.heap_mem + my_heap.heap_size;
}

// Updates the free list by traversing the heap
static void update_free_list(void) {
    my_heap.n_free = 0;
    addr curr = (addr)my_heap.heap_mem;

    while (curr < max_heap_addr()) {
        header_type *chunk = (header_type *)curr;
        if (chunk->status == FREE) {
            my_heap.free_list[my_heap.n_free++] = (byte *)chunk;
        }
        curr += chunk->size;
    }

    // Sort the free list by address for efficient merging
    qsort(my_heap.free_list, my_heap.n_free, sizeof(byte *), compare_chunks);
}

// Merges adjacent free chunks into one
static void merge_chunks(header_type *prev, header_type *current, header_type *next) {
    if (prev && prev->status == FREE) {
        prev->size += current->size;
        current = prev;
    }

    if (next && next->status == FREE) {
        current->size += next->size;
    }

    current->status = FREE;
}

// Comparator for sorting free chunks by address
static int compare_chunks(const void *chunk1, const void *chunk2) {
    return (*(byte **)chunk1 - *(byte **)chunk2);
}




