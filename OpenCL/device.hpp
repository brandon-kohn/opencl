//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_DEVICE_HPP_INCLUDE)
#define OPENCL_DEVICE_HPP_INCLUDE

#include "./platform.hpp"

namespace opencl
{
	class device
	{
	public:

		device(platform plat, cl_device_type type, const std::string& name, int index = -1)
			: plat(plat)
			, type(type)
			, name(name)
			, index(index)
		{}

		device(platform plat, cl_device_id id, int index = -1)
			: plat(plat)
			, id(id)			
			, index(index)
		{
			type = get_info<cl_device_type>(CL_DEVICE_TYPE);
			name = get_info<std::string>(CL_DEVICE_NAME);
		}

		operator cl_device_id&()
		{
			if( !id )
				instantiate();
			return *id;
		}

		operator cl_device_id const&() const
		{
			if( !id )
				instantiate();
			return *id;
		}
						
		cl_device_type              get_type() const { return type; }
		cl_device_id                get_id() const { if( !id ) instantiate(); return *id; }
		std::string                 get_name() const { return name; }
		std::string                 get_vendor() const { return get_info<std::string>(CL_DEVICE_VENDOR); }
		std::string                 get_version() const { return get_info<std::string>(CL_DEVICE_VERSION); }
		std::string                 get_driver_version() const { return get_info<std::string>(CL_DRIVER_VERSION); }
		cl_uint                     get_device_address_bits() const { return get_info<cl_uint>(CL_DEVICE_ADDRESS_BITS); } 
		
		cl_bool                     get_error_correction_support() const { return get_info<cl_bool>(CL_DEVICE_ERROR_CORRECTION_SUPPORT); }		
		
		cl_ulong                    get_max_mem_alloc_size() const { return get_info<cl_ulong>(CL_DEVICE_MAX_MEM_ALLOC_SIZE); } 
		cl_ulong                    get_global_mem_size() const { return get_info<cl_ulong>(CL_DEVICE_GLOBAL_MEM_SIZE); } 
		
		cl_ulong                    get_local_mem_size() const { return get_info<cl_ulong>(CL_DEVICE_LOCAL_MEM_SIZE); } 
		cl_device_local_mem_type    get_local_mem_type() const { return get_info<cl_device_local_mem_type>(CL_DEVICE_LOCAL_MEM_TYPE); }		

		cl_ulong                    get_max_constanct_buffer_size() const { return get_info<cl_ulong>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE); } 
		cl_uint                     get_max_compute_units() const { return get_info<cl_uint>(CL_DEVICE_MAX_COMPUTE_UNITS); } 
		cl_uint                     get_max_clock_freq() const { return get_info<cl_uint>(CL_DEVICE_MAX_CLOCK_FREQUENCY); } 

		std::size_t                 get_max_work_item_dims() const { return get_info<std::size_t>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS); } 
		std::size_t                 get_max_work_group_size() const { return get_info<std::size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE); } 
		std::vector<std::size_t>    get_max_work_item_sizes() const 
		{ 
			std::vector<std::size_t> buf(3);
			clGetDeviceInfo(*id, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(std::size_t)*3, &buf[0], 0);
			return buf;
		} 
		
		cl_device_fp_config         get_fp_config() const { return get_info<cl_device_fp_config>(CL_DEVICE_SINGLE_FP_CONFIG); }
		cl_command_queue_properties get_command_queue_properties() const { return get_info<cl_command_queue_properties>(CL_DEVICE_QUEUE_PROPERTIES); }

		cl_bool                     get_image_support() const { return get_info<cl_bool>(CL_DEVICE_IMAGE_SUPPORT); }
		cl_uint                     get_max_read_image_args() const { return get_info<cl_uint>(CL_DEVICE_MAX_READ_IMAGE_ARGS); }
		cl_uint                     get_max_write_image_args() const { return get_info<cl_uint>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS); }
		std::size_t                 get_image_2d_max_width() const { return get_info<std::size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH); }
		std::size_t                 get_image_2d_max_height() const { return get_info<std::size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT); }
		std::size_t                 get_image_3d_max_width() const { return get_info<std::size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH); }
		std::size_t                 get_image_3d_max_height() const { return get_info<std::size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT); }
		std::size_t                 get_image_3d_max_depth() const { return get_info<std::size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH); }

		cl_uint                     get_preferred_vector_width_char() const { return get_info<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR); }
		cl_uint                     get_preferred_vector_width_short() const { return get_info<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT); }
		cl_uint                     get_preferred_vector_width_int() const { return get_info<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT); }
		cl_uint                     get_preferred_vector_width_long() const { return get_info<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG); }
		cl_uint                     get_preferred_vector_width_float() const { return get_info<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT); }
		cl_uint                     get_preferred_vector_width_double() const { return get_info<cl_uint>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE); }
		
	private:

		template <typename T>
		T get_info(cl_device_info info) const 
		{
			T buf;
			clGetDeviceInfo(*id, info, sizeof(buf), &buf, 0);
			return buf;
		}

		template <>
		std::string get_info(cl_device_info info) const 
		{
			char buf[1024];
			clGetDeviceInfo(*id, info, sizeof(buf), &buf, 0);
			return buf;
		}

		void instantiate() const
		{
			cl_uint nDevices = plat.get_device_count(type);

			if( -1 != index && static_cast<cl_uint>(index) >= nDevices || nDevices == 0 )
				throw std::out_of_range("attempt to access device index which is out of the available range");

			boost::shared_array<cl_device_id> ids( new cl_device_id[nDevices] );
			cl_int status = clGetDeviceIDs(plat, type, nDevices, ids.get(), 0);
			if(status != CL_SUCCESS)
				throw bad_platform_exception();

			if( index != -1 )
			{
				id = ids[index];
				return;
			}
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
					{
						id = ids[i];
						return;
					}
				}
			}	

			throw device_not_found();
		}
				
		platform                              plat;
		cl_device_type                        type;
		std::string                           name;
		int                                   index;
		mutable boost::optional<cl_device_id> id;

	};

}//namespace opencl;

#endif//OPENCL_DEVICE_HPP_INCLUDE
