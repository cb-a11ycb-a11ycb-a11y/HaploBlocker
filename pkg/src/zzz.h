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


#ifndef zzz_H
#define zzz_H 1

#ifndef __cplusplus
#include <stdbool.h>
#endif

#include "Basic_RandomFieldsUtils.h"


#if defined compatibility_to_R_h

#define none 0
#define CDEF(name, n, type) {#name, (DL_FUNC) &name, n, type}
#define CALLDEF(name, n) {#name, (DL_FUNC) &name, n}
#define EXTDEF(name, n)  {#name, (DL_FUNC) &name, n}
#define CALLABLE(FCTN)  R_RegisterCCallable(pkg, #FCTN, (DL_FUNC) FCTN)

#endif

#endif
