#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#include "./server/header_files/gammaCorrect.h"
#include "./server/header_files/getAndEditPPM.h"

extern int height;
extern int width;

int benchmark = 0;


void gamma_correct_P5_V0(const uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result);
void gamma_correct_P6_V1(uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result);
void gamma_correct_P5_V2(uint8_t *arr, int width, int height, float a, float b, float c, float gamma, uint8_t* result);

void save_newImage(char* path, uint8_t image[]);
void save_newImage_P5(char* path, uint8_t image[]);

float root(float num, int depth);
float pow1(float num, float result, float exp);
float pow2(float num, float result, float exp, int depth, int genauigkeit);
float power(float num, float exp);

//usage guide
void printUsageAndExit(char *name){
    printf("Usage:\n%s [inputPath] [--benchmark iterations] [--gamma gamma] [--output outputPath]  [--coeffs a,b,c] [--help]\n\n", name);
    printf("Required and Optional arguments:\n");
    printf("[inputPath]:\tpath of image (Required!)\n");
    printf("-g --gamma gamma:\tvalue for gamma > 0 (Required!)\n");
    printf("-o --output outputPath:\tpath of result image (Optional)\n");
    printf("-c --coeffs values of a,b and c :\tCoefficients (Optional)\n");
    printf("-h --help :\tDescription of all options that can be given to the program (Optional)\n");

    exit(EXIT_FAILURE);
}


//Parameter is not allowed to start with a ('-' in ASCII is 45 is Decimal) , otherwise it could be treated as another argument.
void checkForValidArgument(char *programName, char operationName) {
    if (optarg[0] == 45){
        printf("Wrong argument for option: -%c\n", operationName);
        printUsageAndExit(programName);
    }
}

int main(int argc, char *argv[]) {
    char *name = argv[0];
    char *inputPath = "";
    char *outputPath = "";

    float gamma = -1;
    char opt = ' ';
    char* coeffs = "";

    int version = 0;

    uint8_t* image = 0;
    uint8_t* result = 0;

    // default values of a,b, and c chosen from us. They are used in the gamma correction if the user doesn't give their own values
    int a = 50;
    int b = 141;
    int c = 39;


    static struct option long_options[] = {
            {"version", required_argument, 0, 'v'},
            {"output", required_argument, 0, 'o'},
            {"gamma", required_argument, 0, 'g'},
            {"coeffs", required_argument, 0, 'c'},
            {"benchmark", required_argument, 0, 'b'},
            {"help", no_argument, 0, 'h'},
            {0,0,0,0}
    };


    if (argc < 2) {
        printUsageAndExit(name);
    }


    if (optind < argc) {
        inputPath = argv[optind];
    } else {
        fprintf(stderr, "Error: Input file path is required.\n");
        return 1;
    }



    // parse through the given arguments and update the parameters
    while ((opt = getopt_long(argc, argv, "v:b:c:g:o:h::", long_options, NULL)) != -1) {
        switch (opt) {
            case 'v':
                // choose the given version
                checkForValidArgument(name, opt);
                if (optarg == NULL){
                    printf("Argument required for -V!");
                    return EXIT_FAILURE;
                } else {
                    version = atoi(optarg);
                    if (version != 0 && version != 1 && version != 2){
                        printf("There are only three implementations of the Gammakorrektur: V0, V1 and V2. Choose one of them.\n");
                        return EXIT_FAILURE;
                    }
                }
                break;
            case 'o':
                // get the given output path
                checkForValidArgument(name, opt);
                outputPath = optarg;
                break;
            case 'g':
                // get the given gamma value
                checkForValidArgument(name, opt);
                gamma = atof(optarg);
                break;
            case 'c':
                // get the chosen a,b and c values (Coefficients for grayscale conversion)
                coeffs = optarg;
                if (sscanf(coeffs, "%d,%d,%d", &a, &b, &c) != 3) {
                    printf("Error parsing coefficients.\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'b':
                // get the number of iteration the needs to be run
                checkForValidArgument(name, opt);
                if(atoi(optarg) > 0) {
                    benchmark = atoi(optarg);
                } else {
                    printf("Number of Iteration must be strictly positive (>0)!");
                }
                break;
            case 'h':
                printUsageAndExit(name);
                break;
            default:
                printUsageAndExit(name);
        }
    }



    // here the image is being read
    image = getImage(inputPath);

    // allocating memory for the result
    result = malloc(width * height * 3);


    if(version == 0) {
        if (benchmark > 0) {
            struct timespec start;
            clock_gettime (CLOCK_MONOTONIC,&start);

            while (benchmark > 0) {
                // resetting the result after each iteration inorder to avoid having multiple images on top of each other in the result image
                result = malloc(width * height * 3);
                gamma_correct_P5_V0(image,width,height,a,b,c,gamma, result);


                benchmark--;
            }
            struct timespec end;
            clock_gettime (CLOCK_MONOTONIC,&end);

            double time = end . tv_sec - start . tv_sec + 1e-9 * (end . tv_nsec - start . tv_nsec );
            printf("Execution Time of Gamma P5 V0 (main implementation): %.9f\n", time);
            save_newImage_P5(outputPath, result);
        } else {
            gamma_correct_P5_V0(image, width, height, a, b, c, gamma, result);
            save_newImage_P5(outputPath, result);
        }
    } else if(version == 1) {
        if (benchmark > 0) {
            struct timespec start;
            clock_gettime (CLOCK_MONOTONIC,&start);

            while (benchmark > 0) {
                // resetting the result after each iteration inorder to avoid having multiple images on top of each other in the result image
                result = malloc(width * height * 3);
                gamma_correct_P6_V1(image,width,height,a,b,c,gamma, result);


                benchmark--;
            }
            struct timespec end;
            clock_gettime (CLOCK_MONOTONIC,&end);

            double time = end . tv_sec - start . tv_sec + 1e-9 * (end . tv_nsec - start . tv_nsec );
            printf("Execution Time of Gamma P6 V1 (power used): %.9f\n", time);
            save_newImage(outputPath, result);
        } else {
            gamma_correct_P6_V1(image,width,height,a,b,c,gamma, result);
            save_newImage(outputPath, result);
        }
    } else {
        if (benchmark > 0) {
            struct timespec start;
            clock_gettime (CLOCK_MONOTONIC,&start);

            while (benchmark > 0) {
                // resetting the result after each iteration inorder to avoid having multiple images on top of each other in the result image
                result = malloc(width * height * 3);
                gamma_correct_P5_V2(image,width,height,a,b,c,gamma, result);


                benchmark--;
            }
            struct timespec end;
            clock_gettime (CLOCK_MONOTONIC,&end);

            double time = end . tv_sec - start . tv_sec + 1e-9 * (end . tv_nsec - start . tv_nsec );
            printf("Execution Time of Gamma P5 V2 (powf used): %.9f\n", time);
            save_newImage_P5(outputPath, result);
        } else {
            gamma_correct_P5_V2(image,width,height,a,b,c,gamma, result);
            save_newImage_P5(outputPath, result);
        }
    }

}