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
#include "../OpenCL/kernel.hpp"
#include "../OpenCL/function.hpp"

BOOST_AUTO_TEST_SUITE(single_device)

//! Test the load of a simple kernel.
BOOST_AUTO_TEST_CASE(single_device_test)
{
	using namespace opencl;

	try
	{
		device dev = get_first_device(CL_DEVICE_TYPE_ALL);
		context ctx(dev);
		program prg(ctx, load_source_code("./Kernels/null_kernel.cl"));
		BOOST_CHECK(true);
	}
	catch(...)
	{
		BOOST_CHECK(false);
	}
}

//! Test the load of a kernel which should fail to compile.
BOOST_AUTO_TEST_CASE(single_device_test_syntax_error)
{
    using namespace opencl;

    try
    {
        device dev = get_first_device(CL_DEVICE_TYPE_ALL);
        context ctx(dev);
        program prg(ctx, load_source_code("./Kernels/syntax_error.cl"));
        BOOST_CHECK(false);
    }
    catch(bad_program_build& /*e*/)
    {
        //std::cout << e.what() << std::endl;
        BOOST_CHECK(true);
    }
}

//! Test that kernels are not allowed to return a value.
BOOST_AUTO_TEST_CASE(single_device_test_return)
{
    using namespace opencl;

    try
    {
        device dev = get_first_device(CL_DEVICE_TYPE_ALL);
        context ctx(dev);
        program prg(ctx, load_source_code("./Kernels/return_int.cl"));
        BOOST_CHECK(false);
    }
    catch(bad_program_build& /*e*/)
    {
        //std::cout << e.what() << std::endl;
        BOOST_CHECK(true);
    }
}

//! Test a kernel function
BOOST_AUTO_TEST_CASE(single_device_test_kernel_function)
{
    using namespace opencl;

    try
    {
        device dev = get_first_device(CL_DEVICE_TYPE_ALL);
        context ctx(dev);
        program prg(ctx, load_source_code("./Kernels/set_int.cl"));
        kernel knl(prg, "set_int");
        cl_command_queue_properties qProp = 0;
        command_queue q(ctx, dev, qProp);

        std::size_t globalThreads[2] = {2,2};
        std::size_t localThreads[2] = {2,2};

        function<void(int*)> fn(knl, ctx, q, 2, globalThreads, localThreads);
        int i=0;
        int* pI = &i;
        fn(pI);

        BOOST_CHECK(i==42);
    }
    catch(...)
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_SUITE_END()

#endif //OPENCL_BASIC_SINGLE_DEVICE_TEST_HPP_INCLUDE
