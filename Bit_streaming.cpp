#include <iostream>
#include <cmath>
#include <stdint.h>
#include <string.h>

/*
PROBLEM: We receive a stream of bytes, for example from a sensor:

[A B C D E F G H] [I J K L M N O P] [Q R S T U V X Y] [Z ... ] ...

And have to get the least significant 5 bits of each byte, packing the result into a new stream of bytes:

[D E F G H L M N] [O P T U V X Y Z] ...

This looks trivial but it's not - just give it a try.
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
    // This algorithm avoids shifts and masks, but requires more memory.
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
    /*
        A B C D E F G H   I J K L M N O P  ...    
              ^
              |
            r_bit=4
            
        D E F G H L M N 
        ^
        |
      w_bit=7      
    */
   int r_bit = 4, r_byte = 0;
   int w_bit = 7, w_byte = 0; 
   uint8_t tmp_write = 0;
   uint8_t tmp_val = 0;
   uint8_t mask = 0;
   while (r_byte < INPUT_SZ) {        
        // Read 1 bit from current position (r_bit), if we can. If not, move to next byte
        if (r_bit < 0)
            r_byte++;
        else {
            // Reading from MSB to LSB (higher to lower indices in the array)
            mask = 0;
            mask |= r_bit; // Example: if r_bit == 3 => read_tmp == 00001000  
            tmp_val = input[r_byte] & mask;
            // Put it as LSB in this temporary value
            for (int i = r_bit; i > 0; --i)
                tmp_val = tmp_val >> 1;
            // Now we write this bit, at the LSB of "tmp_read" into its position given by w_byte and w_bit
            if (w_bit < 0)
                ++w_byte;
            for (int i = 0; i <= w_bit; ++i)
                tmp_val << 1;
            // We have the bit in the position indicated by w_bit: let's write it
            output_buffer[w_byte] |= tmp_val;
            // Prepare indices for next iteration
            --r_bit;
            --w_bit;
        }
    }
}

int main(int argc, char* argv[]) {
    init();

    algorithm_1();
    print_output();

    algorithm_2();
    print_output();    
    
    return 0;
}

