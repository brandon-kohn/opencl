//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_PROGRAM_HPP_INCLUDE)
#define OPENCL_PROGRAM_HPP_INCLUDE

#include "./context.hpp"
#include "./binary_code.hpp"
#include "./source_code.hpp"

namespace opencl
{
	struct bad_program_source : std::bad_exception {};
	struct bad_program_build : std::bad_exception
	{
		bad_program_build(const std::string& buildlog)
			: std::bad_exception(buildlog.c_str())
		{}
	};

	struct program_source
	{
		program_source(const binary_code& code)
			: isbytecode(true)
			, code(code)
		{}

		program_source(const source_code& code)
			: isbytecode(false)
			, code(code)
		{}

		bool                         isbytecode;
		std::string                  code;
	};
	
	class program
	{
	public:
		
		program(const context& ctx, const program_source& props)
		{
			build_program(ctx, props);
		}

		operator cl_program const&() const
		{
			return prog;
		}

		operator cl_program&()
		{
			return prog;
		}

		template <typename Device>
		void compile(const Device& dev, const std::string& cflags)
		{
			cl_device_id id = (cl_device_id)dev;
			cl_int status = clBuildProgram(prog, 1, &id, cflags.c_str(), 0, 0);
			if(status != CL_SUCCESS)
			{
				if(status == CL_BUILD_PROGRAM_FAILURE)
				{
					std::vector<char> blog(1024,'\0');
					std::size_t blogsize = 0;
					status = clGetProgramBuildInfo(prog, id, CL_PROGRAM_BUILD_LOG, blogsize, &blog[0], &blogsize);
					if(status != CL_SUCCESS)
						throw bad_program_build("unable to access compile log");
					
					if( blogsize > blog.size() )
						blog.resize(blogsize, '\0');
					
					status = clGetProgramBuildInfo(prog, id, CL_PROGRAM_BUILD_LOG, blogsize, &blog[0], 0);
					if(status != CL_SUCCESS)
						throw bad_program_build("unable to access compile log");
					
					throw bad_program_build(&blog[0]);
				}
				
				throw bad_program_build("unable to access compile log");
			}
		}

	private:

		void build_program(const context& ctx, const program_source& props)
		{
			cl_int status = CL_SUCCESS;
						
			if(props.isbytecode)
			{	
				const std::vector<device>& ctxDevices = ctx.get_devices();
				assert(!ctxDevices.empty());
				if( ctxDevices.empty() )
					throw bad_program_source();
				std::vector<const cl_device_id> tDevIDs(boost::begin(ctxDevices), boost::end(ctxDevices));			
				const unsigned char* binary = (const unsigned char*)props.code.c_str();
				const std::size_t size = props.code.size();
				prog = clCreateProgramWithBinary(ctx, tDevIDs.size(), &tDevIDs[0], &size, &binary, 0, &status);
				if( status != CL_SUCCESS )
					throw bad_program_source();
			}
			else
			{
				const char* source = props.code.c_str();
				std::size_t sourceSize[] = {props.code.size()};
				prog = clCreateProgramWithSource(ctx, 1, &source, sourceSize, &status);
				if( status != CL_SUCCESS )
					throw bad_program_source();
			}			
		}

		cl_program prog;

	};
}

#endif//OPENCL_PROGRAM_HPP_INCLUDE
