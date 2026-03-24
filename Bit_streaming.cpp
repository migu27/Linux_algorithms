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
const int INPUT_SZ = 8;
uint8_t input[INPUT_SZ];

// ________________________________________ Helper functions
void init() {
    // Init the input data with some recognizable simple pattern
    for (int i = 0; i < INPUT_SZ; ++i) {
        input[i] = 0x14; //0x15; //0x1F;
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
    

    memset(output_buffer, 0, sizeof(output_buffer));
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

uint8_t get_mask(const uint8_t bits_to_read) {
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

void get_digits(uint8_t number, uint8_t *result) {
    // Returns an array of 5 elements where each contain a single bit 
    // corresponding to the binary representation of "number"
    // (memory has to be reserved by the caller). Example:
    // number = 10100
    // result[0] = 1, result[1] = 0, result[2] = 1, result[3] = 0, result[4] = 0
    int i = 4; // The algorithm gets first the LSB, so we start the array by its last position
    while (number > 0) {
        result[i--] = number % 2;
        number /= 2;
    }
}

// _________________________________________ Helper functions (end)

void algorithm_1() {
    // This algorithm avoids the hell of shifts and masks, but requires more memory.
    // It converts each original 5 bit value into an array of 5 separate numbers (one
    // number per binary digit), into a bigger array. Then, it is just a matter of reading 
    // elements (=digits) of this array in groups of 8 and converting back into a single  
    // binary number, stored in a single element at the output.
    // The additional memory needed is for the temporary buffer is 8 times the output size.
    int i = 0;
    uint8_t tmp_array[BUFFER_SZ * 8];
    int output_idx = 0;
    uint8_t a[5];

    // Traverse the input stream. For each element (byte), discard the 3 significant bits 
    // and convert the remaining 5 bits into 5 separate elements of a temporary buffer ("tmp_array") 
    memset(tmp_array, 0, sizeof(tmp_array));
    while (i < INPUT_SZ) {
        get_digits(input[i++], a);
        for (int j = 0; j < 5; ++j) {
            tmp_array[output_idx++] = a[j];
        }
    }
    
    // If input was: 00010100 00010100 ...
    // Now we have: tmp_array[0] = 1, tmp_array[1] = 0, tmp_array[2] = 1, tmp_array[3] = 0, tmp_array[4] = 0,
    //              tmp_array[5] = 1, tmp_array[6] = 0, tmp_array[7] = 1, tmp_array[8] = 0, tmp_array[9] = 0,...

    // Convert from tmp_array into output
    uint8_t val = 0, exp = 7;
    for (int k = 0, out_idx = 0; k < (BUFFER_SZ * 8 - 1); ++k) {
        val += tmp_array[k] * pow(2, exp);
        if (exp == 0) {
            // Got a full byte: write it and reset counters
            output_buffer[out_idx++] = val;
            val = 0;
            exp = 8;
        }
        --exp;
    }   
}

void algorithm_2() {
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
}

int main(int argc, char* argv[]) {
    init();

    algorithm_1();
    
    print_output();
    
    return 0;
}

