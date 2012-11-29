//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_MEMORY_HPP_INCLUDE)
#define OPENCL_MEMORY_HPP_INCLUDE

#include "./context.hpp"

namespace opencl
{
	struct bad_mem_flags : std::bad_exception {};
	struct invalid_buffer_size : std::bad_exception {};
	struct invalid_host_pointer : std::bad_exception {};
	struct device_bad_alloc : std::bad_alloc {};
	struct invalid_mem_object : std::bad_exception {};

	class memory
	{
	public:

		memory(cl_context ctx, cl_mem_flags flags, cl_uint size, void* buf = 0)	
            : size(size)
		{
			cl_int status;
			mem = clCreateBuffer(ctx, flags, size, buf, &status);
			if(status != CL_SUCCESS)
			{
				switch(status)
				{
				case CL_INVALID_CONTEXT:// if context is not a valid context..
					{
						throw bad_device_context();
					}
				case CL_INVALID_VALUE:// if values specified in flags are not valid.
					{
						throw bad_mem_flags();
					}
				case CL_INVALID_BUFFER_SIZE://if size is 0 or is greater than CL_DEVICE_MAX_MEM_ALLOC_SIZE value specified in table of OpenCL Device Queries for clGetDeviceInfo for all devices in context.
					{
						throw invalid_buffer_size();
					}
				case CL_INVALID_HOST_PTR://if host_ptr is NULL and CL_MEM_USE_HOST_PTR or CL_MEM_COPY_HOST_PTR are set in flags or if host_ptr is not NULL but CL_MEM_COPY_HOST_PTR or CL_MEM_USE_HOST_PTR are not set in flags.
					{
						throw invalid_host_pointer();
					}
				case CL_MEM_OBJECT_ALLOCATION_FAILURE://if there is a failure to allocate memory for buffer object.
					{
						throw device_bad_alloc();
					}
				case CL_OUT_OF_HOST_MEMORY: //if there is a failure to allocate resources required by the OpenCL implementation on the host.
					{
						throw std::bad_alloc();
					}	
				}
			}
		}
		
		operator cl_mem const&() const
		{
			return mem;
		}

		operator cl_mem &()
		{
			return mem;
		}

        std::size_t get_size() const { return size; }

	private:
	
		cl_mem      mem;
        std::size_t size;

	};
}

#endif//OPENCL_MEMORY_HPP_INCLUDE
