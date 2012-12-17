#pragma OPENCL EXTENSION cl_khr_fp64: enable

//! dot product with coalescence.
__kernel void dot_product(__global const double* a, __global const double* b, __global double* c, uint size)
{
   int i = get_global_id(0);
   int nWorkItems = get_global_size(0);
   
   float val = 0.0f;
   int ind = i;
   for (int j = 0; j < size; ++j)
   {
      val += a[ind]*b[ind];
      ind += nWorkItems;
   }
   val += c[i];

   //Reduction part
   int p = get_local_id(0);
   int q = get_group_id(0);
   int maxp = get_local_size(0);

   __local float values[256];
   values[p] = val;
   barrier(CLK_LOCAL_MEM_FENCE);

   maxp = maxp >> 1;
   while(maxp > 0)
   {
      if (p < maxp)
         values[p] += values[p+maxp];
      
	  maxp = maxp >> 1;
      barrier(CLK_LOCAL_MEM_FENCE);
   }

   if (p == 0)
       c[q] = values[0];
}
