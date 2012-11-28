//
//  Copyright Brandon Kohn 2012
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
#if !defined(OPENCL_BINARY_CODE_HPP_INCLUDE)
#define OPENCL_BINARY_CODE_HPP_INCLUDE

#include "detail/common.hpp"

#include <fstream>

namespace opencl
{
	class binary_code
	{
	public:

		binary_code(const std::string& code)	
			: code(code)
		{}

		operator std::string& ()
		{
			return code;
		}

		operator std::string const& () const
		{
			return code;
		}
		
	private:
		
		cl_device_id dev;
		std::string code;

	};

	inline binary_code load_binary_code(const std::string& path)
	{
		std::ifstream ifs(path.c_str());
		if( !ifs.good() )
			throw std::invalid_argument(path.c_str());

		std::string code;
		ifs.seekg(0, std::ios::end);
		code.reserve(static_cast<std::size_t>(ifs.tellg()));
		ifs.seekg(0, std::ios::beg);
		code.assign(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
		return binary_code(code);
	}
}

#endif//OPENCL_BINARY_CODE_HPP_INCLUDE
