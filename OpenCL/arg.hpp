//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_ARG_HPP_INCLUDE)
#define OPENCL_ARG_HPP_INCLUDE

#include "./memory.hpp"

#include <boost/call_traits.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/array.hpp>
#include <boost/utility/enable_if.hpp>

#include <vector>

namespace opencl
{
    template <typename T, typename EnableIf=void>
    struct void_pointer_mapper
    {
        typedef void* result_type;

        //! Default memory mapper assumes POD semantics.
        static std::size_t size(T const&)
        {
            return sizeof(T);
        }

        //! convert the type to a void*
        static void* apply(T const& a)
        {
            return (void*)&a;
        }
    };

    template <typename T>
    struct void_pointer_mapper<T*, void>
    {
        typedef void* result_type;

        //! Default memory mapper assumes POD semantics.
        static std::size_t size(T* const&)
        {
            return sizeof(T);
        }

        //! convert the type to a void*
        static void* apply(T*& a)
        {
            return (void*)a;
        }
    };

    template <typename T, int N>
    struct void_pointer_mapper< boost::array<T,N> >
    {
        typedef void* result_type;

        //! Default memory mapper assumes POD semantics.
        static std::size_t size(boost::array<T,N> const&)
        {
            return sizeof(T)*N;
        }

        //! convert the type to a void*
        static void* apply(boost::array<T,N>& a)
        {
            return (void*)&a[0];
        }
    };

    //! \brief specify types which must map into a buffer.
    //! The default case is treated like a POD and will be copied bitwise onto the device.
    template <typename T, typename EnableIf=void>
    struct is_buffer_type : boost::false_type{};

    //! specify a few types which must map into a buffer for transfer onto the device.
    template <typename T>
    struct is_buffer_type< boost::range_iterator<T> >
        : boost::true_type{};

    template <typename T, typename Alloc>
    struct is_buffer_type< std::vector<T, Alloc> >
        : boost::true_type{};

    template <typename T, int Size>
    struct is_buffer_type< boost::array<T, Size> >
        : boost::true_type{};

    template <typename T>
    struct is_buffer_type< T, typename boost::enable_if< boost::is_pointer<T> >::type >
        : boost::true_type{};

    template <typename T, typename EnableIf=void>
    struct arg
    {
        arg(int argN, typename boost::call_traits<T>::reference a, const cl_context&, const cl_command_queue&, const cl_kernel& knl)
            : a(a)
            , knl(knl)
            , argN(argN)
        {
            cl_int status = clSetKernelArg(
                knl, 
                argN, 
                void_pointer_mapper<T>::size(a), 
                void_pointer_mapper<T>::apply(a));
            assert(status == CL_SUCCESS);
        }

        void read_data(){}

    private:

        int              argN;
        cl_kernel        knl;
        T&               a;

    };

    template <typename T>
    struct arg<T, typename boost::enable_if< is_buffer_type<T> >::type>
    {
        arg(int argN, typename boost::call_traits<T>::reference a, const cl_context& ctx, const cl_command_queue& cmdQ, const cl_kernel& knl)
            : a(a)
            , cmdQ(cmdQ)
            , knl(knl)
            , argN(argN)
            , mem(ctx, (boost::is_const<T>::value ? CL_MEM_READ_ONLY : CL_MEM_READ_WRITE), void_pointer_mapper<T>::size(a), void_pointer_mapper<T>::apply(a))
        {
            enqueue_map_buffer(CL_TRUE, CL_MAP_WRITE);
            wait_for_map_event();
            memcpy(memPtr, void_pointer_mapper<T>::apply(a), mem.get_size());
            enqueue_unmap_buffer();
            wait_for_unmap_event();
            cl_int status = clSetKernelArg(
                knl, 
                argN, 
                sizeof(cl_mem), 
                (void*)&mem);
            assert(status == CL_SUCCESS);
        }

        void read_data()
        {
            enqueue_map_buffer(CL_TRUE, CL_MAP_READ);
            wait_for_map_event();
            memcpy(void_pointer_mapper<T>::apply(a), memPtr, mem.get_size());
            enqueue_unmap_buffer();
            wait_for_unmap_event();
        }
        
        private:

        void enqueue_map_buffer(cl_bool blocking, cl_map_flags flags)
        {
            cl_int status;
            memPtr = clEnqueueMapBuffer(
                  cmdQ
                , mem
                , blocking
                , flags
                , 0
                , mem.get_size()
                , 0
                , 0
                , &mapEvt
                , &status);
            if( status != CL_SUCCESS )
                throw std::bad_exception();
        }

        void enqueue_unmap_buffer()
        {
            cl_int status = clEnqueueUnmapMemObject(
                  cmdQ
                , mem
                , memPtr
                , 0
                , 0
                , &unmapEvt);
            if( status != CL_SUCCESS )
                throw std::bad_exception();
        }

        void wait_for_map_event()
        {
            cl_int status = CL_SUCCESS;
            cl_int eventStatus = CL_QUEUED;
            while(eventStatus != CL_COMPLETE)
            {
                status = clGetEventInfo(
                    mapEvt, 
                    CL_EVENT_COMMAND_EXECUTION_STATUS, 
                    sizeof(cl_int),
                    &eventStatus,
                    0);
                assert(status == CL_SUCCESS);
            }

            status = clReleaseEvent(mapEvt);
            assert(status == CL_SUCCESS);            
        }

        void wait_for_unmap_event()
        {
            cl_int status = CL_SUCCESS;
            cl_int eventStatus = CL_QUEUED;
            while(eventStatus != CL_COMPLETE)
            {
                status = clGetEventInfo(
                    unmapEvt, 
                    CL_EVENT_COMMAND_EXECUTION_STATUS, 
                    sizeof(cl_int),
                    &eventStatus,
                    0);
                assert(status == CL_SUCCESS);
            }

            status = clReleaseEvent(unmapEvt);
            assert(status == CL_SUCCESS);
        }
    
        int              argN;
        cl_command_queue cmdQ;
        cl_kernel        knl;
        T&               a;
        void*            memPtr;
        memory           mem;
        cl_event         mapEvt;
        cl_event         unmapEvt;
    };    
}

#endif//OPENCL_ARG_HPP_INCLUDE
