#pragma OPENCL EXTENSION cl_khr_fp64: enable

//! Test an out value.
__kernel void dot_product(__global const double* a, __global const double* b, __global double* c, uint size)
{
	int id = get_global_id(0);
	if( id < size )
	{
		int offset = id << 2;
		c[id] = a[offset] * b[offset] + a[offset + 1] * b[offset + 1] + a[offset + 2] * b[offset + 2] + a[offset + 3] * b[offset + 3];
	}
}
