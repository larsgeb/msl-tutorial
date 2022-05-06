#include <iostream>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <omp.h>
#include <stdlib.h>
#include <assert.h>
#define PI 3.14159265

void add_arrays(float *x, float *y, float *z, size_t length);
void add_arrays_parallel(float *x, float *y, float *z, size_t length);
void output_array(float *array, size_t length);
int omp_thread_count();

int main(int argc, char *argv[])
{

    size_t arrayLength = 2048;
    // size_t arrayLength = 134217728;

    std::cout << "Array size: " << arrayLength << std::endl;

    float *x = new float[arrayLength];
    for (size_t i = 0; i < arrayLength; i++)
    {
        x[i] = i * 1.0f;
    }
    float *y = new float[arrayLength];
    for (size_t i = 0; i < arrayLength; i++)
    {
        y[i] = i * i;
    }

    float *z_serial = new float[arrayLength];
    float *z_parallel = new float[arrayLength];

    // Execute operations
    add_arrays(x, y, z_serial, arrayLength);
    add_arrays_parallel(x, y, z_parallel, arrayLength);

    // Check if the result is the same
    for (size_t i = 0; i < arrayLength; i++)
    {
        assert(z_serial[i] == z_parallel[i]);
    }
    std::cout << "Serial and OpenMP code produce same result." << std::endl;

    // Don't scroll further !! Scary stuff. --------------------------------------------

    size_t operation_repeats = 1000;

    auto start = std::chrono::high_resolution_clock::now();
    for (size_t j = 0; j < operation_repeats; j++)
    {
        add_arrays(x, y, z_serial, arrayLength);
    }
    auto end = std::chrono::high_resolution_clock::now();
    float mean_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / float(operation_repeats);
    std::cout << std::setw(30) << "Mean time (SERIAL): ";
    std::cout << float(mean_duration) / 1000000.0 << "[ms]" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (size_t j = 0; j < operation_repeats; j++)
    {
        add_arrays_parallel(x, y, z_serial, arrayLength);
    }
    end = std::chrono::high_resolution_clock::now();
    mean_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / float(operation_repeats);
    std::cout << std::setw(30) << "Mean time (PARALLEL): ";
    std::cout << float(mean_duration) / 1000000.0 << "[ms]" << std::endl;

    std::cout << std::setw(30) << "Threads used (PARALLEL): ";
    std::cout << omp_thread_count() << std::endl;
}

void add_arrays(float *x, float *y, float *z, size_t length)
{
    // with some added complexity: SAXPY
    for (size_t i = 0; i < length; i++)
    {
        z[i] = 2.12 * x[i] + y[i];
    }
}

void add_arrays_parallel(float *x, float *y, float *z, size_t length)
{
    // with some added complexity: SAXPY
    // TODO .. ?? ..
    for (size_t i = 0; i < length; i++)
    {
        z[i] = 2.12 * x[i] + y[i];
    }
}

void output_array(float *array, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        std::cout << std::setw(12) << array[i];

        if (i < length - 1)
            std::cout << ", ";

        if (i == length - 1)
            std::cout << std::endl;
    }
}

int omp_thread_count()
{
    int n = 0;
#pragma omp parallel reduction(+ \
                               : n)
    n += 1;
    return n;
}
