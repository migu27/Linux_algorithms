#include <iostream>
#include <cstdint>
#include <cstring>

using namespace std;
const char* text = "A string-searching algorithm, sometimes called string-matching algorithm, is an algorithm that \
                    searches a body of text for portions that match by pattern. \
                    A basic example of string searching is when the pattern and the searched text are arrays of \
                    elements of an alphabet (finite set) S. S may be a human language alphabet, for example, the \
                    letters A through Z and other applications may use a binary alphabet (A = {0,1}) or a DNA \
                    alphabet (S = {A,C,G,T}) in bioinformatics.\
                    In practice, the method of feasible string-search algorithm may be affected by the string \
                    encoding. In particular, if a variable-width encoding is in use, then it may be slower to \
                    find the Nth character, perhaps requiring time proportional to N. This may significantly \
                    slow some search algorithms. One of many possible solutions is to search for the sequence \
                    of code units instead, but doing so may produce false matches unless the encoding is \
                    specifically designed to avoid it";
char* pattern = (char*)"may";

void naive_string_matching_v1() {
    uint32_t num_ops = 0;
    for (uint32_t i = 0; i < strlen(text) - strlen(pattern); ++i) {
        bool found = true;
        for (uint32_t j = 0; j < strlen(pattern); ++j) {
            found = found && (bool)(text[i+j] == pattern[j]);
	    num_ops++;
        }
        if (found) {
            cout << "Found pattern '" << pattern << "' at position " << i << " ... ";
            for (uint32_t k = i-10; k < i + strlen(pattern) + 10; ++k)
                cout << text[k];
            cout << endl;
        }
    }
    cout << "Text length:" << strlen(text) << ", Pattern length:" << strlen(pattern);
    cout << ", Num operations:" << num_ops << "--------" << endl;
}

void naive_string_matching_v2() {
    uint32_t num_ops = 0;
    for (uint32_t i = 0; i < strlen(text) - strlen(pattern); ++i) {
        bool found = true;
        for (uint32_t j = 0; j < strlen(pattern) && found; ++j) {
            found = found && (bool)(text[i+j] == pattern[j]);
	    num_ops++;
        }
        if (found) {
            cout << "Found pattern '" << pattern << "' at position " << i << " ... ";
            for (uint32_t k = i-10; k < i + strlen(pattern) + 10; ++k)
                cout << text[k];
            cout << endl;
        }
    }
    cout << "Text length:" << strlen(text) << ", Pattern length:" << strlen(pattern);
    cout << ", Num operations:" << num_ops << "--------" << endl;
}

void naive_string_matching_v3() {
    uint32_t num_ops = 0;
    for (uint32_t i = 0; i < strlen(text) - strlen(pattern); ++i) {
        bool found = true;
        for (uint32_t j = 0; j < strlen(pattern) && found; ++j) {
            found = found && (bool)(text[i+j] == pattern[j]);
            num_ops++;
        }
        if (found) {
            cout << "Found pattern '" << pattern << "' at position " << i << " ... ";
            for (uint32_t k = i-10; k < i + strlen(pattern) + 10; ++k)
                cout << text[k];
            cout << endl;
            i += strlen(pattern);
        }
    }
    cout << "Text length:" << strlen(text) << ", Pattern length:" << strlen(pattern);
    cout << ", Num operations:" << num_ops << "--------" << endl;
}


bool pattern_has_no_rep_chars() {
    bool repeated_char = false;
    for (int i = 0; i < strlen(pattern) && !repeated_char; ++i) {
        char c = pattern[i];
        for (int j = i+1; j < strlen(pattern); ++j) {
            if (c == pattern[j]) {
                repeated_char = true;
                break;
            }
        }
    }
    return (!repeated_char);
}

int main() {
    naive_string_matching_v1();
    naive_string_matching_v2();
    if (pattern_has_no_rep_chars())
	    naive_string_matching_v3();
    return 0;
}
