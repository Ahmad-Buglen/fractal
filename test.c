
# include "libft/libft.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

# include "mlx.h"

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else
#  include <CL/cl.h>
# endif

////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//
#define DATA_SIZE (1024)

////////////////////////////////////////////////////////////////////////////////

// Simple compute kernel which computes the square of an input array 
//
const char *KernelSource = "\n" \
"__kernel void square(                                                       \n" \
"   __global int *testBuf)                                           \n" \
"{                                                                      \n" \
"   //int i = get_global_id(0);                                           \n" \
"                                                        \n" \
"       *testBuf = get_global_id(0);                                \n" \
"}                                                                     \n" \
"\n";


int main()
{

	 int err;                            // error code returned from api calls
      
   // float data[DATA_SIZE];              // original data set given to device
   // float results[DATA_SIZE];           // results returned from device
   // unsigned int correct;               // number of correct results returned

    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation

    cl_device_id device_id;             // compute device id 
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    
   // cl_mem input;                       // device memory used for the input array
   // cl_mem output;                      // device memory used for the output array
    
    // Fill our data set with random float values
    //
   // int i = 0;
   // unsigned int count = DATA_SIZE;
   // for(i = 0; i < count; i++)
     //   data[i] = rand() / (float)RAND_MAX;
    cl_mem testBuf;
int test = 0;

    
    // Connect to a compute device
    //
    int gpu = 1;
    err = clGetDeviceIDs(NULL, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to create a device group!\n");
        return EXIT_FAILURE;
    }
  
    // Create a compute context 
    //
    context = clCreateContext(0, 1, &device_id, NULL, NULL, &err);
    if (!context)
    {
        printf("Error: Failed to create a compute context!\n");
        return EXIT_FAILURE;
    }

    // Create a command commands
    //
    commands = clCreateCommandQueue(context, device_id, 0, &err);
    if (!commands)
    {
        printf("Error: Failed to create a command commands!\n");
        return EXIT_FAILURE;
    }

    // Create the compute program from the source buffer
    //
    program = clCreateProgramWithSource(context, 1, (const char **) & KernelSource, NULL, &err);
    if (!program)
    {
        printf("Error: Failed to create compute program!\n");
        return EXIT_FAILURE;
    }

    // Build the program executable
    //
    err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
    if (err != CL_SUCCESS)
    {
        size_t len;
        char buffer[2048];

        printf("Error: Failed to build program executable!\n");
        clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
        printf("%s\n", buffer);
        exit(1);
    }

    // Create the compute kernel in the program we wish to run
    //
    kernel = clCreateKernel(program, "square", &err);
    if (!kernel || err != CL_SUCCESS)
    {
        printf("Error: Failed to create compute kernel!\n");
        exit(1);
    }

    // Create the input and output arrays in device memory for our calculation
    //
    testBuf = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int), NULL, NULL);
  //  input = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(float) * count, NULL, NULL);
 //   output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(float) * count, NULL, NULL);
    if (!testBuf)
    {
        printf("Error: Failed to allocate device memory!\n");
        exit(1);
    }    
    
    // Write our data set into the input array in device memory 
    //
    // err = clEnqueueWriteBuffer(commands, testBuf, CL_TRUE, 0, sizeof(int), test, 0, NULL, NULL);
    // if (err != CL_SUCCESS)
    // {
    //     printf("Error: Failed to write to source array!\n");
    //     exit(1);
    // }

    // Set the arguments to our compute kernel
    //
    err = 0;
    err  = clSetKernelArg(kernel, 0, sizeof(cl_mem), &testBuf);
  //  err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);
 //   err |= clSetKernelArg(kernel, 2, sizeof(unsigned int), &count);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to set kernel arguments! %d\n", err);
        exit(1);
    }

    // Get the maximum work group size for executing the kernel on the device
    //
    err = clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to retrieve kernel work group info! %d\n", err);
        exit(1);
    }

    // Execute the kernel over the entire range of our 1d input data set
    // using the maximum number of work group items for this device
    //
 //   printf("test = %d \n", test);
 	const size_t globalT = 256;
   global = 5;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &globalT, &local, 0, NULL, NULL);
    if (err)
    {
        printf("Error: Failed to execute kernel!\n");
        return EXIT_FAILURE;
    }

    // Wait for the command commands to get serviced before reading back results
    //
    clFinish(commands);

    // Read back the results from the device to verify the output
    //
          printf("CL_INVALID_COMMAND_QUEUE = %d \n", CL_INVALID_COMMAND_QUEUE);
        printf("CL_INVALID_CONTEXT = %d \n", CL_INVALID_CONTEXT);
          printf("CL_INVALID_MEM_OBJECT = %d \n", CL_INVALID_MEM_OBJECT);
            printf("CL_INVALID_VALUE = %d \n", CL_INVALID_VALUE);
              printf("CL_INVALID_EVENT_WAIT_LIST = %d \n", CL_INVALID_EVENT_WAIT_LIST);
    
              printf("CL_MEM_OBJECT_ALLOCATION_FAILURE = %d \n", CL_MEM_OBJECT_ALLOCATION_FAILURE);
              printf("CL_OUT_OF_HOST_MEMORY = %d \n", CL_OUT_OF_HOST_MEMORY);
              
    err = clEnqueueReadBuffer( commands, testBuf, CL_TRUE, 0, sizeof(int), &test, 0, NULL, NULL );  
    if (err != CL_SUCCESS)
    {
        printf("Error: Failed to read output array! %d\n", err);
        exit(1);
    }
    printf("test = %d \n", test);


    
    // Validate our results
    //

    
    // Print a brief summary detailing the results
    //
  //  printf("Computed '%d/%d' correct values!\n", correct, count);
    
    // Shutdown and cleanup
    //
    clReleaseMemObject(testBuf);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);

	//printf("%d \n ", );

	return (1);
}
