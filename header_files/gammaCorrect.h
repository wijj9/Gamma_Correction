#include <stdint.h>

#ifndef TEAM165_GAMMACORRECT_H
#define TEAM165_GAMMACORRECT_H


void gamma_correct_P5_V0(const uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result);
void gamma_correct_P6_V1(uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result);
void gamma_correct_P5_V2(uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result);

float abs_func(float num);
float root(float num, int depth);
float pow1(float num, float result, float exp);
float pow2(float num, float result, float exp, int depth, int genauigkeit);
float power(float num, float exp);

#endif //TEAM165_GAMMACORRECT_H
//



