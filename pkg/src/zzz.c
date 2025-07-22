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



#include "def.h"
#include "zzz.h"

#if defined compatibility_to_R_h
#include "chaploblocker.h"

static R_NativePrimitiveArgType
// RFU INCLUDE host_arg[] = { STRSXP, INTSXP},
  int_arg[] = { INTSXP };
//char2int7[] = { CHARSXP, CHARSXP, INTSXP, INTSXP, INTSXP, INTSXP, INTSXP,
//	      INTSXP, INTSXP}; 


static const R_CMethodDef cMethods[]  = {
  CDEF(loadoptions, 1, int_arg),
  CDEF(detachoptions, 0, none),
  //  CDEF(, 0, none),
  {NULL, NULL, 0, none}
};


static R_CallMethodDef callMethods[]  = {
  // in die respectiven C-Dateien muss adoption.h eingebunden sein
  // CALLDEF(attachoptions, 0),
  CALLDEF(fixcoding, 1),
  CALLDEF(codeSNPs, 4),
  CALLDEF(decodeSNPs, 1),
  CALLDEF(factorSNPs, 3),
  CALLDEF(colSumsEqualSNPs, 4),
  CALLDEF(intersect, 2),
  //  CALLDEF(codeSNPs, 2),
  {NULL, NULL, 0}
};


void R_init_HaploBlocker(DllInfo  *dll) {
  R_registerRoutines(dll, cMethods, // .C
		     callMethods,
		     NULL, // .Fortran
		     NULL); // ext
  R_useDynamicSymbols(dll, FALSE); // OK
  R_forceSymbols(dll, TRUE);// OK
  //  CALLABLE(codeInner);
}


#ifdef SCHLATHERS_MACHINE
#if defined __GNUC__ && __GCC__
// https://gcc.gnu.org/onlinedocs/gcc/Diagnostic-Pragmas.html
//#pragma GCC diagnostic ignored "-Wcast-function-type"
#endif
#endif
void R_unload_HaploBlocker(DllInfo VARIABLE_IS_NOT_USED *info) {
  // just to avoid warning from compiler on my computer
#ifdef SCHLATHERS_MACHINE
  if (info == NULL) {};
#endif
 // Release resources.
}


#endif
