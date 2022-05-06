#include <iostream>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <stdlib.h>
#define PI 3.14159265

void add_arrays(float *x, float *y, float *z, size_t length);
void output_array(float *array, size_t length);

int main(int argc, char *argv[])
{

    std::cout << "Hello World!" << std::endl;

    // Create array x
    size_t arrayLength = 4;
    float *x = new float[arrayLength];

    // Initialize array x
    for (size_t i = 0; i < arrayLength; i++)
    {
        x[i] = i * 1.0f;
    }

    // Output array
    std::cout << "Array x" << std::endl;
    output_array(x, arrayLength);

    // Initialize, fill and output array y
    float *y = new float[arrayLength];
    for (size_t i = 0; i < arrayLength; i++)
    {
        y[i] = i * i;
    }
    std::cout << "Array y" << std::endl;
    output_array(y, arrayLength);

    // Create array z, and comput z = x + y
    float *z = new float[arrayLength];
    add_arrays(x, y, z, arrayLength);
    std::cout << "Array x + y = z" << std::endl;
    output_array(z, arrayLength);
}

void add_arrays(float *x, float *y, float *z, size_t length)
{
    // Add two arrays together
    for (size_t i = 0; i < length; i++)
    {
        // TODO .. ?? ..
    }
}

void output_array(float *array, size_t length)
{
    // Output array to the terminal.
    for (size_t i = 0; i < length; i++)
    {
        std::cout << std::setw(12) << array[i];

        if (i < length - 1)
            std::cout << ", ";

        if (i == length - 1)
            std::cout << std::endl;
    }
}
