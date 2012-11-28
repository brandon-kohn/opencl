//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_COMMAND_QUEUE_HPP_INCLUDE)
#define OPENCL_COMMAND_QUEUE_HPP_INCLUDE

#include "detail/common.hpp"

namespace opencl
{
	class command_queue
	{
	public:
		
		command_queue(cl_context ctx, cl_device_id dev, cl_command_queue_properties props)	
		{
			cl_int status;
			q = clCreateCommandQueue(ctx, dev, props, &status);
			if( status != CL_SUCCESS )
				throw bad_device_context();
		}			
		
		operator cl_command_queue const&() const
		{
			return ctx;
		}

		operator cl_command_queue&()
		{
			return ctx;
		}

	private:
		
		cl_command_queue q;

	};
}

#endif//OPENCL_COMMAND_QUEUE_HPP_INCLUDE
