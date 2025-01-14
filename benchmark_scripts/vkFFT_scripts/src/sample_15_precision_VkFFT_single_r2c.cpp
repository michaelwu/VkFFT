//general parts
#include <stdio.h>
#include <vector>
#include <memory>
#include <string.h>
#include <chrono>
#include <thread>
#include <iostream>
#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif
#include <inttypes.h>

#if(VKFFT_BACKEND==0)
#include "vulkan/vulkan.h"
#include "glslang_c_interface.h"
#elif(VKFFT_BACKEND==1)
#include <cuda.h>
#include <cuda_runtime.h>
#include <nvrtc.h>
#include <cuda_runtime_api.h>
#include <cuComplex.h>
#elif(VKFFT_BACKEND==2)
#ifndef __HIP_PLATFORM_HCC__
#define __HIP_PLATFORM_HCC__
#endif
#include <hip/hip_runtime.h>
#include <hip/hiprtc.h>
#include <hip/hip_runtime_api.h>
#include <hip/hip_complex.h>
#elif(VKFFT_BACKEND==3)
#ifndef CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#endif
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif 
#elif(VKFFT_BACKEND==4)
#include <ze_api.h>
#elif(VKFFT_BACKEND==5)
#include "Foundation/Foundation.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "Metal/Metal.hpp"
#endif
#include "vkFFT.h"
#include "utils_VkFFT.h"
#include "fftw3.h"
#ifdef USE_cuFFT
#include "precision_cuFFT_r2c.h"
#endif	
#ifdef USE_rocFFT
#include "precision_rocFFT_r2c.h"
#endif	
VkFFTResult sample_15_precision_VkFFT_single_r2c(VkGPU* vkGPU, uint64_t file_output, FILE* output, uint64_t isCompilerInitialized)
{
	VkFFTResult resFFT = VKFFT_SUCCESS;
#if(VKFFT_BACKEND==0)
	VkResult res = VK_SUCCESS;
#elif(VKFFT_BACKEND==1)
	cudaError_t res = cudaSuccess;
#elif(VKFFT_BACKEND==2)
	hipError_t res = hipSuccess;
#elif(VKFFT_BACKEND==3)
	cl_int res = CL_SUCCESS;
#elif(VKFFT_BACKEND==4)
	ze_result_t res = ZE_RESULT_SUCCESS;
#elif(VKFFT_BACKEND==5)
#endif
	if (file_output)
		fprintf(output, "15 - VkFFT / FFTW R2C+C2R precision test in single precision\n");
	printf("15 - VkFFT / FFTW R2C+C2R precision test in single precision\n");

	const int num_benchmark_samples = 277;
	const int num_runs = 1;

	uint64_t benchmark_dimensions[num_benchmark_samples][5] = { {1, 1, 1, 1, 3}, {(uint64_t)pow(2,5), 1, 1, 1, 1}, {(uint64_t)pow(2,6), 1, 1, 1, 1},{(uint64_t)pow(2,7), 1, 1, 1, 1},{(uint64_t)pow(2,8), 1, 1, 1, 1},{(uint64_t)pow(2,9), 1, 1, 1, 1},{(uint64_t)pow(2,10), 1, 1, 1, 1},
		{(uint64_t)pow(2,11), 1, 1, 1, 1},{(uint64_t)pow(2,12), 1, 1, 1, 1},{(uint64_t)pow(2,13), 1, 1, 1, 1},{(uint64_t)pow(2,14), 1, 1, 1, 1},{(uint64_t)pow(2,15), 1, 1, 1, 1},{(uint64_t)pow(2,16), 1, 1, 1, 1},{(uint64_t)pow(2,17), 1, 1, 1, 1},{(uint64_t)pow(2,18), 1, 1, 1, 1},
		{(uint64_t)pow(2,19), 1, 1, 1, 1},{(uint64_t)pow(2,20), 1, 1, 1, 1},{(uint64_t)pow(2,21), 1, 1, 1, 1},{(uint64_t)pow(2,22), 1, 1, 1, 1},{(uint64_t)pow(2,23), 1, 1, 1, 1},{(uint64_t)pow(2,24), 1, 1, 1, 1},{(uint64_t)pow(2,25), 1, 1, 1, 1},{(uint64_t)pow(2,26), 1, 1, 1, 1},

		{8, (uint64_t)pow(2,3), 1, 1, 2},{8, (uint64_t)pow(2,4), 1, 1, 2},{8, (uint64_t)pow(2,5), 1, 1, 2},{8, (uint64_t)pow(2,6), 1, 1, 2},{8, (uint64_t)pow(2,7), 1, 1, 2},{8, (uint64_t)pow(2,8), 1, 1, 2},{8, (uint64_t)pow(2,9), 1, 1, 2},{8, (uint64_t)pow(2,10), 1, 1, 2},
		{8, (uint64_t)pow(2,11), 1, 1, 2},{8, (uint64_t)pow(2,12), 1, 1, 2},{8, (uint64_t)pow(2,13), 1, 1, 2},{8, (uint64_t)pow(2,14), 1, 1, 2},{8, (uint64_t)pow(2,15), 1, 1, 2},{8, (uint64_t)pow(2,16), 1, 1, 2},{8, (uint64_t)pow(2,17), 1, 1, 2},{8, (uint64_t)pow(2,18), 1, 1, 2},
		{8, (uint64_t)pow(2,19), 1, 1, 2},{8, (uint64_t)pow(2,20), 1, 1, 2},{8, (uint64_t)pow(2,21), 1, 1, 2},{8, (uint64_t)pow(2,22), 1, 1, 2},{8, (uint64_t)pow(2,23), 1, 1, 2},{8, (uint64_t)pow(2,24), 1, 1, 2},

		{ (uint64_t)pow(2,3), (uint64_t)pow(2,3), 1, 1, 2},{ (uint64_t)pow(2,4), (uint64_t)pow(2,4), 1, 1, 2},{ (uint64_t)pow(2,5), (uint64_t)pow(2,5), 1, 1, 2},{ (uint64_t)pow(2,6), (uint64_t)pow(2,6), 1, 1, 2},{ (uint64_t)pow(2,7), (uint64_t)pow(2,7), 1, 1, 2},{ (uint64_t)pow(2,8), (uint64_t)pow(2,8), 1, 1, 2},{ (uint64_t)pow(2,9), (uint64_t)pow(2,9), 1, 1, 2},
		{ (uint64_t)pow(2,10), (uint64_t)pow(2,10), 1, 1, 2},{ (uint64_t)pow(2,11), (uint64_t)pow(2,11), 1, 1, 2},{ (uint64_t)pow(2,12), (uint64_t)pow(2,12), 1, 1, 2},{ (uint64_t)pow(2,13), (uint64_t)pow(2,13), 1, 1, 2},{ (uint64_t)pow(2,14), (uint64_t)pow(2,13), 1, 1, 2},

		{ (uint64_t)pow(2,3), (uint64_t)pow(2,3), (uint64_t)pow(2,3), 1, 3},{ (uint64_t)pow(2,4), (uint64_t)pow(2,4), (uint64_t)pow(2,4), 1, 3},{ (uint64_t)pow(2,5), (uint64_t)pow(2,5), (uint64_t)pow(2,5), 1, 3},{ (uint64_t)pow(2,6), (uint64_t)pow(2,6), (uint64_t)pow(2,6), 1, 3},{ (uint64_t)pow(2,7), (uint64_t)pow(2,7), (uint64_t)pow(2,7), 1, 3},{ (uint64_t)pow(2,8), (uint64_t)pow(2,8), (uint64_t)pow(2,8), 1, 3},{ (uint64_t)pow(2,9), (uint64_t)pow(2,9), (uint64_t)pow(2,9), 1, 3},
		{720, 480, 1, 1, 2},{1280, 720, 1, 1, 2},{1920, 1080, 1, 1, 2}, {2560, 1440, 1, 1, 2},{3840, 2160, 1, 1, 2},

		{3, 1, 1, 1, 1},{5, 1, 1, 1, 1},{6, 1, 1, 1, 1},{7, 1, 1, 1, 1},{9, 1, 1, 1, 1},{10, 1, 1, 1, 1},{11, 1, 1, 1, 1},{12, 1, 1, 1, 1},{13, 1, 1, 1, 1},{14, 1, 1, 1, 1},
		{15, 1, 1, 1, 1},{17, 1, 1, 1, 1},{19, 1, 1, 1, 1},{21, 1, 1, 1, 1},{22, 1, 1, 1, 1},{23, 1, 1, 1, 1},{24, 1, 1, 1, 1},{25, 1, 1, 1, 1},{26, 1, 1, 1, 1},{27, 1, 1, 1, 1},{28, 1, 1, 1, 1},{29, 1, 1, 1, 1},{30, 1, 1, 1, 1},{31, 1, 1, 1, 1},{33, 1, 1, 1, 1},{35, 1, 1, 1, 1},{37, 1, 1, 1, 1},{39, 1, 1, 1, 1},{41, 1, 1, 1, 1},{42, 1, 1, 1, 1},{43, 1, 1, 1, 1},{44, 1, 1, 1, 1},{45, 1, 1, 1, 1},{49, 1, 1, 1, 1},{52, 1, 1, 1, 1},{55, 1, 1, 1, 1},{56, 1, 1, 1, 1},{60, 1, 1, 1, 1},{65, 1, 1, 1, 1},{66, 1, 1, 1, 1},{81, 1, 1, 1, 1},
		{121, 1, 1, 1, 1},{125, 1, 1, 1, 1},{143, 1, 1, 1, 1},{149, 1, 1, 1, 1},{169, 1, 1, 1, 1},{229, 1, 1, 1, 1},{243, 1, 1, 1, 1},{286, 1, 1, 1, 1},{343, 1, 1, 1, 1},{429, 1, 1, 1, 1},{487, 1, 1, 1, 1},{572, 1, 1, 1, 1},{625, 1, 1, 1, 1},{720, 1, 1, 1, 1},{1080, 1, 1, 1, 1},{1001, 1, 1, 1, 1},{1181, 1, 1, 1, 1},{1287, 1, 1, 1, 1},{1400, 1, 1, 1, 1},{1440, 1, 1, 1, 1},{1920, 1, 1, 1, 1},{1951, 1, 1, 1, 1},{2160, 1, 1, 1, 1},{3024,1,1, 1, 1},{3500,1,1, 1, 1},
		{3840, 1, 1, 1, 1},{4000 , 1, 1, 1, 1},{4050, 1, 1, 1, 1},{4320 , 1, 1, 1, 1},{4864 , 1, 1, 1, 1},{7000,1,1, 1, 1},{7680, 1, 1, 1, 1},{9000, 1, 1, 1, 1},{4864 *5, 1, 1, 1, 1},{7680 * 5, 1, 1, 1, 1},{39829 * 2, 1, 1, 1, 1},
		{3, 8, 1, 1, 2},{5, 8, 1, 1, 2},{6, 8, 1, 1, 2},{7, 8, 1, 1, 2},{9, 8, 1, 1, 2},{10, 8, 1, 1, 2},{18, 8, 1, 1, 2},{12, 8, 1, 1, 2},{13, 8, 1, 1, 2},{14, 8, 1, 1, 2},
		{15, 8, 1, 1, 2},{17, 8, 1, 1, 2},{19, 8, 1, 1, 2},{21, 8, 1, 1, 2},{22, 8, 1, 1, 2},{23, 8, 1, 1, 2},{24, 8, 1, 1, 2},{25, 8, 1, 1, 2},{26, 8, 1, 1, 2},{27, 8, 1, 1, 2},{28, 8, 1, 1, 2},{29, 8, 1, 1, 2},{30, 8, 1, 1, 2},{31, 8, 1, 1, 2},{33, 8, 1, 1, 2},{34, 8, 1, 1, 2},{35, 8, 1, 1, 2},{37, 8, 1, 1, 2},{39, 8, 1, 1, 2},{41, 8, 1, 1, 2},{42, 8, 1, 1, 2},{44, 8, 1, 1, 2},{45, 8, 1, 1, 2},{46, 8, 1, 1, 2},{49, 8, 1, 1, 2},{52, 8, 1, 1, 2},{55, 8, 1, 1, 2},{56, 8, 1, 1, 2},{60, 8, 1, 1, 2},{65, 8, 1, 1, 2},{66, 8, 1, 1, 2},{88, 8, 1, 1, 2},
		{128, 8, 1, 1, 2},{125, 8, 1, 1, 2},{143, 8, 1, 1, 2},{149, 8, 1, 1, 2},{169, 8, 1, 1, 2},{229, 8, 1, 1, 2},{243, 8, 1, 1, 2},{286, 8, 1, 1, 2},{343, 8, 1, 1, 2},{429, 8, 1, 1, 2},{482, 1, 1, 1, 1},{572, 8, 1, 1, 2},{625, 8, 1, 1, 2},{720, 8, 1, 1, 2},{1080, 8, 1, 1, 2},{1008, 8, 1, 1, 2},{1181, 8, 1, 1, 2},{1287, 8, 1, 1, 2},{1400, 8, 1, 1, 2},{1440, 8, 1, 1, 2},{1920, 8, 1, 1, 2},{1951, 8, 1, 1, 2},{2160, 8, 1, 1, 2},{3024,1,1, 1, 1},{3500,1,1, 1, 1},
		{3840, 8, 1, 1, 2},{4000 , 8, 1, 1, 2},{4050, 8, 1, 1, 2},{4320 , 8, 1, 1, 2},{4864 , 8, 1, 1, 2},{7000,1,1, 1, 1},{7680, 8, 1, 1, 2},{9000, 8, 1, 1, 2},{4864 * 5, 8, 1, 1, 2},{7680 * 5, 8, 1, 1, 2},{39829*2, 8, 1, 1, 2},
		{3, 3, 3, 3, 4},{5, 5, 5, 5, 4},{6, 6, 6, 6, 4},{7, 7, 7, 7, 4},{9, 9, 9, 9, 4},{10, 10, 10, 10, 4},{11, 11, 11, 11, 4},{12, 12, 12, 12, 4},{13, 13, 13, 13, 4},{14, 14, 14, 14, 4},
		{15, 15, 15, 15, 4},{17, 17, 17, 17, 4},{21, 21, 21, 21, 4},{22, 22, 22, 22, 4},{23, 23, 23, 23, 4},{24, 24, 24, 24, 4},{25, 25, 25, 25, 4},{26, 26, 26, 26, 4},{27, 27, 27, 27, 4},{28, 28, 28, 28, 4},{29, 29, 29, 29, 4},{30, 30, 30, 30, 4},{31, 31, 31, 31, 4},{33, 33, 33, 33, 4},{35, 35, 35, 35, 4},{37, 37, 37, 37, 4},{39, 39, 39, 39, 4},{41, 41, 41, 41, 4},{42, 42, 42, 42, 4},{43, 43, 43, 43, 4},{44, 44, 44, 44, 4},{45, 45, 45, 45, 4},{47, 47, 47, 47, 4},{49, 49, 49, 49, 4},{52, 52, 52, 52, 4},{53, 53, 53, 53, 4},{56, 56, 56, 56, 4},{59, 59, 59, 59, 4},{60, 60, 60, 60, 4},{61, 61, 61, 61, 4},{81, 81, 81, 81, 4},
		{3, 5, 7, 9, 4},{5, 3, 7, 9, 4},{9, 7, 5, 3, 4},{23, 25, 27, 29, 4},{25, 23, 27, 29, 4},{29, 27, 25, 23, 4},{123, 25, 127, 129, 4},{125, 123, 27, 129, 4},{129, 127, 125, 23, 4},
		{20000, 2, 3, 3, 4},{3, 20000, 2, 3, 4},{3, 2, 3, 20000, 4}
	};

	double benchmark_result = 0;//averaged result = sum(system_size/iteration_time)/num_benchmark_samples

	for (int n = 0; n < num_benchmark_samples; n++) {
		for (int r = 0; r < num_runs; r++) {

			float* inputC;
			double* inputC_double;
			uint64_t dims[4] = { benchmark_dimensions[n][0] , benchmark_dimensions[n][1] , benchmark_dimensions[n][2] , benchmark_dimensions[n][3]};
			
			inputC = (float*)(malloc(sizeof(float) * (dims[0]) * dims[1] * dims[2] * dims[3]));
			inputC_double = (double*)(malloc(sizeof(double) * (dims[0]) * dims[1] * dims[2] * dims[3]));
			for (uint64_t k = 0; k < dims[3]; k++) {
				for (uint64_t l = 0; l < dims[2]; l++) {
					for (uint64_t j = 0; j < dims[1]; j++) {
						for (uint64_t i = 0; i < dims[0]; i++) {
							inputC[i + j * (dims[0]) + l * (dims[0]) * dims[1] + k * dims[0] * dims[1] * dims[2]] = (float)(2 * ((float)rand()) / RAND_MAX - 1.0);
							inputC_double[i + j * (dims[0]) + l * (dims[0]) * dims[1] + k * dims[0] * dims[1] * dims[2]] = (double)inputC[i + j * (dims[0]) + l * (dims[0]) * dims[1] + k * dims[0] * dims[1] * dims[2]];
						}
					}
				}
			}
			fftw_plan p;

			fftw_complex* output_FFTW = (fftw_complex*)(malloc(sizeof(fftw_complex) * (dims[0] / 2 + 1) * dims[1] * dims[2]* dims[3]));
			if (!output_FFTW) return VKFFT_ERROR_MALLOC_FAILED;
			
			double* output_FFTWR = (double*)(malloc(sizeof(double) * (dims[0]) * dims[1] * dims[2]* dims[3]));
			if (!output_FFTWR) return VKFFT_ERROR_MALLOC_FAILED;
			switch (benchmark_dimensions[n][4]) {
			case 1:
				p = fftw_plan_dft_r2c_1d((int)benchmark_dimensions[n][0], inputC_double, output_FFTW, FFTW_ESTIMATE);
				break;
			case 2:
				p = fftw_plan_dft_r2c_2d((int)benchmark_dimensions[n][1], (int)benchmark_dimensions[n][0], inputC_double, output_FFTW, FFTW_ESTIMATE);
				break;
			case 3:
				p = fftw_plan_dft_r2c_3d((int)benchmark_dimensions[n][2], (int)benchmark_dimensions[n][1], (int)benchmark_dimensions[n][0], inputC_double, output_FFTW, FFTW_ESTIMATE);
				break;
			case 4:
				fftw_iodim dims[4];
				dims[0].n = (int)benchmark_dimensions[n][3];
				dims[0].is = (int)(benchmark_dimensions[n][2]*benchmark_dimensions[n][1]*benchmark_dimensions[n][0]);
				dims[0].os = (int)(benchmark_dimensions[n][2]*benchmark_dimensions[n][1]*(benchmark_dimensions[n][0]/2+1));
				dims[1].n = (int)benchmark_dimensions[n][2];
				dims[1].is = (int)(benchmark_dimensions[n][1]*benchmark_dimensions[n][0]);
				dims[1].os = (int)(benchmark_dimensions[n][1]*(benchmark_dimensions[n][0]/2+1));
				dims[2].n = (int)benchmark_dimensions[n][1];
				dims[2].is = (int)(benchmark_dimensions[n][0]);
				dims[2].os = (int)(benchmark_dimensions[n][0]/2+1);
				dims[3].n = (int)benchmark_dimensions[n][0];
				dims[3].is = 1;
				dims[3].os = 1;
				fftw_iodim howmany_dims[1];
				howmany_dims[0].n = 1;
				howmany_dims[0].is = 1;
				howmany_dims[0].os = 1;

				p = fftw_plan_guru_dft_r2c(4, dims, 1, howmany_dims, inputC_double, output_FFTW, FFTW_ESTIMATE);
				break;
			}

			fftw_execute(p);
			fftw_destroy_plan(p);
			
			switch (benchmark_dimensions[n][4]) {
			case 1:
				p = fftw_plan_dft_c2r_1d((int)benchmark_dimensions[n][0], output_FFTW, output_FFTWR, FFTW_ESTIMATE);
				break;
			case 2:
				p = fftw_plan_dft_c2r_2d((int)benchmark_dimensions[n][1], (int)benchmark_dimensions[n][0], output_FFTW, output_FFTWR, FFTW_ESTIMATE);
				break;
			case 3:
				p = fftw_plan_dft_c2r_3d((int)benchmark_dimensions[n][2], (int)benchmark_dimensions[n][1], (int)benchmark_dimensions[n][0], output_FFTW, output_FFTWR, FFTW_ESTIMATE);
				break;
			case 4:
				fftw_iodim fftw_iodims[4];
				fftw_iodims[0].n = (int)benchmark_dimensions[n][3];
				fftw_iodims[0].os = (int)(benchmark_dimensions[n][2]*benchmark_dimensions[n][1]*benchmark_dimensions[n][0]);
				fftw_iodims[0].is = (int)(benchmark_dimensions[n][2]*benchmark_dimensions[n][1]*(benchmark_dimensions[n][0]/2+1));
				fftw_iodims[1].n = (int)benchmark_dimensions[n][2];
				fftw_iodims[1].os = (int)(benchmark_dimensions[n][1]*benchmark_dimensions[n][0]);
				fftw_iodims[1].is = (int)(benchmark_dimensions[n][1]*(benchmark_dimensions[n][0]/2+1));
				fftw_iodims[2].n = (int)benchmark_dimensions[n][1];
				fftw_iodims[2].os = (int)(benchmark_dimensions[n][0]);
				fftw_iodims[2].is = (int)(benchmark_dimensions[n][0]/2+1);
				fftw_iodims[3].n = (int)benchmark_dimensions[n][0];
				fftw_iodims[3].os = 1;
				fftw_iodims[3].is = 1;
				fftw_iodim howmany_dims[1];
				howmany_dims[0].n = 1;
				howmany_dims[0].is = 1;
				howmany_dims[0].os = 1;

				p = fftw_plan_guru_dft_c2r(4, fftw_iodims, 1, howmany_dims, output_FFTW, output_FFTWR, FFTW_ESTIMATE);
				break;
			}
			fftw_execute(p);
#ifdef USE_cuFFT
			//fftwf_complex* output_extFFT = (fftwf_complex*)(malloc(sizeof(fftwf_complex) * (dims[0]/2+1) * dims[1] * dims[2]));
			float* output_extFFT = (float*)(malloc(sizeof(float) * (dims[0]) * dims[1] * dims[2]));
			if (!output_extFFT) return VKFFT_ERROR_MALLOC_FAILED;
			if (benchmark_dimensions[n][4] < 4) {
				launch_precision_cuFFT_r2c(inputC, (void*)output_extFFT, (int)vkGPU->device_id, benchmark_dimensions[n]);
			}
#endif // USE_cuFFT
#ifdef USE_rocFFT
			//fftwf_complex* output_extFFT = (fftwf_complex*)(malloc(sizeof(fftwf_complex) * (dims[0]/2+1) * dims[1] * dims[2]));
			float* output_extFFT = (float*)(malloc(sizeof(float) * (dims[0]) * dims[1] * dims[2]));
			if (!output_extFFT) return VKFFT_ERROR_MALLOC_FAILED;
			if (benchmark_dimensions[n][4] < 4) {
				launch_precision_rocFFT_r2c(inputC, (void*)output_extFFT, (int)vkGPU->device_id, benchmark_dimensions[n]);
			}
#endif // USE_rocFFT
			float totTime = 0;
			int num_iter = 1;

			//VkFFT part

			VkFFTConfiguration configuration = {};
			VkFFTApplication app = {};
			configuration.FFTdim = benchmark_dimensions[n][4]; //FFT dimension, 1D, 2D or 3D (default 1).
			configuration.size[0] = benchmark_dimensions[n][0]; //Multidimensional FFT dimensions sizes (default 1). For best performance (and stability), order dimensions in descendant size order as: x>y>z.   
			configuration.size[1] = benchmark_dimensions[n][1];
			configuration.size[2] = benchmark_dimensions[n][2];
			configuration.size[3] = benchmark_dimensions[n][3];
			configuration.performR2C = 1;
			//configuration.disableMergeSequencesR2C = 1;
			configuration.inverseReturnToInputBuffer = 1;
			//configuration.keepShaderCode = 1;
			//configuration.printMemoryLayout = 1;
			//configuration.coalescedMemory = 64;
			//configuration.useLUT = 1;
			//After this, configuration file contains pointers to Vulkan objects needed to work with the GPU: VkDevice* device - created device, [uint64_t *bufferSize, VkBuffer *buffer, VkDeviceMemory* bufferDeviceMemory] - allocated GPU memory FFT is performed on. [uint64_t *kernelSize, VkBuffer *kernel, VkDeviceMemory* kernelDeviceMemory] - allocated GPU memory, where kernel for convolution is stored.
#if(VKFFT_BACKEND==5)
            configuration.device = vkGPU->device;
#else
            configuration.device = &vkGPU->device;
#endif
#if(VKFFT_BACKEND==0)
			configuration.queue = &vkGPU->queue; //to allocate memory for LUT, we have to pass a queue, vkGPU->fence, commandPool and physicalDevice pointers 
			configuration.fence = &vkGPU->fence;
			configuration.commandPool = &vkGPU->commandPool;
			configuration.physicalDevice = &vkGPU->physicalDevice;
			configuration.isCompilerInitialized = isCompilerInitialized;//compiler can be initialized before VkFFT plan creation. if not, VkFFT will create and destroy one after initialization
#elif(VKFFT_BACKEND==3)
			configuration.context = &vkGPU->context;
#elif(VKFFT_BACKEND==4)
			configuration.context = &vkGPU->context;
			configuration.commandQueue = &vkGPU->commandQueue;
			configuration.commandQueueID = vkGPU->commandQueueID;
#elif(VKFFT_BACKEND==5)
            configuration.queue = vkGPU->queue;
#endif			

			uint64_t numBuf = 1;

			uint64_t* inputBufferSize = (uint64_t*)malloc(sizeof(uint64_t) * numBuf);
			if (!inputBufferSize) return VKFFT_ERROR_MALLOC_FAILED;
			for (uint64_t i = 0; i < numBuf; i++) {
				inputBufferSize[i] = {};
				inputBufferSize[i] = (uint64_t)sizeof(float) * configuration.size[0] * configuration.size[1] * configuration.size[2] * configuration.size[3] / numBuf;
			}
			uint64_t* bufferSize = (uint64_t*)malloc(sizeof(uint64_t) * numBuf);
			if (!bufferSize) return VKFFT_ERROR_MALLOC_FAILED;
			for (uint64_t i = 0; i < numBuf; i++) {
				bufferSize[i] = {};
				bufferSize[i] = (uint64_t)sizeof(float) * 2 * (configuration.size[0] / 2 + 1) * configuration.size[1] * configuration.size[2] * configuration.size[3] / numBuf;
			}
#if(VKFFT_BACKEND==0)
			VkBuffer* ibuffer = (VkBuffer*)malloc(numBuf * sizeof(VkBuffer));
			if (!ibuffer) return VKFFT_ERROR_MALLOC_FAILED;
			VkDeviceMemory* ibufferDeviceMemory = (VkDeviceMemory*)malloc(numBuf * sizeof(VkDeviceMemory));
			if (!ibufferDeviceMemory) return VKFFT_ERROR_MALLOC_FAILED;
			VkBuffer* buffer = (VkBuffer*)malloc(numBuf * sizeof(VkBuffer));
			if (!buffer) return VKFFT_ERROR_MALLOC_FAILED;
			VkDeviceMemory* bufferDeviceMemory = (VkDeviceMemory*)malloc(numBuf * sizeof(VkDeviceMemory));
			if (!bufferDeviceMemory) return VKFFT_ERROR_MALLOC_FAILED;
#elif(VKFFT_BACKEND==1)
			float* ibuffer = 0;
			cuFloatComplex* buffer = 0;
#elif(VKFFT_BACKEND==2)
			float* ibuffer = 0;
			hipFloatComplex* buffer = 0;
#elif(VKFFT_BACKEND==3)
			cl_mem ibuffer = 0;
			cl_mem buffer = 0;
#elif(VKFFT_BACKEND==4)
			void* ibuffer = 0;
			void* buffer = 0;
#elif(VKFFT_BACKEND==5)
            MTL::Buffer* ibuffer = 0;
            MTL::Buffer* buffer = 0;
#endif
			for (uint64_t i = 0; i < numBuf; i++) {
#if(VKFFT_BACKEND==0)
				buffer[i] = {};
				bufferDeviceMemory[i] = {};
				resFFT = allocateBuffer(vkGPU, &buffer[i], &bufferDeviceMemory[i], VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, bufferSize[i]);
				if (resFFT != VKFFT_SUCCESS) return resFFT;
				ibuffer[i] = {};
				ibufferDeviceMemory[i] = {};
				resFFT = allocateBuffer(vkGPU, &ibuffer[i], &ibufferDeviceMemory[i], VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_HEAP_DEVICE_LOCAL_BIT, inputBufferSize[i]);
				if (resFFT != VKFFT_SUCCESS) return resFFT;
#elif(VKFFT_BACKEND==1)
				res = cudaMalloc((void**)&ibuffer, inputBufferSize[i]);
				if (res != cudaSuccess) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
				res = cudaMalloc((void**)&buffer, bufferSize[i]);
				if (res != cudaSuccess) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
#elif(VKFFT_BACKEND==2)
				res = hipMalloc((void**)&ibuffer, inputBufferSize[i]);
				if (res != hipSuccess) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
				res = hipMalloc((void**)&buffer, bufferSize[i]);
				if (res != hipSuccess) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
#elif(VKFFT_BACKEND==3)
				ibuffer = clCreateBuffer(vkGPU->context, CL_MEM_READ_WRITE, inputBufferSize[i], 0, &res);
				if (res != CL_SUCCESS) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
				buffer = clCreateBuffer(vkGPU->context, CL_MEM_READ_WRITE, bufferSize[i], 0, &res);
				if (res != CL_SUCCESS) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
#elif(VKFFT_BACKEND==4)
				ze_device_mem_alloc_desc_t device_desc = {};
				device_desc.stype = ZE_STRUCTURE_TYPE_DEVICE_MEM_ALLOC_DESC;
				res = zeMemAllocDevice(vkGPU->context, &device_desc, inputBufferSize[i], sizeof(float), vkGPU->device, &ibuffer);
				if (res != ZE_RESULT_SUCCESS) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
				res = zeMemAllocDevice(vkGPU->context, &device_desc, bufferSize[i], sizeof(float), vkGPU->device, &buffer);
				if (res != ZE_RESULT_SUCCESS) return VKFFT_ERROR_FAILED_TO_ALLOCATE;
#elif(VKFFT_BACKEND==5)
                ibuffer = vkGPU->device->newBuffer(inputBufferSize[i], MTL::ResourceStorageModePrivate);
                buffer = vkGPU->device->newBuffer(bufferSize[i], MTL::ResourceStorageModePrivate);
#endif
			}
			configuration.inputBufferNum = numBuf;
			configuration.bufferNum = numBuf;
			configuration.bufferSize = bufferSize;
			configuration.isInputFormatted = 1;
			configuration.inputBufferStride[0] = configuration.size[0];
			configuration.inputBufferStride[1] = configuration.size[0] * configuration.size[1];
			configuration.inputBufferStride[2] = configuration.size[0] * configuration.size[1] * configuration.size[2];
			configuration.inputBufferStride[3] = configuration.size[0] * configuration.size[1] * configuration.size[2] * configuration.size[3];
			configuration.inputBufferSize = inputBufferSize;


			//Sample buffer transfer tool. Uses staging buffer (if needed) of the same size as destination buffer, which can be reduced if transfer is done sequentially in small buffers.
			uint64_t shift = 0;
			for (uint64_t i = 0; i < numBuf; i++) {
#if(VKFFT_BACKEND==0)
				resFFT = transferDataFromCPU(vkGPU, (inputC + shift / sizeof(fftwf_complex)), &ibuffer[i], inputBufferSize[i]);
				if (resFFT != VKFFT_SUCCESS) return resFFT;
#else
                resFFT = transferDataFromCPU(vkGPU, (inputC + shift / sizeof(fftwf_complex)), &ibuffer, inputBufferSize[i]);
                if (resFFT != VKFFT_SUCCESS) return resFFT;
#endif
				shift += inputBufferSize[i];
			}
			//Initialize applications. This function loads shaders, creates pipeline and configures FFT based on configuration file. No buffer allocations inside VkFFT library.  
			resFFT = initializeVkFFT(&app, configuration);
			if (resFFT != VKFFT_SUCCESS) return resFFT;

			//Submit FFT+iFFT.
			//num_iter = 1;
			//specify buffers at launch
			VkFFTLaunchParams launchParams = {};
#if(VKFFT_BACKEND==0)
			launchParams.inputBuffer = ibuffer;
			launchParams.buffer = buffer;
#elif(VKFFT_BACKEND==1)
			launchParams.inputBuffer = (void**)&ibuffer;
			launchParams.buffer = (void**)&buffer;
#elif(VKFFT_BACKEND==2)
			launchParams.inputBuffer = (void**)&ibuffer;
			launchParams.buffer = (void**)&buffer;
#elif(VKFFT_BACKEND==3)
			launchParams.inputBuffer = &ibuffer;
			launchParams.buffer = &buffer;
#elif(VKFFT_BACKEND==4)
			launchParams.inputBuffer = (void**)&ibuffer;
			launchParams.buffer = (void**)&buffer;
#elif(VKFFT_BACKEND==5)
            launchParams.inputBuffer = &ibuffer;
            launchParams.buffer = &buffer;
#endif
			resFFT = performVulkanFFT(vkGPU, &app, &launchParams, -1, num_iter);
			if (resFFT != VKFFT_SUCCESS) return resFFT;
			VkFFTLaunchParams launchParams2 = {};
#if(VKFFT_BACKEND==0)
			launchParams2.inputBuffer = ibuffer;
			launchParams2.buffer = buffer;
#elif(VKFFT_BACKEND==1)
			launchParams2.inputBuffer = (void**)&ibuffer;
			launchParams2.buffer = (void**)&buffer;
#elif(VKFFT_BACKEND==2)
			launchParams2.inputBuffer = (void**)&ibuffer;
			launchParams2.buffer = (void**)&buffer;
#elif(VKFFT_BACKEND==3)
			launchParams2.inputBuffer = &ibuffer;
			launchParams2.buffer = &buffer;
#elif(VKFFT_BACKEND==4)
			launchParams2.inputBuffer = (void**)&ibuffer;
			launchParams2.buffer = (void**)&buffer;
#elif(VKFFT_BACKEND==5)
            launchParams2.inputBuffer = &ibuffer;
            launchParams2.buffer = &buffer;
#endif
			resFFT = performVulkanFFT(vkGPU, &app, &launchParams2, 1, num_iter);
			if (resFFT != VKFFT_SUCCESS) return resFFT;
			//fftwf_complex* output_VkFFT = (fftwf_complex*)(malloc(sizeof(fftwf_complex) * (dims[0] / 2 + 1) * dims[1] * dims[2]));
			float* output_VkFFT = (float*)(malloc(bufferSize[0]));
			if (!output_VkFFT) return VKFFT_ERROR_MALLOC_FAILED;
			//Transfer data from GPU using staging buffer.
			shift = 0;
			for (uint64_t i = 0; i < numBuf; i++) {
#if(VKFFT_BACKEND==0)
				//resFFT = transferDataToCPU(vkGPU, (output_VkFFT + shift / sizeof(fftwf_complex)), &buffer[i], bufferSize[i]);
				resFFT = transferDataToCPU(vkGPU, (output_VkFFT + shift / sizeof(fftwf_complex)), &ibuffer[i], inputBufferSize[i]);
				if (resFFT != VKFFT_SUCCESS) return resFFT;
#else
                //resFFT = transferDataToCPU(vkGPU, (output_VkFFT + shift / sizeof(fftwf_complex)), &buffer, bufferSize[i]);
                resFFT = transferDataToCPU(vkGPU, (output_VkFFT + shift / sizeof(fftwf_complex)), &ibuffer, inputBufferSize[i]);
                if (resFFT != VKFFT_SUCCESS) return resFFT;
#endif
				shift += inputBufferSize[i];
			}
			double avg_difference[2] = { 0,0 };
			double max_difference[2] = { 0,0 };
			double avg_eps[2] = { 0,0 };
			double max_eps[2] = { 0,0 };
			for (uint64_t k = 0; k < dims[3]; k++) {
				for (uint64_t l = 0; l < dims[2]; l++) {
					for (uint64_t j = 0; j < dims[1]; j++) {
						for (uint64_t i = 0; i < dims[0]; i++) {
							uint64_t loc_i = i;
							uint64_t loc_j = j;
							uint64_t loc_l = l;

							//printf("%.2e %.2e - %.2e %.2e \n", output_FFTW[i + j * dims[0] + l * dims[0] * dims[1]][0], output_FFTW[i + j * dims[0] + l * dims[0] * dims[1]][1], output_VkFFT[(loc_i + loc_j * dims[0] + loc_l * dims[0] * dims[1])][0], output_VkFFT[(loc_i + loc_j * dims[0] + loc_l * dims[0] * dims[1])][1]);
							//if (i > dims[0] - 10)
							//printf("%.2e %.2e - %.2e %.2e\n", output_FFTW[i + j * (dims[0]) + l * (dims[0]) * dims[1]][0], output_FFTW[i + j * (dims[0]) + l * (dims[0]) * dims[1]][1], output_VkFFT[2*(loc_i + loc_j * (dims[0]) + loc_l * (dims[0]) * dims[1])], output_VkFFT[1+2 * (loc_i + loc_j * (dims[0]) + loc_l * (dims[0]) * dims[1])]);

							//printf("%.2e - %.2e \n", output_FFTWR[i + j * (dims[0]) + l * (dims[0]) * dims[1]], output_VkFFT[(loc_i + loc_j * (dims[0]) + loc_l * (dims[0]) * dims[1])]);
							double current_data_norm = sqrt(output_FFTWR[i + j * (dims[0]) + l * (dims[0]) * dims[1] + k * (dims[0]) * dims[1] * dims[2]] * output_FFTWR[i + j * (dims[0]) + l * (dims[0]) * dims[1] + k * (dims[0]) * dims[1] * dims[2]]);
#if defined(USE_cuFFT) || defined(USE_rocFFT)
							double current_diff_x_extFFT = (output_extFFT[loc_i + loc_j * (dims[0]) + loc_l * (dims[0]) * dims[1]] - output_FFTWR[i + j * (dims[0]) + l * (dims[0]) * dims[1]]);
							double current_diff_norm_extFFT = sqrt(current_diff_x_extFFT * current_diff_x_extFFT);
							if (current_diff_norm_extFFT > max_difference[0]) max_difference[0] = current_diff_norm_extFFT;
							avg_difference[0] += current_diff_norm_extFFT;
							if ((current_diff_norm_extFFT / current_data_norm > max_eps[0])) {
								max_eps[0] = current_diff_norm_extFFT / current_data_norm;
							}
							avg_eps[0] += current_diff_norm_extFFT / current_data_norm;
#endif
							double current_diff_x_VkFFT = (output_VkFFT[loc_i + loc_j * (dims[0]) + loc_l * (dims[0]) * dims[1] + k * (dims[0]) * dims[1] * dims[2]] - output_FFTWR[i + j * (dims[0]) + l * (dims[0]) * dims[1] + k * (dims[0]) * dims[1] * dims[2]]);
							double current_diff_norm_VkFFT = sqrt(current_diff_x_VkFFT * current_diff_x_VkFFT);
							if (current_diff_norm_VkFFT > max_difference[1]) max_difference[1] = current_diff_norm_VkFFT;
							avg_difference[1] += current_diff_norm_VkFFT;
							if ((current_diff_norm_VkFFT / current_data_norm > max_eps[1])) {
								max_eps[1] = current_diff_norm_VkFFT / current_data_norm;
							}
							avg_eps[1] += current_diff_norm_VkFFT / current_data_norm;
						}
					}
				}
			}
			avg_difference[0] /= (dims[0] * dims[1] * dims[2] * dims[3]);
			avg_eps[0] /= (dims[0] * dims[1] * dims[2] * dims[3]);
			avg_difference[1] /= (dims[0] * dims[1] * dims[2] * dims[3]);
			avg_eps[1] /= (dims[0] * dims[1] * dims[2] * dims[3]);
#ifdef USE_cuFFT
			if (benchmark_dimensions[n][4] < 4) {
				if (file_output)
					fprintf(output, "cuFFT System: %" PRIu64 "x%" PRIu64 "x%" PRIu64 "x%" PRIu64 " avg_difference: %.2e max_difference: %.2e avg_eps: %.2e max_eps: %.2e\n", dims[0], dims[1], dims[2], dims[3], avg_difference[0], max_difference[0], avg_eps[0], max_eps[0]);
				printf("cuFFT System: %" PRIu64 "x%" PRIu64 "x%" PRIu64 "x%" PRIu64 " avg_difference: %.2e max_difference: %.2e avg_eps: %.2e max_eps: %.2e\n", dims[0], dims[1], dims[2], dims[3], avg_difference[0], max_difference[0], avg_eps[0], max_eps[0]);
			}
#endif
#ifdef USE_rocFFT
			if (benchmark_dimensions[n][4] < 4) {
				if (file_output)
					fprintf(output, "rocFFT System: %" PRIu64 "x%" PRIu64 "x%" PRIu64 "x%" PRIu64 " avg_difference: %.2e max_difference: %.2e avg_eps: %.2e max_eps: %.2e\n", dims[0], dims[1], dims[2], dims[3], avg_difference[0], max_difference[0], avg_eps[0], max_eps[0]);
				printf("rocFFT System: %" PRIu64 "x%" PRIu64 "x%" PRIu64 "x%" PRIu64 " avg_difference: %.2e max_difference: %.2e avg_eps: %.2e max_eps: %.2e\n", dims[0], dims[1], dims[2], dims[3], avg_difference[0], max_difference[0], avg_eps[0], max_eps[0]);
			}
#endif
			if (file_output)
				fprintf(output, "VkFFT System: %" PRIu64 "x%" PRIu64 "x%" PRIu64 "x%" PRIu64 " avg_difference: %.2e max_difference: %.2e avg_eps: %.2e max_eps: %.2e\n", dims[0], dims[1], dims[2], dims[3], avg_difference[1], max_difference[1], avg_eps[1], max_eps[1]);
			printf("VkFFT System: %" PRIu64 "x%" PRIu64 "x%" PRIu64 "x%" PRIu64 " avg_difference: %.2e max_difference: %.2e avg_eps: %.2e max_eps: %.2e\n", dims[0], dims[1], dims[2], dims[3], avg_difference[1], max_difference[1], avg_eps[1], max_eps[1]);
			free(output_VkFFT);
			for (uint64_t i = 0; i < numBuf; i++) {

#if(VKFFT_BACKEND==0)
				vkDestroyBuffer(vkGPU->device, buffer[i], NULL);
				vkFreeMemory(vkGPU->device, bufferDeviceMemory[i], NULL);
				vkDestroyBuffer(vkGPU->device, ibuffer[i], NULL);
				vkFreeMemory(vkGPU->device, ibufferDeviceMemory[i], NULL);
#elif(VKFFT_BACKEND==1)
				cudaFree(ibuffer);
				cudaFree(buffer);
#elif(VKFFT_BACKEND==2)
				hipFree(ibuffer);
				hipFree(buffer);
#elif(VKFFT_BACKEND==3)
				clReleaseMemObject(ibuffer);
				clReleaseMemObject(buffer);
#elif(VKFFT_BACKEND==4)
				zeMemFree(vkGPU->context, ibuffer);
				zeMemFree(vkGPU->context, buffer);
#elif(VKFFT_BACKEND==5)
                ibuffer->release();
                buffer->release();
#endif

			}
#if(VKFFT_BACKEND==0)
			free(buffer);
			free(bufferDeviceMemory);
			free(ibuffer);
			free(ibufferDeviceMemory);
#endif
			free(inputBufferSize);
			free(bufferSize);
			deleteVkFFT(&app);
			free(inputC);
			fftw_destroy_plan(p);
			free(inputC_double);
			free(output_FFTW);
			free(output_FFTWR);
#if defined(USE_cuFFT) || defined(USE_rocFFT)
			free(output_extFFT);
#endif
		}
	}
	return resFFT;
}
