// 16-bit memory allocator, where memory chunks are of fixed size. 
// "16-bit" means that's the maximum amount of memory available (addressable).
// Using the word "slot" instead of "chunk" as size is fixed.

extern "C" {
    #include <stdint.h>
}
#include <iostream>

constexpr int CAPACITY = 128; // Toy size so we can visualize in the console
constexpr int SLOT_SZ = 1;   // Toy size
constexpr int NUM_SLOTS = CAPACITY / SLOT_SZ;
uint8_t heap[CAPACITY]; // The heap is just a chunk of bytes (uint8_t)
bool free_slots[NUM_SLOTS]; // 'False' means the slot in the heap is used.
                           
void init() {
    for (uint16_t i = 0; i < NUM_SLOTS; ++i)
        free_slots[i] = true; 
}

// Allocates one chunk, returning a pointer to it or NULLPTR if there are no free slots
void* mem_alloc() {
    void* result = nullptr;

    // Locate first empty slot
    for (uint16_t i = 0; i < NUM_SLOTS; ++i) {
        if (free_slots[i]) { // Found one!
            free_slots[i] = false;
            result = (void*) &heap[i * SLOT_SZ]; // Or: result = heap + (i * SLOT_SZ)
            break;
        }
    }
    return result;
}

// Frees one slot, giving it back to the heap
void mem_free(void* ptr) {
    // (ptr - heap) is computed in uint8_t (bytes). As we need a slot index, we divide by SLOT_SZ
    // (bytes per slot)
    uint16_t slot_index = ((uint8_t*) ptr - heap) / SLOT_SZ; 
    free_slots[slot_index] = true;
    std::cout << "Freeing slot " << slot_index << std::endl;
}

// ----------------------------------
// Alternative implementation
typedef struct {
    uint8_t data[SLOT_SZ];
    bool used;
} Slot;
Slot heap2[NUM_SLOTS];
// ----------------------------------

void print_heap_status() {
    for (int i = 0; i < NUM_SLOTS; ++i) {
        if (free_slots[i] == true)
            std::cout << "o";
        else
            std::cout << "x";
    }
    std::cout << std::endl;
}

int main() {
    init();
    uint8_t* ptr1 = (uint8_t*) mem_alloc(); 
    print_heap_status();
    uint8_t* ptr2 = (uint8_t*) mem_alloc(); 
    print_heap_status();
    uint8_t* ptr3 = (uint8_t*) mem_alloc(); 
    print_heap_status();
    uint8_t* ptr4 = (uint8_t*) mem_alloc(); 
    print_heap_status();
    uint8_t* ptr5 = (uint8_t*) mem_alloc(); 
    print_heap_status();
    mem_free(ptr3);
    print_heap_status();
    mem_free(ptr4);
    print_heap_status();
    mem_free(ptr1);
    print_heap_status();
    mem_free(ptr2);
    print_heap_status();
    mem_free(ptr5);
    print_heap_status();
    // Is this valid? user requests 2 slots and concatenates into a single one of 128 bytes
    // uint8_t* ptr6 = (uint8_t*) mem_alloc();
    // uint8_t* ptr7 = (uint8_t*) mem_alloc();
    // for (int i = 0; i < 128; ++i) {
        // ptr6[i] = i;
    // }
    // It is not valid! There is NO WARRANTY mem_alloc() returns contiguous slots!!

    return 0;
}

