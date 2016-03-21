//
//  simulator.cpp
//  AllenCahnFD
//
//  Created by Yue Sun on 7/20/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#include "simulator.hpp"
#include "util.hpp"


/* initializers */

template <typename T>
Simulator<T>::Simulator(const unsigned int nx,
                        const unsigned int ny,
                        const unsigned int nz,
                        const unsigned int ndim) : context(_context), device(_device), queue(_queue),
                                                   current_step(_current_step), time(_time)

{
    _dim = {nx, ny, nz};
    _ndim = ndim;
    _size = nx*ny*nz;
    _current_step = 0;
    _time = 0;
}


template <typename T>
Simulator<T>::Simulator(const Dim3 dim,
                        const unsigned int ndim) : context(_context), device(_device), queue(_queue),
                                                   current_step(_current_step), time(_time)
{
    _dim = dim;
    _ndim = ndim;
    _size = dim.x*dim.y*dim.z;
    _current_step = 0;
    _time = 0;
}


template <typename T>
Simulator<T>::~Simulator()
{
    if (_cl_initialized)
    {
        if (queue != 0)
            clReleaseCommandQueue(queue);
        
        if (device != 0)
            clReleaseDevice(device);
        
        if (context != 0)
            clReleaseContext(context);
    }
}


/* data transfer between host and device */

// write array from host to device

// Buffer type
template <typename T>
cl_int Simulator<T>::WriteArrayToBuffer(const cl_mem & image, const T* orig, size_t x, size_t y, size_t z)
{
    size_t offset=0;
    size_t size=x*y*z*sizeof(T);
    CHECK_ERROR(clEnqueueWriteBuffer(_queue, image, true, offset, size, orig, 0, NULL, NULL));
    return CL_SUCCESS;
}

// Image type
template <typename T>
cl_int Simulator<T>::WriteArrayToImage(const cl_mem & image, const T* orig, size_t x, size_t y, size_t z)
{
    size_t offset[3]={0,0,0};
    size_t size[3]={x,y,z};
    CHECK_ERROR(clEnqueueWriteImage(_queue, image, true, offset, size, 0, 0, orig, 0, NULL, NULL));
    return CL_SUCCESS;
}

// read array from device to host

// Buffer type
template <typename T>
cl_int Simulator<T>::ReadArrayFromBuffer(const cl_mem & image, T* dest, size_t x, size_t y, size_t z)
{
    size_t offset=0;
    size_t size=x*y*z*sizeof(T);
    CHECK_ERROR(clEnqueueReadBuffer(_queue, image, true, offset, size, dest, 0, NULL, NULL));
    return CL_SUCCESS;
};

// Image type
template <typename T>
cl_int Simulator<T>::ReadArrayFromImage(const cl_mem & image, T* dest, size_t x, size_t y, size_t z)
{
    size_t offset[3]={0,0,0};
    size_t size[3]={x,y,z};
    CHECK_ERROR(clEnqueueReadImage(_queue, image, true, offset, size, 0, 0, dest, 0, NULL, NULL));
    return CL_SUCCESS;
};


/* initialize OpenCL */

template <typename T>
cl_int Simulator<T>::init_cl(const cl_context & context_, const cl_device_id & device_, const cl_command_queue & queue_)
{
    if (device_==NULL) return CL_INVALID_DEVICE;
    _context=context_;
    if (context_==NULL)return CL_INVALID_CONTEXT;
    _device=device_;
    if (queue_==NULL)
    {
        cl_int err;
        CHECK_RETURN(_queue, clCreateCommandQueue(context_, device_, 0, &err), err);
    }
    else {_queue=queue_;}
    return CL_SUCCESS;
}

template <typename T>
cl_int Simulator<T>::init_cl()
{
    cl_int err;
    CHECK_RETURN(_context, CreateContext(err), err);
    CHECK_RETURN(_queue, CreateCommandQueue(_context, &_device, err), err)
    return CL_SUCCESS;
}


template <typename T>
cl_int Simulator<T>::init_cl(const cl_device_type & device_type, const unsigned int & device_num)
{
    cl_int err;
    CHECK_RETURN(_context, CreateContext(_device, device_type, device_num, err), err);
    CHECK_RETURN(_queue, CreateCommandQueue(_context, &_device, err), err);
    return CL_SUCCESS;
}



/* simulation steps */

template <typename T>
void Simulator<T>::step(T dt)
{
    _current_step++;
    _time+=dt;
}


template <typename T>
void Simulator<T>::steps(const T dt, const unsigned int nsteps, const bool finish, const bool cputime)
{
    _current_step+=nsteps;
    _time+=dt*nsteps;
}

template <typename T>
void Simulator<T>::restart(const unsigned int t, const T current_time)
{
    _current_step = t;
    _time = current_time;
}


/* write output file */

template <typename T>
void Simulator<T>:: writefile(const std::string & filename, const T * data)
{
    if (data == NULL)
        data = _data;
    
    if (filename=="")
        write2bin(time2fname("out_", current_step), data, _size);
    else
        write2bin(filename, data, _size);
}



template class Simulator<float>;
template class Simulator<double>;
