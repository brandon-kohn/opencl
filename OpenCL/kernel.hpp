//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_KERNEL_HPP_INCLUDE)
#define OPENCL_KERNEL_HPP_INCLUDE

#include "./program.hpp"
#include "./memory.hpp"

namespace opencl
{
	struct kernel_not_found : std::bad_exception {};
	class kernel
	{
	public:

		kernel(const program& prg, const std::string& kernelName)	
		{
			cl_int status;
			knl = clCreateKernel(prg, kernelName.c_str(), &status);
			if( status != CL_SUCCESS )
				throw kernel_not_found();
		}

		kernel( const kernel& rhs )
		{
			knl = rhs.knl;
			clRetainKernel(knl);
		}

		~kernel()
		{
			clReleaseKernel(knl);
		}
		
		operator cl_kernel const&() const
		{
			return knl;
		}

		operator cl_kernel&()
		{
			return knl;
		}
		
	private:
				
		cl_kernel knl;

	};
}

#endif//OPENCL_KERNEL_HPP_INCLUDE
