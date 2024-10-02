#include <iostream>
#include <bitset>
#include <cstdint>

using namespace std;

void multiply_floats(float x, float y) {
    const int bias = 127;

    union {
        float value;
        uint32_t bits;
    } float1, float2, product;

    float1.value = x;
    float2.value = y;

    int sign1 = (float1.bits >> 31) & 1;
    int sign2 = (float2.bits >> 31) & 1;
    int exp1 = ((float1.bits >> 23) & 0xFF) - bias;
    int exp2 = ((float2.bits >> 23) & 0xFF) - bias;

    uint32_t mantissa1 = (float1.bits & 0x7FFFFF) | (1 << 23);
    uint32_t mantissa2 = (float2.bits & 0x7FFFFF) | (1 << 23);

    uint64_t mantissa_product = static_cast<uint64_t>(mantissa1) * mantissa2;

    int exp_result = exp1 + exp2;

    if (mantissa_product & (1ULL << 47)) {
        mantissa_product >>= 1;
        exp_result++;
    }

    if (exp_result + bias >= 255) {
        product.bits = (1 << 31) | (0xFF << 23);
    } else if (exp_result + bias <= 0) {
        product.bits = 0;
    } else {
        product.bits = ((sign1 ^ sign2) << 31) | ((exp_result + bias) << 23) | ((mantissa_product >> 24) & 0x7FFFFF);
    }

    cout << "Resultado en binario: " << bitset<32>(product.bits) << endl;
}

int main() {
    float first_num, second_num;
    cout << "Ingrese el primer numero flotante: ";
    cin >> first_num;
    cout << "Ingrese el segundo numero flotante: ";
    cin >> second_num;

    multiply_floats(first_num, second_num);
    // Ejemplo: puede ingresar 10.5 y 2.375
    // Debe resultar 01000001111000111100000000000000

    return 0;
}
