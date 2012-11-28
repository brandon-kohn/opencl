//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_MEMORY_HPP_INCLUDE)
#define OPENCL_MEMORY_HPP_INCLUDE

#include "detail/common.hpp"

namespace opencl
{
	class memory
	{
	public:
		
		operator cl_mem const&() const
		{
			return mem;
		}

		operator cl_mem &()
		{
			return mem;
		}

	private:
		
		friend class context;

		memory(cl_context ctx, cl_mem_flags flags, cl_uint size, void* buf = 0)	
		{
			cl_int status;
			mem = clCreateBuffer( ctx, flags, size, buf, &status);
			if(status != CL_SUCCESS)
				throw bad_alloc();
		}

		cl_mem mem;

	};
}

#endif//OPENCL_MEMORY_HPP_INCLUDE
