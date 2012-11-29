//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_FUNCTION_HPP_INCLUDE)
#define OPENCL_FUNCTION_HPP_INCLUDE

#include "./kernel.hpp"
#include "./arg.hpp"
#include "./command_queue.hpp"
#include "./context.hpp"

#include <boost/function_types/function_type.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/function_arity.hpp>

#include "./detail/function.hpp"

/*
            void* outMapPtrFloat = clEnqueueMapBuffer(
                cmdQ, 
                outputBufferFloat, 
                CL_FALSE, 
                CL_MAP_READ, 
                0, 
                width1 * height0 * sizeof(cl_float), 
                0, 
                NULL, 
                &outMapEvt, 
                &status);
            CHECK_OPENCL_ERROR(status, "clEnqueueMapBuffer failed. (outputBuffer)");
            status = clFlush(cmdQ);
            CHECK_OPENCL_ERROR(status, "clFlush failed.");

            wait_for_event_and_release(outMapEvt);
            
            memcpy(outputFloat, outMapPtrFloat, sizeof(cl_float) * width1  * height0);

            status = clEnqueueUnmapMemObject(
                cmdQ, 
                outputBufferFloat, 
                outMapPtrFloat, 
                0, 
                NULL, 
                &outUnmapEvt);
            CHECK_OPENCL_ERROR(status, "clEnqueueUnmapMemObject failed. (outputBuffer)");

            status = clFlush(cmdQ);
            CHECK_OPENCL_ERROR(status, "clFlush failed.");

            status = wait_for_event_and_release(&outUnmapEvt);
            CHECK_ERROR(status,0, "waitForEventAndRelease(outUnmapEvt) failed");

            return SDK_SUCCESS;
        }
	    */

#endif//OPENCL_KERNEL_HPP_INCLUDE
