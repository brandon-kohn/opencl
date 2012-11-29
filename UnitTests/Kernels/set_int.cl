
//! Shouln't compile as needs to be void.
__kernel void set_int(__global int* i)
{
	i[0] = 42;
}