# Description

### Usage:           `./gammaCorr [options] <input_file> --gamma <gamma value> --output <output_file>`


| Options                      | Description                                                                                                        |
|------------------------------|--------------------------------------------------------------------------------------------------------------------|
| `--output or -o`             | specify the output_file                                                                                            |
| `--gamma or -g`              | Floating point number , γ ∈ [0,∞]                                                                                  |
| `--coeffs <a>,<b>,<c> or -c` | The grayscale conversion coefficients a, b and c. If this option is not set, the values you specify should be used |
| `--version <version> or -v`  | define which version should be used                                                                                |
| `--benchmark [cycles] or -b` | print out run-time of function, if `[cycles]` is given, it defines how often function is called                    |
| `--help or -h`               | print out this description as a help message                                                                       |

## Example:
The Input file: `mandrill.ppm`

The Execution command: `./gammaCorr Input_Images/mandrill.ppm --gamma 2.0 --output Result_Images/result.ppm`

![Mandrill Image](Readme_Images/mandrill.png)
![Result Image](Readme_Images/result.png)


