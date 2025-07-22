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



#ifndef compatibility_to_C_h 
#define compatibility_to_C_h 1

#include "errors_messages.h"


#if defined compatibility_to_R_h
#error melange of C and R in header file calls of compatibility_to_*.h
#endif

// #include "compatibility.C.h"
//#include "options_RFU.h"

typedef long double LongDouble;
#define TRUNCL truncl


extern double ownNA;
extern double ownNaN;
extern int errno ;

#if !defined INT_MIN
#define INT_MIN (-2147483647-1)
//#define INT_MIN (-2147483648)
#endif
#define NA_INTEGER INT_MIN  // NA_INTEGER OK
#define R_FINITE(X) (FABS(X) <= 1e300)
#define ISNA(X) ((X) == RF_NA)
#define isNaN(X) ((X) == RF_NAN)
#define ISNAN(X) (ISNA(X) || isNaN(X))
#define warning(X) PRINTF("WARNING: %s", X);
// void warning(const char* format, ...);
// #define R_NilValue NULL
#define R_NilValue 1

#define NA_REAL ownNA
#define RF_NA NA_REAL
#define RF_NAN ownNaN
#define RF_NEGINF (-INFINITY)
#define RF_INF INFINITY
#define ACOS acos
#define ASIN asin
#define ATAN atan
#define ATANH atanh
#define ACOSH acosh
#define ASINH asinh
#define EXPM1 expm1
#define LOG1P log1p
#define FROUND fround
#define COS cos
#define EXP exp
#define FABS(X) fabs((double) X) // OK; keine Klammern um X!
#if ! defined MALLOCX
#define MALLOCX(X) notNull(malloc(X), __LINE__, __FILE__)
#define FLOOR floor
#define SQRT(X) sqrt((double) X) // OK
#define CEIL(X) ceil((double) X) // OK; keine Klammern um X!
#define FREEX free
#endif
#define LOG log
#define POW(X, Y) pow((double) X, (double) Y) // OK; keine Klammern um X!
#define SIN sin
#define STRCMP(A, B) strcmp(A, B) // OK
#define STRCPY(A, B) strcpy(A, B) // OK
#define STRLEN strlen
#define STRNCMP(A, B, C) strncmp(A, B, C) // OK
#define TAN tan
#define MEMCOPYX memcpy
#define MEMMOVE memmove
#define MEMSET memset  
#define MEMCMP memcmp
#define AALLOC(X) notNull(aligned_alloc(X), __LINE__, __FILE__)
#define CALLOCX(X,N) notNull(calloc(X,N), __LINE__, __FILE__)
#define SPRINTF sprintf // Rprintf
#define QSORT qsort
#define RFERROR(X) {fprintf(stderr, "%s", X); exit(EXIT_FAILURE); }
#define LOGGAMMAFN lgamma
#define GAMMAFN tgamma
#define RPRINTF printf // Rprintf
#define ATAN2 atan2
#define COSH cosh
#define SINH sinh
#define TANH tanh
#define SIGN(X) (((X) > 0) - ((X) < 0))
#define TRUNC(X) trunc((double) X) // OK; keine Klammern um X!


double gauss_random(double mu, double sigma);
double uniform_random(double a, double b); 
double poisson_random(double lambda);
#define GAUSS_RANDOM(SIGMA) gauss_random(0.0, SIGMA)
#define UNIFORM_RANDOM uniform_random(0.0, 1.0) // OK
#define POISSON_RANDOM(x) poisson_random(x)

#if defined USE_FC_LEN_T
#undef USE_FC_LEN_T
#warning USE_FC_LEN_T was already defined
#endif


#define F77_NAME(F) F##_  // Basic_utils
#define F77_CALL(F) F##_  // Basic_utils


F77name(dgeqrf)(int*, int*, double* , int *, double*, double*, int*, int*);
F77name(dsyevr)(const char*,  const char *, const char *, int*, double* ,
		int*, double* , double* ,
		int *, int*, double*,
		int*, double* , double* ,
		int *, int*, double*,
		int*, int*, int*, int*);
F77name(dgetrf)(int*, int*, double* , int *, int*, int*);
F77name(dgetrs)(const char *, int*, int*, double* , int *, int*,double*, int*,
		int*);
F77name(dgetri)(int*,  double* , int *, int*,double*, int*, int*);
F77name(dgesv)(int*, int*, double* , int *, int*,double*, int*, int*);
F77name(dpotrf)(const char*, int*, double* , int *, int*);
F77name(dtrmv)(const char*,  const char *, const char *,
	       int*, double* , int*,double*, int*);

F77name(dgesdd)(const char*, int*, int*, double* ,
		int*, double* , double* ,
		int*, double* , int*, double* ,
		int *, int*, int*);
F77name(dgemv)(const char*,  int*, int*, double* , double*,
	       int*, double* ,
	       int*, double* , double*,
	       int *);
F77name(ddot)(int*, double* , int*, double*, int*);
F77name(dsyrk)(const char*,  const char *,
	       int*, int*, double* ,  double*,
	       int *, double*,double*,
	       int*);

#define stopIfRandNotInt(i)
#define stopIfRandNotUInt(i)
#define stopIfRandNotAnyInt(i)
#define intIfR Long

#endif
