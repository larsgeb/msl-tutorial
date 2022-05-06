#include <iostream>
#include <chrono>
#include <math.h>
#include <iomanip>
#include <omp.h>
#include <stdlib.h>
#include <assert.h>
#define PI 3.14159265

#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"

#include "MetalOperations.hpp"

void add_arrays(float *x, float *y, float *z, size_t length);
void add_arrays_parallel(float *x, float *y, float *z, size_t length);
void output_array(float *array, size_t length);
int omp_thread_count();
bool equalFloat(float a, float b, float epsilon);
bool equalArray(const float *x, const float *y, size_t arrayLength);

int main(int argc, char *argv[])
{

    // size_t arrayLength = 1024;
    size_t arrayLength = 67108864;

    // Load GPU
    MTL::Device *device = MTL::CreateSystemDefaultDevice();
    MetalOperations *arrayOps = new MetalOperations(device);
    // Create GPU arrays
    const unsigned int bufferSize = arrayLength * sizeof(float);
    MTL::Buffer *x_gpu = device->newBuffer(bufferSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer *y_gpu = device->newBuffer(bufferSize, MTL::ResourceStorageModeManaged);
    MTL::Buffer *z_gpu = device->newBuffer(bufferSize, MTL::ResourceStorageModeManaged);
    // Get a C++-style reference to the gpu array
    float *x = (float *)x_gpu->contents();
    float *y = (float *)y_gpu->contents();
    float *z = (float *)z_gpu->contents();

    std::cout << "Array size: " << arrayLength << std::endl;

    for (size_t i = 0; i < arrayLength; i++)
    {
        x[i] = i * 1.0f;
    }
    for (size_t i = 0; i < arrayLength; i++)
    {
        y[i] = i * i;
    }

    float *z_serial = new float[arrayLength];
    float *z_parallel = new float[arrayLength];

    // Execute operations
    add_arrays(x, y, z_serial, arrayLength);
    add_arrays_parallel(x, y, z_parallel, arrayLength);
    arrayOps->addArrays(x_gpu, y_gpu, z_gpu, arrayLength);

    assert(equalArray(z_serial, z_parallel, arrayLength));
    assert(equalArray(z_serial, z, arrayLength));
    std::cout << "CPU serial, OpenMP and GPU code produce same result." << std::endl
              << std::endl;

    // Don't scroll further !! Scary stuff. --------------------------------------------

    size_t operation_repeats = 10;

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

    start = std::chrono::high_resolution_clock::now();
    for (size_t j = 0; j < operation_repeats; j++)
    {
        arrayOps->addArrays(x_gpu, y_gpu, z_gpu, arrayLength);
    }
    end = std::chrono::high_resolution_clock::now();
    mean_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() / float(operation_repeats);
    std::cout << std::setw(30) << "Mean time (GPU): ";
    std::cout << float(mean_duration) / 1000000.0 << "[ms]" << std::endl;
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
#pragma omp parallel for
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

bool equalFloat(float a, float b, float epsilon)
{
    return fabs(a - b) <= ((fabs(a) > fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}

bool equalArray(const float *x, const float *y, size_t arrayLength)
{
    for (unsigned long index = 0; index < arrayLength; index++)
    {
        if (!equalFloat(x[index], y[index], std::numeric_limits<float>::epsilon()))
        {

            printf("Compute ERROR: index=%lu x=%e vs y=%e\n",
                   index, x[index], y[index]);
            return false;
        };
    }
    return true;
}