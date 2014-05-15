//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_PLATFORM_TESTS_HPP_INCLUDE)
#define OPENCL_PLATFORM_TESTS_HPP_INCLUDE

#include <boost/test/included/unit_test.hpp>

#include "../OpenCL/platform.hpp"

BOOST_AUTO_TEST_SUITE(platform_tests)

BOOST_AUTO_TEST_CASE(platform_enumerate_test)
{
	using namespace opencl;

	try
	{
		std::vector<platform> platforms = get_platforms();
		BOOST_CHECK(!platforms.empty());
	}
	catch(...)
	{
		BOOST_CHECK(false);
	}
}

BOOST_AUTO_TEST_SUITE_END()

#endif //OPENCL_PLATFORM_TESTS_HPP_INCLUDE
