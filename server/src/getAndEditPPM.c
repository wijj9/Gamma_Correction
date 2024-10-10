#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define NUMBEROFCHARSPERLINE 100
int width;
int height;

int maxColorVal;

extern int benchmark;

void wrongFormatAndExit(const char* message) {
    printf("The file has a wrong format.\n%s\n", message);
    exit(0);
}


uint8_t* getImage(char* path) {
    char buffer[NUMBEROFCHARSPERLINE];
    FILE* file = fopen(path, "rb"); // opening in binary mode (wb)

    // Check to stop seg-faults:
    if (file == NULL) {
        printf("File was not found. \n");
        exit(0);
    }


    // First line
    if (fgets(buffer, NUMBEROFCHARSPERLINE, file) != NULL) {
        const char format[] = "P6";
        // If the first line has the wrong format
        if (strncmp(format, buffer, strlen(format)) != 0) {
            wrongFormatAndExit("Only PPM files with the binary P6 format are accepted!\n");
        }
    } else {
        wrongFormatAndExit("Incorrect file format\n");
    }

    // skipping comments : Comments can only be between P6 and height/width or between height/width and maxVal
    while (fgets(buffer, NUMBEROFCHARSPERLINE, file) != NULL && buffer[0] == '#') {
        // doing nothing
    }

    // Third line: read height and width
    if (sscanf(buffer, "%d %d", &width, &height) != 2) {
        wrongFormatAndExit("The image does not specify its width and height\n");
    }

    // dimensions need to be strictly positive
    if (width < 1 || height < 1) {
        wrongFormatAndExit("Invalid image dimensions\n");
    }

    while (fgets(buffer, NUMBEROFCHARSPERLINE, file) != NULL && buffer[0] == '#') {
        // doing nothing
    }

    // forth line: read maximum color value
    if (sscanf(buffer, "%d", &maxColorVal) == 1) {
        if (maxColorVal != 255) {
            wrongFormatAndExit("Only the pixel format with maxValue = 255 is supported\n");
        }
    } else {
        wrongFormatAndExit("Invalid color value specification\n");
    }

    // Rest of the file: allocate memory and then read the binary pixel data
    uint8_t* pixels = (uint8_t*) malloc(width * height * 3 * sizeof(uint8_t));

    if (pixels == NULL) {
        printf("Was not able to allocate the memory space needed for this image.\n");
        exit(0);
    }

    size_t bytesRead = fread(pixels, 1, width * height * 3, file);

    if (bytesRead != ((long unsigned int)(width * height * 3))) {
        wrongFormatAndExit("Error reading binary data\n");
    }

    fclose(file);
    return pixels;
}

void save_newImage(char* path, uint8_t image[]) {
    FILE *file = fopen(path, "wb");  // opening in binary mode (wb)
    if (file != NULL) {
        // write P6 identifier
        fprintf(file, "P6\n");
        // write width and height to file
        fprintf(file, "%d %d\n", width, height);
        // write the max color value
        fprintf(file, "255\n");



        // Write binary RGB values in the result file
        int lengthOfArray = width * height * 3;
        //printf("Error after this: %d.\n",lengthOfArray);
        fwrite(image, sizeof(uint8_t), lengthOfArray, file);

        fclose(file);

        if(benchmark == 0) {
            printf("Image Saved Successfully. Check the Result_Images folder!\n");
        }

    } else {
        printf("Saving the image was not possible.\n");
        exit(0);
    }
}

void save_newImage_P5(char* path, uint8_t image[]) {
    FILE *file = fopen(path, "wb");  // opening in binary mode (wb)
    if (file != NULL) {
        // write P5 identifier
        fprintf(file, "P5\n");
        // write width and height to file
        fprintf(file, "%d %d\n", width, height);
        // write the max color value
        fprintf(file, "255\n");

        // Write binary RGB values in the result file
        int lengthOfArray = width * height ;
        //printf("Error after this: %d.\n",lengthOfArray);
        fwrite(image, sizeof(uint8_t), lengthOfArray, file);

        fclose(file);
        if(benchmark == 0) {
            printf("Image Saved Successfully. Check the Result_Images folder!\n");
        }

    } else {
        printf("Saving the image was not possible.\n");
        exit(0);
    }
}