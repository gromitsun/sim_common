//
//  simulator.h
//  sim_common
//
//  Created by Yue Sun on 7/20/15.
//  Copyright (c) 2015 Yue Sun. All rights reserved.
//

#ifndef __SteinbachPhaseFieldFD__simulator__
#define __SteinbachPhaseFieldFD__simulator__

#include <iostream>
#include "cl_common.hpp"


// Uncomment the following line if using OpenCL Buffer objects instead of Image objects
//#define __USE_CL_BUFFER__

typedef struct
{
    unsigned int x;
    unsigned int y;
    unsigned int z;
}Dim3;

template <typename T>
class Simulator
{
protected:
    cl_context          _context;
    cl_device_id        _device;    
    cl_command_queue    _queue;
    
    Dim3 _dim;
    unsigned int _ndim;
    unsigned int _size;
    
    T * _data;
    unsigned int _current_step;
    T _time;
    
    bool _cl_initialized;
    bool _sim_initialized;
    
public:
    /* Variables */
    // OpenCL
    const cl_context          & context;
    const cl_device_id        & device;
    const cl_command_queue    & queue;
    // Simulation
//    const T * const data;
    const unsigned int & current_step;
    const T & time;
    
    /* Constructors & destructors */
    Simulator(const unsigned int nx,
              const unsigned int ny,
              const unsigned int nz,
              const unsigned int ndim);
    Simulator(const Dim3 dim,
              const unsigned int ndim);
    virtual ~Simulator();
    
    /* Functions */
    // OpenCL
    virtual cl_int  init_cl(const cl_context & context_, const cl_device_id & device_, const cl_command_queue & queue_=NULL);
    virtual cl_int  init_cl();
    virtual cl_int  init_cl(const cl_device_type & device_type, const unsigned int & device_num);
    virtual cl_int  build_kernel(const char * filename) {return CL_SUCCESS;};
    virtual void    finishQueue() {if(_queue)clFinish(_queue);}
    
    // write array from host to device
    cl_int WriteArray(const cl_mem & image, const T* orig, size_t x, size_t y, size_t z=1)
    {
        
#ifdef __USE_CL_BUFFER__
        WriteArrayToBuffer(image, orig, x, y, z);
#else
        WriteArrayToImage(image, orig, x, y, z);
#endif
        return CL_SUCCESS;
    }
    
    // Buffer type
    cl_int WriteArrayToBuffer(const cl_mem & image, const T* orig, size_t x, size_t y=1, size_t z=1);
    
    // Image type
    cl_int WriteArrayToImage(const cl_mem & image, const T* orig, size_t x, size_t y=1, size_t z=1);
    
    // read array from device to host
    cl_int ReadArray(const cl_mem & image, T* dest, size_t x, size_t y=1, size_t z=1)
    {
#ifdef __USE_CL_BUFFER__
        ReadArrayFromBuffer(image, dest, x, y, z);
#else
        ReadArrayFromImage(image, dest, x, y, z);
#endif
        return CL_SUCCESS;
    };
    
    // Buffer type
    cl_int ReadArrayFromBuffer(const cl_mem & image, T* dest, size_t x, size_t y=1, size_t z=1);

    // Image type
    cl_int ReadArrayFromImage(const cl_mem & image, T* dest, size_t x, size_t y=1, size_t z=1);

    
    // simulation
    virtual void init_sim() {};
    virtual void step(const T dt);
    virtual void steps(const T dt,const unsigned int nsteps,const bool finish=true, const bool cputime=true);
    virtual void restart(const unsigned int t, const T current_time); // restart from #t steps; reset step counter to t

    // output
    virtual void writefile(const std::string & filename="", const T * data=NULL);
    
};

#endif /* defined(__SteinbachPhaseFieldFD__simulator__) */
