#include <metal_stdlib>
using namespace metal;

kernel void add_arrays(device const float* x,
                       device const float* y,
                       device float* z,
                       uint index [[thread_position_in_grid]])
{
    // TODO .. ?? ..
}

