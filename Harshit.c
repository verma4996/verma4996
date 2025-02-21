#include <stdio.h>
#include <stdint.h>
#include <math.h>

uint32_t float_to_ieee754(float num) {
    uint32_t ieee754 = 0;

    if (isnan(num)) {
        ieee754 = 0x7FC00000; 
        return ieee754;
    }
    if (isinf(num)) {
        if (num > 0) {
            ieee754 = 0x7F800000;
        } else {
            ieee754 = 0xFF800000; 
        }
        return ieee754;
    }


    
    if (num < 0) {
        ieee754 |= (1 << 31); 
        num = -num; 
    }

    
    int exponent;
    float mantissa;

    if (num == 0.0f) {
        exponent = 0;
        mantissa = 0;
    } else {
      mantissa = frexpf(num, &exponent); 
      mantissa *= 2.0f; 
      exponent += 127 - 1; 
    }
    ieee754 |= (exponent << 23); 

    uint32_t mantissa_bits = (uint32_t)((mantissa - 1.0f) * 0x800000); 
    ieee754 |= mantissa_bits; 

    return ieee754;
}
int main() {
    float num;

    printf("Enter a decimal number: ");
    scanf("%f", &num);

    uint32_t ieee754_representation = float_to_ieee754(num);

    printf("IEEE 754 representation (hex): 0x%08X\n", ieee754_representation);
    printf("Bits: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", (ieee754_representation >> i) & 1);
    }
    printf("\n");
   return 0;
}
