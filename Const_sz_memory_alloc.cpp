// 16-bit memory allocator, where memory chunks are of fixed size. 
// "16-bit" means that's the maximum amount of memory available (addressable).
// Using the word "slot" instead of "chunk" as size is fixed.

extern "C" {
    #include <stdint.h>
}
#include <iostream>

constexpr int CAPACITY = 64 * 1024;
constexpr int SLOT_SZ = 64;
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
Slot heap[NUM_SLOTS];
// ----------------------------------

int main() {
    init();
    uint8_t* ptr1 = (uint8_t*) mem_alloc(); std::cout << (ptr1 - heap) << std::endl;
    uint8_t* ptr2 = (uint8_t*) mem_alloc(); std::cout << (ptr2 - heap) << std::endl;
    uint8_t* ptr3 = (uint8_t*) mem_alloc(); std::cout << (ptr3 - heap) << std::endl;
    uint8_t* ptr4 = (uint8_t*) mem_alloc(); std::cout << (ptr4 - heap) << std::endl;
    uint8_t* ptr5 = (uint8_t*) mem_alloc(); std::cout << (ptr5 - heap) << std::endl;
    mem_free(ptr3);
    mem_free(ptr4);

    return 0;
}

