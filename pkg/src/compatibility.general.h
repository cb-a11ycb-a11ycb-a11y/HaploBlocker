
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


#ifndef compatibility_general_h 
#define compatibility_general_h 1

#ifndef __cplusplus
#include <stdbool.h>
#endif
#include "compatibility.header.h"
#include <inttypes.h>

typedef unsigned int Uint;
typedef uint64_t Ulong;
typedef int64_t Long;
typedef unsigned char Uchar;

#define F77call F77_CALL // rename to control that USE_FC_LEN_T has been called
#ifdef __cplusplus
#define F77name extern "C" void F77_NAME // rename to control that USE_FC_LEN_T has been called
#else
//#warning not cplusplus
#define F77name void F77_NAME 
#endif
#define F77dgesdd F77call(dgesdd)
#define F77dgemv F77call(dgemv)
#define F77ddot F77call(ddot)
#define F77dsyrk F77call(dsyrk)


int stopIfNotIntI(Long i, Long line, const char *file);
#define stopIfNotInt(i) stopIfNotIntI(i, __LINE__, __FILE__)

Uint stopIfNotUIntI(Long i, Long line, const char *file);
#define stopIfNotUint(i) stopIfNotUIntI(i, __LINE__, __FILE__)

int stopIfNotAnyIntI(Long i, Long line, const char *file);
#define stopIfNotAnyInt(i) stopIfNotAnyIntI(i, __LINE__, __FILE__)

#define stopIfNotSame(i,j)\
  if (sizeof(i) == sizeof(j)) {} else { ERR6("'%s' (%ld) and '%s' (%ld) do not have the same size at line %d in '%s'\n", #i, (long) sizeof(i), #j, (long) sizeof(j), __LINE__, __FILE__) }


#if defined STAND_ALONE
//#warning STAND_ALONE
#include "compatibility.C.h"
#else
//#warning R_VERSION
#include "compatibility.R.h"
#endif



#if !defined STAND_ALONE || defined DO_PARALLEL
#define ASSERT_SOLVE(sp) assert(sp != NULL);
#else
#define ASSERT_SOLVE(sp) if (sp == NULL) sp = &(WhichOptionList(false)->solve); else {}
#endif

void *notNull(void *X, int line, const char *file);
  
#endif
