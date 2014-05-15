//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_PLATFORM_HPP_INCLUDE)
#define OPENCL_PLATFORM_HPP_INCLUDE

#include "./detail/common.hpp"

#include <boost/shared_array.hpp>
#include <boost/optional.hpp>

#include <exception>
#include <string>

namespace opencl
{
	struct bad_platform_exception : std::bad_exception {};
	struct device_not_found : std::bad_exception {};

	inline cl_uint number_platforms()
	{
		cl_uint numPlatforms;
		cl_int status = clGetPlatformIDs(0, 0, &numPlatforms);
		if(status != CL_SUCCESS)
			throw bad_platform_exception();
		return numPlatforms;
	}

	class platform
	{
	public:

		platform( const std::string& vendor )
			: vendor(vendor)
		{}

		platform( cl_platform_id id )
			: id(id)
		{
			std::size_t size=0;
			cl_int status = clGetPlatformInfo(id, CL_PLATFORM_VENDOR, 0, 0, &size);
			if(status != CL_SUCCESS)
				throw bad_platform_exception();

			boost::shared_array<char> buf(new char[size]);
			status = clGetPlatformInfo(id, CL_PLATFORM_VENDOR, size, buf.get(), &size);
			if(status != CL_SUCCESS)
				throw bad_platform_exception();
			vendor.assign(buf.get(), buf.get() + size);
		}

		operator cl_platform_id&()
		{
			if( !id )
				instantiate();
			return *id;
		}

		operator cl_platform_id const&() const
		{
			if( !id )
				instantiate();
			return *id;
		}
		
		cl_uint get_device_count(cl_device_type dType) const
		{
			if( !id )
				instantiate();

			// Get device count
			cl_uint deviceCount = 0;
			cl_int status = clGetDeviceIDs(*id, dType, 0, 0, &deviceCount);
			if(status != CL_SUCCESS)
		    {
                switch(status)
                {
                case CL_DEVICE_NOT_FOUND: //! if no OpenCL devices that matched device_type were found.
                    return 0;
                case CL_INVALID_PLATFORM ://! if platform is not a valid platform.
                case CL_INVALID_DEVICE_TYPE: //! if device_type is not a valid value.
                case CL_INVALID_VALUE: //! if num_entries is equal to zero and devices is not NULL or if both num_devices and devices are NULL.                
                case CL_OUT_OF_RESOURCES: //! if there is a failure to allocate resources required by the OpenCL implementation on the device.
                case CL_OUT_OF_HOST_MEMORY: //! if there is a failure to allocate resources required by the OpenCL implementation on the host.
                    throw bad_platform_exception();
                };                
            }

			return deviceCount;
		}

		std::vector<cl_device_id> get_device_ids(cl_device_type dType)
		{
			if( !id )
				instantiate();

			cl_uint nDevices = get_device_count(dType);
			std::vector<cl_device_id> ids( nDevices, 0 );
			if( !ids.empty() )
			{
				cl_int status = clGetDeviceIDs(*id, dType, 1, &ids[0], 0);
				if(status != CL_SUCCESS)
					throw bad_platform_exception();		
			}

			return ids;
		}

		cl_device_id get_device_id(cl_device_type dType, const std::string& name, int dIndex = -1)
		{
			if( !id )
				instantiate();

			cl_uint nDevices = get_device_count(dType);

			if( -1 != dIndex && static_cast<cl_uint>(dIndex) >= nDevices || nDevices == 0 )
				throw std::out_of_range("attempt to access device index which is out of the available range");

			boost::shared_array<cl_device_id> ids( new cl_device_id[nDevices] );
			cl_int status = clGetDeviceIDs(*id, dType, nDevices, ids.get(), 0);
			if(status != CL_SUCCESS)
				throw bad_platform_exception();

			if( dIndex != -1 )
				return ids[dIndex];
			else
			{
				//!Find the first type device with given name.
				char buf[1024];
				for(unsigned int i = 0; i < nDevices; ++i )
				{
					status = clGetDeviceInfo(ids[i], CL_DEVICE_NAME, sizeof(buf), &buf, 0);
					if( status != CL_SUCCESS )
						throw bad_platform_exception();

					if( name == buf )
						return ids[i];
				}
			}

			throw device_not_found();
		}

		cl_device_id get_device_id(cl_device_type dType, int dIndex = -1)
		{
			if( !id )
				instantiate();

			cl_uint nDevices = get_device_count(dType);

			cl_uint index = dIndex == -1 ? 0 : dIndex;
			if( index >= nDevices || nDevices == 0 )
				throw std::out_of_range("attempt to access device index which is out of the available range");

			boost::shared_array<cl_device_id> ids( new cl_device_id[nDevices] );
			cl_int status = clGetDeviceIDs(*id, dType, nDevices, ids.get(), 0);
			if(status != CL_SUCCESS)
				throw bad_platform_exception();

			return ids[index];
		}
				
	private:

		void instantiate() const
		{
			//!Find the platform with the given name:
			// Get selected platform
			cl_uint numPlatforms = number_platforms();
			boost::shared_array<cl_platform_id> platforms(new cl_platform_id[numPlatforms]);
			cl_int status = clGetPlatformIDs(numPlatforms, platforms.get(), 0);
			if(status != CL_SUCCESS)
				throw bad_platform_exception();

			for( cl_uint i = 0; i < numPlatforms; ++i )
			{
				char buf[256];
				status = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(buf), buf, 0);

				if( vendor == buf )
				{
					//! Initialize from the found platform.
					id = platforms[i];
					return;
				}
			}

			throw bad_platform_exception();
		}

		mutable boost::optional<cl_platform_id> id;
		std::string                             vendor;
	};					

	inline std::vector<platform> get_platforms()
	{
		cl_uint numPlatforms = number_platforms();
		boost::shared_array<cl_platform_id> platforms(new cl_platform_id[numPlatforms]);
		cl_int status = clGetPlatformIDs(numPlatforms, platforms.get(), 0);
		if(status != CL_SUCCESS)
			throw bad_platform_exception();

		std::vector<platform> plats(platforms.get(), platforms.get()+numPlatforms);
		return plats;
	}
}//namespace opencl;

#endif//OPENCL_PLATFORM_HPP_INCLUDE
