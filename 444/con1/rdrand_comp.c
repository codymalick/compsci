/*
 * Source Material for these functions:
 * https://software.intel.com/en-us/articles/intel-digital-random-number-generator-drng-software-implementation-guide
 */
/*
 *License from intel to use their rdrand call
 *Copyright (c) 2014, Intel Corporation

 *All rights reserved.

 *Redistribution and use in source and binary forms, with or without
 *modification, are permitted provided that the following conditions are met:

 *1. Redistributions of source code must retain the above copyright notice,
 *this list of conditions and the following disclaimer.

 *2. Redistributions in binary form must reproduce the above copyright notice,
 *this list of conditions and the following disclaimer in the documentation
 *and/or other materials provided with the distribution.

 *3. Neither the name of the copyright holder nor the names of its
 *contributors may be used to endorse or promote products derived from this
 *software without specific prior written permission.

 *THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *POSSIBILITY OF SUCH DAMAGE.
 */

/* These are bits that are OR’d together */
#include<stdint.h>
#include<string.h>
#define DRNG_NO_SUPPORT	0x0	/* For clarity */
#define DRNG_HAS_RDRAND	0x1
#define DRNG_HAS_RDSEED	0x2

typedef struct cpuid_struct {
	unsigned int eax;
	unsigned int ebx;
	unsigned int ecx;
	unsigned int edx;
} cpuid_t;

void cpuid (cpuid_t *info, unsigned int leaf, unsigned int subleaf)
{
	asm volatile("cpuid"
		: "=a" (info->eax), "=b" (info->ebx), "=c" (info->ecx), "=d" (info->edx)
		: "a" (leaf), "c" (subleaf)
		);
}

int _is_intel_cpu()
{
	static int intel_cpu= -1;
	cpuid_t info;

	if ( intel_cpu == -1 ) {
		cpuid(&info, 0, 0);
		if (
			memcmp((char *) &info.ebx, "Genu", 4) ||
			memcmp((char *) &info.edx, "ineI", 4) ||
			memcmp((char *) &info.ecx, "ntel", 4)
		) {
			intel_cpu= 0;
		} else {
			intel_cpu= 1;
		}
	}

	return intel_cpu;
}

/*checks the cpu for rdrand support*/
int get_drng_support ()
{
	static int drng_features= -1;

	/*
	 * So we don't call cpuid multiple times for
	 * the same information
	 */

	if ( drng_features == -1 ) {
		drng_features= DRNG_NO_SUPPORT;

		if ( _is_intel_cpu() ) {
			cpuid_t info;
			cpuid(&info, 1, 0);

			if ( (info.ecx & 0x40000000) == 0x40000000 ) {
				drng_features|= DRNG_HAS_RDRAND;
				cpuid(&info, 7, 0);

				if ( (info.ebx & 0x40000) == 0x40000 ) {
					drng_features|= DRNG_HAS_RDSEED;
				}
			}
		}

	return drng_features;
	}
}

/* Actual rdrand call*/
int rdrand32_step (uint32_t *rand)
{
	unsigned char ok;
	asm volatile ("rdrand %0; setc %1" : "=r" (*rand), "=qm" (ok));
	return (int) ok;
}

/*
int rdseed32_step (uint32_t *seed)
{
	unsigned char ok;
	asm volatile ("rdseed %0; setc %1" : "=r" (*seed), "=qm" (ok));
	return (int) ok;
}
*/
