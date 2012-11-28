//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_BASIC_SINGLE_DEVICE_TEST_HPP_INCLUDE)
#define OPENCL_BASIC_SINGLE_DEVICE_TEST_HPP_INCLUDE

#include <boost/test/unit_test.hpp>

#include "../OpenCL/device.hpp"
#include "../OpenCL/context.hpp"
#include "../OpenCL/program.hpp"

BOOST_AUTO_TEST_SUITE(single_device)

BOOST_AUTO_TEST_CASE(single_device_test)
{
	using namespace opencl;

	try
	{
		platform plt("NVIDIA Corporation");
		device nvidiaGPU(plt, plt.get_device_id(CL_DEVICE_TYPE_GPU, 0), 0);
		context nvidiaGPUCtx(nvidiaGPU);
		program prg(nvidiaGPUCtx, load_source_code("./Kernels/null_kernel.cl"));
		BOOST_CHECK(true);
	}
	catch(...)
	{
		BOOST_CHECK(false);
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif //OPENCL_BASIC_SINGLE_DEVICE_TEST_HPP_INCLUDE
