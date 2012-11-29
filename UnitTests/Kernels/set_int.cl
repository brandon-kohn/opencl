
//! Test an out value.
__kernel void set_int(__global int* i)
{
	i[0] = 42;
}