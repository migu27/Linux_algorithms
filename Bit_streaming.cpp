#include <iostream>
#include <cmath>
#include <stdint.h>
#include <string.h>



/*
PROBLEM: We receive a stream of bytes, for example from a sensor:

[A B C D E F G H] [I J K L M N O P] [Q R S T U V X Y] [Z ... ] ...

And have to get the least significant 5 bits of each byte, packing the result into a new stream of bytes:

[D E F G H L M N] [O P T U V X Y Z] ...

This looks trivial but it's not - just give it a try
*/

const int BUFFER_SZ = 12;
uint8_t output_buffer[BUFFER_SZ];
const int INPUT_SZ = 10;
uint8_t input[INPUT_SZ];

void init() {
    // Init the input data with some recognizable simple pattern
    for (int i = 0; i < INPUT_SZ; ++i) {
        input[i] = 0x15; //0x1F;
        uint8_t copy = input[i];
        for (int j = 0; j < 8; ++j) {
            // Print the value digit by digit
            uint8_t val = copy & 0x80;
            val = val >> 7;
            std::cout << (int) val;
            copy = copy << 1;
        }
        std::cout << " ";
    }
    std::cout << std::endl;
    // input ==  00011111 00011111 00011111 00011111 00011111 00011111 00011111 00011111

    memset(output_buffer, 0, sizeof(output_buffer));
}

uint8_t get_mask(uint8_t bits_to_read) {
    /* Bits to read   |   mask
       -----------------------
            5         |   0x1F
            4         |   0x0F 
            3         |   0x07
            2         |   0x03
            1         |   0x01
    */
   return (pow(2, bits_to_read) - 1);
}

void print_output() {
    for (int i = 0; i < BUFFER_SZ; i++) {
        uint8_t copy = output_buffer[i];
        for (int j = 0; j < 8; ++j) {
            // Print the value digit by digit
            uint8_t val = copy & 0x80;
            val = val >> 7;
            std::cout << (int) val;
            copy = copy << 1;
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    init();
    // We need signed values for "w_bit" only, but I'm using "int" for all
    int r_bit = 4, r_byte = 0;
    int w_bit = 7, w_byte = 0; 
    
    /*
        My algorithm:

        In the input stream I keep 2 indices: "r_byte" is the current read byte, and "r_bit" is the 
        next bit to be read, starting as 0 from the right (LSB) in the current read byte. According to 
        this, we start with r_byte == 0 and r_bit == 4:

        A B C D E F G H   I J K L M N O P      
              ^
              |
            r_bit=4
            
        So our first operation would be reading bits D E F G H and copying into the output stream.
        In the output stream I keep 2 indices as well_: "w_bit" points to the next bit to write, and 
        "w_byte" points to the current byte where we are writing. We start with w_byte == 0 and 
        w_bit == 7 (which is the MSB):

        D E F G H L M N 
        ^
        |
      w_bit=7      

        -Case 1: w_bit > r_bit
         This means that we have more room in the output byte than bits available in the input byte.
         I read the bits available, prepare them (masking and shifting appropriately),
         and move to the next input byte. I don't write yet this partial result in the output byte: I'll 
         do that when I have a complete byte to write.


    */

    uint8_t partial = 0;
    uint8_t mask = 0;
    while (r_byte < INPUT_SZ) {
        // We need to read w_bit+1 bits to complete the current written byte, and have available r_bit+1 bits in current read byte
        if (w_bit > r_bit) {
            // Read r_bit+1 (0 to r_bit) and increment r_byte
            uint8_t tmp = input[r_byte];
            mask = get_mask(r_bit + 1);
            tmp = tmp & mask;
            tmp = tmp << (w_bit - r_bit);  
            w_bit -= (r_bit + 1);    
            partial |= tmp;
            r_bit = 0;
        }            
        else {
            // Read w_bit bit from current byte and accomodate in current partial value ORing with what we had
            uint8_t tmp = input[r_byte];
            tmp = tmp >> (r_bit - w_bit);
            mask = get_mask(w_bit + 1);
            tmp = tmp & mask;
            partial |= tmp;
            output_buffer[w_byte] = partial; 
            partial = 0;
            r_bit -= (w_bit + 1);
            w_bit = -1;
        }
        if (r_bit <= 0) {
            r_bit = 4;
            ++r_byte;
        }
        if (w_bit < 0) {
            w_bit = 7;
            ++w_byte;
        }
    } 
    std::cout << "partial: " << (int) partial << ", w_byte: " << (int) w_byte << ", r_byte: " << (int) r_byte << std::endl;       
        
    print_output();
    
    return 0;
}

