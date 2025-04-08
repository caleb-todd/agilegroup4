#include "crypto_util.h"
#include <algorithm>
using namespace std;

string simple_encrypt(const string& input){//just for demo, need better encrypt / decrypt for actual product
    const char key = 0x55;
    string output = input;

    for(size_t i = 0; i < output.size(); ++i){
        output[i] ^= key;
    }
    return output;
}

string simple_decrypt(const string& input){
    return simple_encrypt(input);
}