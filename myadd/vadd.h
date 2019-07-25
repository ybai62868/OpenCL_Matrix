/*
    Yang.Bai
    yb269.cornell.edu
*/

# pragma once
# define CL_HPP_CL_1_2_DEFAULT_BUILD
# define CL_HPP_TARGET_OPENCL_VERSION 120
# define CL_HPP_MINIMUM_OPENCL_VERSION 120
# define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
# include <CL/cl2.hpp>
# include <fstream>
# include <cstdlib>
# include <cstdio>
# include <iostream>
# include <vector>
using std::vector;

# define LENGTH (2)
# define NUM_WORKGROUPS (1)
# define WORKGROUP_SIZE (1) 

template <typename T>
struct aligned_allocator {
    using value_type = T;
    T * allocate(std::size_t num) {
        void * ptr = nullptr;
        if (posix_memalign(&ptr, 4096, num*sizeof(T))) {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T*>(ptr);
    }
    void deallocate(T * p, std::size_t num) {
        free(p);
    }
};