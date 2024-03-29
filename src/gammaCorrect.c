#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include <time.h>

extern int benchmark;

// berechnet den Betrag
float abs_func(float num) {
    return (num < 0.0 ? num * -1.0 : num);
}

// root description: gives back the root of a float in a float.
// Warning: depth >= 1
float root(float num, int depth) {
    // x = result, epsilon = Genauigkeit
    float x = num;
    float epsilon = 0.0001;
    while ((x * x - num) > epsilon) {
        // Newton-Raphson Verfahren (siehe Erklärung in Ausarbeitung)
        x = (x + num / x) / 2.0;
    }
    if(depth==1)
        return x;
    else
        return root(x,depth-1);
}

// pow1 description: gives back the num powered by the number before Komma of exp knowing that result always begins with 1
// examples: pow1(2.5,1,3.9) = pow1(2.5,1,3.0)=2.5^3
// if exp = 0 then pow1 = 1 for all num in R
float pow1(float num, float result, float exp) {
    if((int)exp>0)
        return pow1(num,result*num,exp-1);
    else
        return result;
}

// pow2 description: gives back the given result multiplied by (num powered by the number after komma of exp)
// Warning: depth >= 1
// the parameter genauigkeit represents the number of the processed bits after Komma in the Fixkommadarstellung of exp.
// example: let's say result = 1, num = 2.0 and exp = 3.75 (depth always 1 and genauigkeit > 10 for better results)
// the functions would get the 0.75 part of the exp and see it in binary Fixkommadarstellung : (0.75)decimal = (0.11)binary
// then gives you 2.0^0.5 * 2.0^0.25 which is 2.0^0.75.
float pow2(float num, float result, float exp, int depth, int genauigkeit) {

    // lösche unnötige Zyklen bis Genauigkeit erreicht.
    if(exp==0) {
        return result;
    }

    // Im ersten Zyklus, vorkommazahl von Gamma entfernen.
    if(depth==1) {
        exp = exp - ((int) exp);
    }

    //euklidische Algorithmus
    if (depth<=genauigkeit) {
        exp *= 2;
        if(exp>=1) { // next bit is 1 in fixkommadarstellung
            int test = 0;
            float xx = num;
            if(num<1 && num!=0) {
                xx=1/num;
                test = 1;
            }
            float rootx = root(xx,depth);
            if(test==1 && rootx!=0) {
                rootx=1/rootx;
            }
            result *= rootx;
            return pow2(num,result,exp-1,depth+1,genauigkeit);
        } else { // next bit is 0 in fixkommadarstellung
            return pow2(num,result,exp,depth+1,genauigkeit);
        }
    } else {
        return result;
    }
}

// power description: this function combines between pow1 and pow2 to give the final result of the power function.
float power(float num, float exp) {
    if(exp==0)
        return 1;
    if(num==0)
        return 0;
    return pow2(num,pow1(num,1,exp),exp,1,32);
}



void gamma_correct_P5_V0(const uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result){

    struct timespec start;
    clock_gettime (CLOCK_MONOTONIC,&start);
    //We have 3 values for every pixel, so the total number of values is width * height * 3
    int length = width * height * 3;

    int j = 0;

    for(int i = 0; i < length; i+=3){ //For every pixel:
        //we calculate the first formula: D = (a * R + b * G + c * B) / (a + b + c)
        uint8_t d = (a * (*(arr+i)) + b * (*(arr+i+1)) + c * (*(arr+i+2))) / (a+b+c);
        // if (a+b+c)=0 then d=0 [getestet]

        //then the second one: Q´ = ((D / 255) ^ gamma ) * 255
        float q = power((float)d / 255, gamma) * 255;

        //Storing the modified gray scale values in the result array
        *(result+j) = (uint8_t) q;
        j++;
    }
    struct timespec end;
    clock_gettime (CLOCK_MONOTONIC,&end);

    if(benchmark == 0) {
        double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        printf("Execution Time of Gamma P5 V0 (main implementation): %.9f\n", time);
    }
}


void gamma_correct_P6_V1(uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result){

    struct timespec start;
    clock_gettime (CLOCK_MONOTONIC,&start);

    //We have 3 values for every pixel, so the total number of values is width * height * 3
    int length = width * height * 3;

    for(int i = 0; i < length; i+=3){ //For every pixel:
        //we calculate the first formula: D = (a * R + b * G + c * B) / (a + b + c)
        uint8_t d = (a * (*(arr+i)) + b * (*(arr+i+1)) + c * (*(arr+i+2))) / (a+b+c);

        //then the second one: Q´ = ((D / 255) ^ gamma ) * 255
        float q = power((float)d / 255, gamma) * 255;


        //Storing the modified RGB values in the result array
        *(result+i) = (uint8_t) q;
        *(result+i+1) = (uint8_t) q;
        *(result+i+2) = (uint8_t) q;
    }

    struct timespec end;
    clock_gettime (CLOCK_MONOTONIC,&end);

    if(benchmark == 0) {
        double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        printf("Execution Time of Gamma P6 V1 (power used): %.9f\n", time);
    }

}

void gamma_correct_P5_V2(uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result){

    struct timespec start;
    clock_gettime (CLOCK_MONOTONIC,&start);
    //We have 3 values for every pixel, so the total number of values is width * height * 3
    int length = width * height * 3;

    int j = 0;

    for(int i = 0; i < length; i+=3){ //For every pixel:
        //we calculate the first formula: D = (a * R + b * G + c * B) / (a + b + c)
        uint8_t d = (a * (*(arr+i)) + b * (*(arr+i+1)) + c * (*(arr+i+2))) / (a+b+c);

        //then the second one: Q´ = ((D / 255) ^ gamma ) * 255
        float q = powf((float)d / 255, gamma) * 255;


        //Storing the modified gray scale values in the result array
        *(result+j) = (uint8_t) q;
        j++;
    }
    struct timespec end;
    clock_gettime (CLOCK_MONOTONIC,&end);

    if(benchmark == 0) {
        double time = end.tv_sec - start.tv_sec + 1e-9 * (end.tv_nsec - start.tv_nsec);
        printf("Execution Time of Gamma P5 V2 (powf used): %.9f\n", time);
    }

}






