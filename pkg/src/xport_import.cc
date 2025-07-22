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

#include <R_ext/Rdynload.h>
#include "intrinsics.h"
#include "General_utils.h"
#include "options.h"
#include "kleinkram.h"
#include "chaploblocker.h"


extern bool BigEndian;

void startUtils(void);
void fixcodingIntern(int *values, Long nval);

Uint startLocal(int cores) {
  startUtils();
  Uint simd_info = noUSE; // check_intrinsics();

  const bool local=false;
 utilsoption_type S;
  load_utilsoptions(&S, local);
  S.basic.cores = GreaterZero(cores);  
  BigEndian = S.basic.bigendian;
  
  //  assert(!BigEndian);
  int value = -2;
  fixcodingIntern(&value, 1);
  assert(codes != NULL || coding_type == local);
  // printf("leaving stratlocal\n");
 
  return simd_info;
}



