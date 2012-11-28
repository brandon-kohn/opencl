//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_KERNAL_HPP_INCLUDE)
#define OPENCL_KERNAL_HPP_INCLUDE

#include "detail/common.hpp"

namespace opencl
{
	struct bad_device_context : std::bad_exception {};
	class kernel
	{
	public:
		
		operator cl_kernel const&() const
		{
			return kern;
		}

		operator cl_kernel &()
		{
			return kern;
		}

	private:
		
		friend class device;

		kernel(cl_device_id did)	
		{
			cl_int status;
			ctx = clCreateContext(0, 1, &did, 0, 0, &status);
			if( status != CL_SUCCESS )
				throw bad_device_context();
		}

		cl_kernel kern;

	};
}

#endif//OPENCL_KERNAL_HPP_INCLUDE
