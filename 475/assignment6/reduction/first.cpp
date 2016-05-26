// 1. Program header

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <omp.h>

#include "CL/cl.h"
#include "CL/cl_platform.h"

#ifndef POINTS
#define POINTS 8
#endif 

#ifndef MIN_SIZE
#define MIN_SIZE 1000
#endif 

#ifndef MAX_SIZE
#define MAX_SIZE 8000000
#endif

#ifndef LOCAL_SIZE
#define LOCAL_SIZE 32
#endif 





const char *			CL_FILE_NAME = { "first.cl" };
const float			TOL = 0.0001f;

void				Wait( cl_command_queue );
int				LookAtTheBits( float );
double				ArrayMult(int,int);
double				ArrayMultAdd(int, int);
double				ArrayMultRedux(int, int);

int
main( int argc, char *argv[ ] )
{
	double m;
	double ma;
	double mr;
	std::ofstream file;
	file.open("global.csv");
	for (int i = MIN_SIZE; i <= 8000000; i = i + (MAX_SIZE - MIN_SIZE) / (POINTS-1)) {
		m = ArrayMult(i, LOCAL_SIZE);
		ma = ArrayMultAdd(i, LOCAL_SIZE);
		mr = ArrayMultRedux(i, LOCAL_SIZE);
		file <<  i << "," << LOCAL_SIZE << "," << m << "," << ma << "," << mr <<std::endl;
	}
	file.close();
	//these are the values that worked on my hardware between 8 and 512
	int vals_that_work[] = { 8,10,16,20,25,32,40,50,64,80,100,128,160,200,256,320,400,512 };
	file.open("local.csv");
	for (int i = 0; i < 18; i++) {
		m = ArrayMult(MIN_SIZE*25, vals_that_work[i]);
		ma = ArrayMultAdd(MIN_SIZE * 25, vals_that_work[i]);
		if (m != -1 && ma != -1 && mr != -1) {
			file << MIN_SIZE * 25 << "," << vals_that_work[i] << "," << m << "," << ma << std::endl;
		}
	}
	file.close();
	return 0;
}


int
LookAtTheBits( float fp )
{
	int *ip = (int *)&fp;
	return *ip;
}


// wait until all queued tasks have taken place:

void
Wait( cl_command_queue queue )
{
      cl_event wait;
      cl_int      status;

      status = clEnqueueMarker( queue, &wait );
      if( status != CL_SUCCESS )
	      fprintf( stderr, "Wait: clEnqueueMarker failed\n" );

      status = clWaitForEvents( 1, &wait );
      if( status != CL_SUCCESS )
	      fprintf( stderr, "Wait: clWaitForEvents failed\n" );
}

double
ArrayMult(int arr_size, int loc_size)
{
	// see if we can even open the opencl kernel program
	// (no point going on if we can't):

	FILE *fp;
#ifdef WIN32
	errno_t err = fopen_s( &fp, CL_FILE_NAME, "rb" );
	if( err != 0 )
#else
	fp = fopen( CL_FILE_NAME, "rb" );
	if( fp == NULL )
#endif
	{
		fprintf( stderr, "Cannot open OpenCL source file '%s'\n", CL_FILE_NAME );
		return -1;
	}

	cl_int status;		// returned status from opencl calls
				// test against CL_SUCCESS

	// get the platform id:

	cl_platform_id platform;
	status = clGetPlatformIDs( 1, &platform, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clGetPlatformIDs failed (2)\n" );
	
	// get the device id:
	cl_device_id device;
	status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clGetDeviceIDs failed (2)\n" );
	// 2. allocate the host memory buffers:

	float *hA = new float[arr_size];
	float *hB = new float[arr_size];
	float *hC = new float[arr_size];

	// fill the host memory buffers:

	for( int i = 0; i < arr_size; i++ )
	{
		hA[i] = hB[i] = (float) sqrt(  (double)i  );
	}

	size_t dataSize = arr_size * sizeof(float);

	// 3. create an opencl context:

	cl_context context = clCreateContext( NULL, 1, &device, NULL, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateContext failed\n" );

	// 4. create an opencl command queue:

	cl_command_queue cmdQueue = clCreateCommandQueue( context, device, 0, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateCommandQueue failed\n" );

	// 5. allocate the device memory buffers:

	cl_mem dA = clCreateBuffer( context, CL_MEM_READ_ONLY,  dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (1)\n" );

	cl_mem dB = clCreateBuffer( context, CL_MEM_READ_ONLY,  dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (2)\n" );

	cl_mem dC = clCreateBuffer( context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (3) %d \n", arr_size);

	// 6. enqueue the 2 commands to write the data from the host buffers to the device buffers:

	status = clEnqueueWriteBuffer( cmdQueue, dA, CL_FALSE, 0, dataSize, hA, 0, NULL, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clEnqueueWriteBuffer failed (1)\n" );

	status = clEnqueueWriteBuffer( cmdQueue, dB, CL_FALSE, 0, dataSize, hB, 0, NULL, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clEnqueueWriteBuffer failed (2)\n" );

	Wait( cmdQueue );

	// 7. read the kernel code from a file:

	fseek( fp, 0, SEEK_END );
	size_t fileSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	char *clProgramText = new char[ fileSize+1 ];		// leave room for '\0'
	size_t n = fread( clProgramText, 1, fileSize, fp );
	clProgramText[fileSize] = '\0';
	fclose( fp );
	if( n != fileSize )
		fprintf( stderr, "Expected to read %d bytes read from '%s' -- actually read %d.\n", fileSize, CL_FILE_NAME, n );

	// create the text for the kernel program:

	char *strings[1];
	strings[0] = clProgramText;
	cl_program program = clCreateProgramWithSource( context, 1, (const char **)strings, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateProgramWithSource failed\n" );
	delete [ ] clProgramText;

	// 8. compile and link the kernel code:

	char *options = { "" };
	status = clBuildProgram( program, 1, &device, options, NULL, NULL );
	if( status != CL_SUCCESS )
	{
		size_t size;
		clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size );
		cl_char *log = new cl_char[ size ];
		clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, size, log, NULL );
		fprintf( stderr, "clBuildProgram failed:\n%s\n", log );
		delete [ ] log;
		clReleaseProgram(program);
		clReleaseCommandQueue(cmdQueue);
		clReleaseMemObject(dA);
		clReleaseMemObject(dB);
		clReleaseMemObject(dC);

		delete[] hA;
		delete[] hB;
		delete[] hC;
		return -1;
	}
	// 9. create the kernel object:

	cl_kernel kernel = clCreateKernel( program, "ArrayMult", &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateKernel failed\n" );

	// 10. setup the arguments to the kernel object:

	status = clSetKernelArg( kernel, 0, sizeof(cl_mem), &dA );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (1)\n" );

	status = clSetKernelArg( kernel, 1, sizeof(cl_mem), &dB );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (2)\n" );

	status = clSetKernelArg( kernel, 2, sizeof(cl_mem), &dC );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (3)\n" );


	// 11. enqueue the kernel object for execution:

	size_t globalWorkSize[3] = { arr_size, 1, 1 };
	size_t localWorkSize[3]  = { loc_size,   1, 1 };

	Wait( cmdQueue );
	double time0 = omp_get_wtime( );
	status = clEnqueueNDRangeKernel( cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL );
	if (status != CL_SUCCESS) {
		//fprintf(stderr, "clEnqueueNDRangeKernel failed: %d\n", status);
		clReleaseProgram(program);
		clReleaseCommandQueue(cmdQueue);
		clReleaseMemObject(dA);
		clReleaseMemObject(dB);
		clReleaseMemObject(dC);

		delete[] hA;
		delete[] hB;
		delete[] hC;
		return -1;
	}
	Wait( cmdQueue );
	double time1 = omp_get_wtime( );

	// 12. read the results buffer back from the device to the host:

	status = clEnqueueReadBuffer( cmdQueue, dC, CL_TRUE, 0, dataSize, hC, 0, NULL, NULL );
	if( status != CL_SUCCESS )
			fprintf( stderr, "clEnqueueReadBuffer failed\n" );

	// 13. clean everything up:

	clReleaseKernel(        kernel   );
	clReleaseProgram(       program  );
	clReleaseCommandQueue(  cmdQueue );
	clReleaseMemObject(     dA  );
	clReleaseMemObject(     dB  );
	clReleaseMemObject(     dC  );

	delete [ ] hA;
	delete [ ] hB;
	delete [ ] hC;
	return (double)arr_size / (time1 - time0) / 1000000000.;
}

double
ArrayMultAdd(int arr_size, int loc_size)
{
	// see if we can even open the opencl kernel program
	// (no point going on if we can't):

	FILE *fp;
#ifdef WIN32
	errno_t err = fopen_s( &fp, CL_FILE_NAME, "rb" );
	if( err != 0 )
#else
	fp = fopen( CL_FILE_NAME, "rb" );
	if( fp == NULL )
#endif
	{
		fprintf( stderr, "Cannot open OpenCL source file '%s'\n", CL_FILE_NAME );
		return -1;
	}

	cl_int status;		// returned status from opencl calls
				// test against CL_SUCCESS

	// get the platform id:

	cl_platform_id platform;
	status = clGetPlatformIDs( 1, &platform, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clGetPlatformIDs failed (2)\n" );
	
	// get the device id:

	cl_device_id device;
	status = clGetDeviceIDs( platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clGetDeviceIDs failed (2)\n" );

	// 2. allocate the host memory buffers:

	float *hA = new float[arr_size];
	float *hB = new float[arr_size];
	float *hC = new float[arr_size];
	float *hD = new float[arr_size];
	// fill the host memory buffers:

	for( int i = 0; i < arr_size; i++ )
	{
		hA[i] = hB[i] = (float) sqrt(  (double)i  );
	}

	size_t dataSize = arr_size * sizeof(float);

	// 3. create an opencl context:

	cl_context context = clCreateContext( NULL, 1, &device, NULL, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateContext failed\n" );

	// 4. create an opencl command queue:

	cl_command_queue cmdQueue = clCreateCommandQueue( context, device, 0, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateCommandQueue failed\n" );

	// 5. allocate the device memory buffers:

	cl_mem dA = clCreateBuffer( context, CL_MEM_READ_ONLY,  dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (1)\n" );

	cl_mem dB = clCreateBuffer( context, CL_MEM_READ_ONLY,  dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (2)\n" );

	cl_mem dC = clCreateBuffer( context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (3) %d \n", arr_size);
		
	cl_mem dD = clCreateBuffer( context, CL_MEM_WRITE_ONLY, dataSize, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateBuffer failed (4) %d %d \n",arr_size,loc_size );

	// 6. enqueue the 2 commands to write the data from the host buffers to the device buffers:

	status = clEnqueueWriteBuffer( cmdQueue, dA, CL_FALSE, 0, dataSize, hA, 0, NULL, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clEnqueueWriteBuffer failed (1)\n" );

	status = clEnqueueWriteBuffer( cmdQueue, dB, CL_FALSE, 0, dataSize, hB, 0, NULL, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clEnqueueWriteBuffer failed (2)\n" );
	status = clEnqueueWriteBuffer( cmdQueue, dC, CL_FALSE, 0, dataSize, hB, 0, NULL, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clEnqueueWriteBuffer failed (3)\n" );
	Wait( cmdQueue );

	// 7. read the kernel code from a file:

	fseek( fp, 0, SEEK_END );
	size_t fileSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	char *clProgramText = new char[ fileSize+1 ];		// leave room for '\0'
	size_t n = fread( clProgramText, 1, fileSize, fp );
	clProgramText[fileSize] = '\0';
	fclose( fp );
	if( n != fileSize )
		fprintf( stderr, "Expected to read %d bytes read from '%s' -- actually read %d.\n", fileSize, CL_FILE_NAME, n );

	// create the text for the kernel program:

	char *strings[1];
	strings[0] = clProgramText;
	cl_program program = clCreateProgramWithSource( context, 1, (const char **)strings, NULL, &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateProgramWithSource failed\n" );
	delete [ ] clProgramText;

	// 8. compile and link the kernel code:

	char *options = { "" };
	status = clBuildProgram( program, 1, &device, options, NULL, NULL );
	if( status != CL_SUCCESS )
	{
		size_t size;
		clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size );
		cl_char *log = new cl_char[ size ];
		clGetProgramBuildInfo( program, device, CL_PROGRAM_BUILD_LOG, size, log, NULL );
		fprintf( stderr, "clBuildProgram failed:\n%s\n", log );
		delete [ ] log;
	}

	// 9. create the kernel object:

	cl_kernel kernel = clCreateKernel( program, "ArrayMultAdd", &status );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clCreateKernel failed\n" );

	// 10. setup the arguments to the kernel object:

	status = clSetKernelArg( kernel, 0, sizeof(cl_mem), &dA );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (1)\n" );

	status = clSetKernelArg( kernel, 1, sizeof(cl_mem), &dB );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (2)\n" );

	status = clSetKernelArg( kernel, 2, sizeof(cl_mem), &dC );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (3)\n" );
	status = clSetKernelArg( kernel, 3, sizeof(cl_mem), &dD );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clSetKernelArg failed (4)\n" );

	// 11. enqueue the kernel object for execution:

	size_t globalWorkSize[3] = { arr_size, 1, 1 };
	size_t localWorkSize[3]  = { loc_size,   1, 1 };

	Wait( cmdQueue );
	double time0 = omp_get_wtime( );
	status = clEnqueueNDRangeKernel( cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL );
	if( status != CL_SUCCESS )
		fprintf( stderr, "clEnqueueNDRangeKernel failed: %d\n", status );
	Wait( cmdQueue );
	double time1 = omp_get_wtime( );

	// 12. read the results buffer back from the device to the host:

	status = clEnqueueReadBuffer( cmdQueue, dC, CL_TRUE, 0, dataSize, hC, 0, NULL, NULL );
	if (status != CL_SUCCESS)
		fprintf(stderr, "clEnqueueReadBuffer failed\n");

	// 13. clean everything up:

	clReleaseKernel(        kernel   );
	clReleaseProgram(       program  );
	clReleaseCommandQueue(  cmdQueue );
	clReleaseMemObject(     dA  );
	clReleaseMemObject(     dB  );
	clReleaseMemObject(     dC  );
	clReleaseMemObject(     dD  );
	delete [ ] hA;
	delete [ ] hB;
	delete [ ] hC;
	delete [ ] hD;
	return (double)arr_size / (time1 - time0) / 1000000000.;
}

double
ArrayMultRedux(int arr_size, int loc_size)
{
	// see if we can even open the opencl kernel program
	// (no point going on if we can't):

	FILE *fp;
#ifdef WIN32
	errno_t err = fopen_s(&fp, CL_FILE_NAME, "rb");
	if (err != 0)
#else
	fp = fopen(CL_FILE_NAME, "rb");
	if (fp == NULL)
#endif
	{
		fprintf(stderr, "Cannot open OpenCL source file '%s'\n", CL_FILE_NAME);
		return -1;
	}

	cl_int status;		// returned status from opencl calls
						// test against CL_SUCCESS

						// get the platform id:

	cl_platform_id platform;
	status = clGetPlatformIDs(1, &platform, NULL);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clGetPlatformIDs failed (2)\n");

	// get the device id:

	cl_device_id device;
	status = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clGetDeviceIDs failed (2)\n");

	// 2. allocate the host memory buffers:
	size_t numWorkGroups = arr_size / loc_size;
	float * hA = new float[arr_size];
	float * hB = new float[arr_size];
	float * hC = new float[numWorkGroups];
	size_t  abSize = arr_size * sizeof(float);
	size_t    cSize = numWorkGroups * sizeof(float);

	// fill the host memory buffers:

	for (int i = 0; i < arr_size; i++)
	{
		hA[i] = hB[i] = (float)sqrt((double)i);
	}

	size_t dataSize = arr_size * sizeof(float);

	// 3. create an opencl context:

	cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateContext failed\n");

	// 4. create an opencl command queue:

	cl_command_queue cmdQueue = clCreateCommandQueue(context, device, 0, &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateCommandQueue failed\n");

	// 5. allocate the device memory buffers:
	cl_mem  dA = clCreateBuffer(context, CL_MEM_READ_ONLY, abSize, NULL, &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateBuffer failed (1)\n");

	cl_mem  dB = clCreateBuffer(context, CL_MEM_READ_ONLY, abSize, NULL, &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateBuffer failed (2)\n");

	cl_mem  dC = clCreateBuffer(context, CL_MEM_WRITE_ONLY, cSize, NULL, &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateBuffer failed (3) %d \n", arr_size);

	// 6. enqueue the 2 commands to write the data from the host buffers to the device buffers:

	status = clEnqueueWriteBuffer(cmdQueue, dA, CL_FALSE, 0, abSize, hA, 0, NULL, NULL);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clEnqueueWriteBuffer failed (1)\n");

	status = clEnqueueWriteBuffer(cmdQueue, dB, CL_FALSE, 0, abSize, hB, 0, NULL, NULL);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clEnqueueWriteBuffer failed (2)\n");

	Wait(cmdQueue);

	// 7. read the kernel code from a file:

	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *clProgramText = new char[fileSize + 1];		// leave room for '\0'
	size_t n = fread(clProgramText, 1, fileSize, fp);
	clProgramText[fileSize] = '\0';
	fclose(fp);
	if (n != fileSize)
		fprintf(stderr, "Expected to read %d bytes read from '%s' -- actually read %d.\n", fileSize, CL_FILE_NAME, n);

	// create the text for the kernel program:

	char *strings[1];
	strings[0] = clProgramText;
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)strings, NULL, &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateProgramWithSource failed\n");
	delete[] clProgramText;

	// 8. compile and link the kernel code:

	char *options = { "" };
	status = clBuildProgram(program, 1, &device, options, NULL, NULL);
	if (status != CL_SUCCESS)
	{
		size_t size;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &size);
		cl_char *log = new cl_char[size];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, size, log, NULL);
		fprintf(stderr, "clBuildProgram failed:\n%s\n", log);
		delete[] log;
		clReleaseProgram(program);
		clReleaseCommandQueue(cmdQueue);
		clReleaseMemObject(dA);
		clReleaseMemObject(dB);
		clReleaseMemObject(dC);

		delete[] hA;
		delete[] hB;
		delete[] hC;
		return -1;
	}
	// 9. create the kernel object:

	cl_kernel kernel = clCreateKernel(program, "ArrayMultRedux", &status);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clCreateKernel failed\n");

	// 10. setup the arguments to the kernel object:

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &dA);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clSetKernelArg failed (1)\n");

	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &dB);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clSetKernelArg failed (2)\n");

	status = clSetKernelArg(kernel, 2, sizeof (float), NULL); // local “prods” array – one per work-item
	if (status != CL_SUCCESS)
		fprintf(stderr, "clSetKernelArg failed (3)\n");
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), &dC);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clSetKernelArg failed (2)\n");


	// 11. enqueue the kernel object for execution:

	size_t globalWorkSize[3] = { arr_size, 1, 1 };
	size_t localWorkSize[3] = { loc_size,   1, 1 };

	Wait(cmdQueue);
	double time0 = omp_get_wtime();
	status = clEnqueueNDRangeKernel(cmdQueue, kernel, 1, NULL, globalWorkSize, localWorkSize, 0, NULL, NULL);
	if (status != CL_SUCCESS)
		fprintf(stderr, "clEnqueueNDRangeKernel failed: %d\n", status);
	Wait(cmdQueue);
	double time1 = omp_get_wtime();

	// 13. clean everything up:

	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(cmdQueue);
	clReleaseMemObject(dA);
	clReleaseMemObject(dB);
	clReleaseMemObject(dC);

	delete[] hA;
	delete[] hB;
	delete[] hC;
	return (double)arr_size / (time1 - time0) / 1000000000.;
}
