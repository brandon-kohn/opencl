//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_CONTEXT_HPP_INCLUDE)
#define OPENCL_CONTEXT_HPP_INCLUDE

#include "./device.hpp"
#include <boost/range.hpp>

namespace opencl
{
	struct bad_device_context : std::bad_exception {};
	class context
	{
	public:

		context(device dev)	
			: devices(1, dev)
		{}

		template <typename DeviceRange>
		context(DeviceRange dIDs)	
			: devices(boost::begin(dIDs), boost::end(dIDs))
		{}

		void instantiate() const
		{
			cl_int status;
			cl_uint nDevices = devices.size();
			std::vector<cl_device_id> ids(devices.begin(), devices.end());
			if( !ids.empty() )
			{
				ctx = clCreateContext(0, nDevices, &ids[0], 0, 0, &status);
				if( status != CL_SUCCESS )
					throw bad_device_context();
			}
		}
		
		operator cl_context const&() const
		{
			if( !ctx )
				instantiate();
			return *ctx;
		}

		operator cl_context&()
		{
			if( !ctx )
				instantiate();
			return *ctx;
		}

		const std::vector<device>& get_devices() const { return devices; }

	private:
		
		std::vector<device>                 devices;
		mutable boost::optional<cl_context> ctx;

	};
}

#endif//OPENCL_CONTEXT_HPP_INCLUDE
