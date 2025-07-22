/*
 Authors 
 Martin Schlather, martin.schlather@uni-mannheim.de

 Copyright (C) 2022-2023 Martin Schlather

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/


#ifndef parallel_omp_base_H
#define parallel_omp_base_H 1


// NEVER_** is used by parallel_**, so no conflict with NO_, which is used
// in the programmes. Except that there is no differences between
// NEVER_ and NO_

// #define NEVER_OMP 1
// #define NEVER_AVX 1
// #define NEVER_SSE 1

#define max_number_of_cores 128
extern int number_of_cores;


#define CNVT22STR(x) #x
#define CNVT2STR(x) CNVT22STR(x)


// *********************************************************
//  ****  HARDWARE + OS
// *********************************************************


#if defined WIN32 || defined _WIN32 || defined __WIN32__
#define MSDOS_WINDOWS 1
#elif defined MSDOS_WINDOWS
#undef MSDOS_WINDOWS
#endif


#if defined __x86_64 || defined __x86_64__ || defined __amd64__ || defined __amd64 || defined _M_X64
#define X86_64 1
#elif defined X86_64
#undef X86_64
#endif


#if defined __arm64__ || defined __arm64 || defined __aarch64__
#define ARM64 1
#elif defined ARM64
#undef ARM64
#endif

#if defined __arm32__ || defined __arm__ || defined ARM64
#define ARM32 1
#define NO_AVX 1
#endif

#if defined __ARM_NEON__ || defined __aarch64__ || defined _M_ARM || defined _M_ARM64
#define NEON 1
#elif defined NEON
#undef NEON
#endif

#if defined MSDOS_WINDOWS || defined (__APPLE__) || defined(__sun)
  #if defined TIME_AVAILABLE
    #undef TIME_AVAILABLE
  #endif
#else
#define TIME_AVAILABLE 1
#endif




// *********************************************************
//  ****  PARALLEL
// *********************************************************


#define DO_OMP 1

#if defined DO_POSIX // user should #define DO_POSIX to get it
  #undef DO_POSIX
  #if defined DO_PARALLEL
    #undef DO_PARALLEL
  #endif
  #define DO_PARALLEL 2
#endif
#define DO_POSIX 2

#if defined DO_FIXED_POSIX // user should #define DO_FIXED_POSIX to get it
  #undef DO_FIXED_POSIX
  #if defined DO_PARALLEL
    #undef DO_PARALLEL
  #endif
  #define DO_PARALLEL 3
#endif
#define DO_FIXED_POSIX 3


// delete DO_PARALLEL if necessary
#if defined DO_PARALLEL
  #if defined NEVER_OMP
    #undef DO_PARALLEL
    #warning("parallel processing is turned off")
  #elif defined MSDOS_WINDOWS 
    #undef DO_PARALLEL // only DO_OMP possible!
    #if defined NO_OMP || !defined _OPENMP
      #warning("parallel processing is being turned off on windows")
    #endif
  #elif defined NO_OMP || defined ARM32 || defined __APPLE__ || !defined _OPENMP
    #undef DO_PARALLEL  // only posix possible!   
  #endif
#endif // DO_PARALLEL

// set DO_PARALLEL if reasonable
#if !defined DO_PARALLEL && !NEVER_OMP
  #if defined MSDOS_WINDOWS
    #if !defined NO_OMP && defined _OPENMP
      #define DO_PARALLEL DO_OMP
    #endif
  #elif defined NO_OMP || !defined _OPENMP || defined ARM32 || defined __APPLE__
    #define DO_PARALLEL DO_FIXED_POSIX
  #else
    #if defined SCHLATHERS_MACHINE
      #define DO_PARALLEL DO_OMP // may change value when debugging
    #else
      #define DO_PARALLEL DO_OMP// never changes value 
    #endif
  #endif
#endif


#if defined STICK_THREAD_TO_CORE
#undef STICK_THREAD_TO_CORE
#endif
#if DO_PARALLEL == DO_POSIX || DO_PARALLEL == DO_FIXED_POSIX
  #include <pthread.h>
#else // DO_PARALLEL
  #include <omp.h>
#endif
#if DO_PARALLEL == DO_FIXED_POSIX
  void stick_this_thread_to_core(int thread);
  void initialize_thread_to_core(long loop, int *cores, long *partial_start);
  void initialize_fixed_threads(int *cores, int weakperformance,
				int hyperperformance, int hyperturnOn,
				int prnt);
#define INITIALIZE_FIXED_THREADS(cores, weak, hyper, turnedon, prnt)	\
  initialize_fixed_threads(cores, weak, hyper, turnedon, prnt)
  #define STICK_THIS_THREAD_TO_CORE(thread) stick_this_thread_to_core(thread)
#else
  #define STICK_THREAD_TO_CORE(A)
#define INITIALIZE_FIXED_THREADS(cores, weak, hyper, turnedoff, info)
  #define STICK_THIS_THREAD_TO_CORE(thread) 
#endif


// *********************************************************
//  ****  SIMD 
// *********************************************************

//#pragma message CNVT2STR(MY_VARIANT)

#if defined MY_VARIANT //&& ! defined MY_LDABITALIGN
#if MY_VARIANT ==  32
  #define PlainInteger32 1
  #if !defined SSE2 
    #define NO_SSE2 1
  #endif
#elif MY_VARIANT ==  64
  #define PlainInteger64 1
  #if !defined SSE2
    #define NO_SSE2 1
  #endif
#elif MY_VARIANT == 128
  #if !defined AVX && !defined AVX2
    #define NO_AVX 1
  #endif
#elif MY_VARIANT == 256
  #if !defined AVX512F
    #define NO_AVX512 1
  #endif
#elif MY_VARIANT == 512
#else
#error unknown MY_VARIANT
#endif
#endif



#if defined NEVER_SSE
  #ifndef NO_SSE2
     #define NO_SSE2 1
  #endif
#elif defined NEVER_AVX
  #ifndef NO_AVX
     #define NO_AVX 1
  #endif
#elif defined NEVER_AVX512
  #ifndef NO_AVX512
     #define NO_AVX512 1
  #endif
#endif // NEVER_SIMDs


#if defined NO_SSE2 && ! defined NO_SSE3
#define NO_SSE3 1
#endif
#if defined NO_SSE3 && ! defined NO_SSSE3
#define NO_SSSE3 1
#endif
#if defined NO_SSSE3 && ! defined NO_SSE41
#define NO_SSE41 1
#endif
#if defined NO_SSE41 && ! defined NO_AVX
#define NO_AVX 1
#endif
#if defined NO_AVX && ! defined NO_AVX2
#define NO_AVX2 1
#endif
#if defined NO_AVX2 && ! defined NO_AVX512
#define NO_AVX512 1
#endif

#if ! defined NO_AVX512

#if ! defined DO_AVX512BITALG && ! defined DO_AVX512BW && ! defined DO_AVX512CD && ! defined DO_AVX512DQ && ! defined DO_AVX512ER && ! defined DO_AVX512F && ! defined DO_AVX512IFMA && ! defined DO_AVX512PF && ! defined DO_AVX512VBMI && ! defined DO_AVX512VL && ! defined DO_AVX512VPOPCNTDQ  && ! defined DO_AVX5124FMAPS && ! defined DO_AVX5124VNNIW
#define DO_AVX512BITALG 1
#define DO_AVX512BW 1 
#define DO_AVX512CD 1
#define DO_AVX512DQ 1
#define DO_AVX512ER 1
#define DO_AVX512F 1
#define DO_AVX512IFMA 1
#define DO_AVX512PF 1
#define DO_AVX512VBMI 1
#define DO_AVX512VL 1
#define DO_AVX512VPOPCNTDQ 1
#define DO_AVX5124FMAPS 1
#define DO_AVX5124VNNIW 1
#endif

#if defined __AVX512BITALG__ && defined DO_AVX512BITALG
#define AVX512BITALG 1
#endif
#if defined __AVX512BW__ && defined DO_AVX512BW
#define AVX512BW 1
#endif
#if defined __AVX512CD__ && defined DO_AVX512CD
#define AVX512CD 1
#endif
#if defined __AVX512DQ__ && defined DO_AVX512DQ
#define AVX512DQ 1
#endif
#if defined __AVX512ER__ && defined DO_AVX512ER
#define AVX512ER 1
#endif
#if defined __AVX512F__ && defined DO_AVX512F
#define AVX512F 1
#define AVX512 1
#endif
#if defined __AVX512IFMA__ && defined DO_AVX512IFMA
#define AVX512IFMA 1
#endif
#if defined __AVX512PF__ && defined DO_AVX512PF
#define AVX512PF 1
#endif
#if defined __AVX512VBMI__ && defined DO_AVX512VBMI
#define AVX512VBMI 1
#endif
#if defined __AVX512VL__ && defined DO_AVX512VL
#define AVX512VL 1 //
#endif
#if defined __AVX512VPOPCNTDQ__ && defined DO_AVX512VPOPCNTDQ
#define AVX512VPOPCNTDQ 1 //
#endif
#if defined __AVX5124FMAPS__ && defined DO_AVX5124FMAPS
#define AVX5124FMAPS 1
#endif
#if defined __AVX5124VNNIW__ && defined DO_AVX5124VNNIW
#define AVX5124VNNIW 1
#endif

#endif // end ! no 512


#if defined __AVX2__ && ! defined NO_AVX2
#define AVX2 1
//defined;
#elif defined AVX2
#undef AVX2
//make_undefined;
#else
//undefined;
#endif

#if defined __AVX__  && ! defined NO_AVX
#define AVX 1
#elif defined AVX
#undef AVX
#endif

#if (defined __SSE4_1__ || defined NEON) && ! defined NO_SSE41
#define SSE41 1
#elif defined SSE41
#undef SSE41
#endif


#if (defined __SSSE3__ || defined NEON)  && ! defined NO_SSSE3
#define SSSE3 1
#elif defined SSSE3
#undef SSSE3
#endif

#if (defined __SSE3__ || defined NEON) && ! defined NO_SSE3
#define SSE3 1
#elif defined SSE3
#undef SSE3
#endif

#if (defined __SSE2__ || defined NEON) && ! defined NO_SSE2
//#error xxx.ooo
#define SSE2 1 
#elif defined SSE2
#undef SSE2
#endif


// *********************************************************
//  ****  END SIMD
// *********************************************************




#endif
