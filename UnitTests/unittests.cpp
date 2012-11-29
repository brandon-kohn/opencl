#define BOOST_TEST_ALTERNATIVE_INIT_API

#include <boost/test/unit_test_suite.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test.hpp>

#include "./basic_single_device_test.hpp"
#include "./Tests/platform_tests.hpp"

#include <fstream>

void standard_exception_translator( const std::exception& e )
{
    BOOST_TEST_MESSAGE( e.what() );
}

//! Initialize the test suites.
bool init_function()
{
	boost::unit_test::unit_test_log.set_threshold_level( boost::unit_test::log_messages );
    boost::unit_test::unit_test_monitor.register_exception_translator<std::exception>( &standard_exception_translator );
    boost::unit_test::framework::master_test_suite().p_name.value = "OpenCL Unit Testing";
    
    // with explicit registration we could specify a test case timeout
    //boost::unit_test::framework::master_test_suite().add( BOOST_TEST_CASE( &infinite_loop ), 0, /* timeout */ 2 );
	
    return true; 
}

int main(int argc, char** argv)
{
    return ::boost::unit_test::unit_test_main( (boost::unit_test::init_unit_test_func)&init_function, argc, argv );
}
