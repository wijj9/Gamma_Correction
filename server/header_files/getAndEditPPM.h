#include <stdint.h>

#ifndef TEAM165_GETANDEDITPPM_H
#define TEAM165_GETANDEDITPPM_H

uint8_t* getImage(char* path);
void wrongFormatAndExit(const char* message);
void save_newImage(char* path, uint8_t image[]);
void save_newImage_P5(char* path, uint8_t image[]);

#endif //TEAM165_GETANDEDITPPM_H
