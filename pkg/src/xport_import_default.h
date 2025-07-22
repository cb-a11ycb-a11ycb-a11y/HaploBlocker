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



#ifndef xport_import_H
#define xport_import_H 1

/*
   UNUSED IN RFU !
*/

#include "def.h"
#include "General_utils.h"
#include "zzz_RFU.h"


#define KEY_type_default			\
  KEY_type *next;				\
  option_type global;				\
  utilsoption_type global_utils;		\
  int pid, visitingpid;				\
  bool ok, doshow;				\
  errorstring_type error_location;		\
  int *ToIntDummy;				\
  int  ToIntN = 0



#define UTILS_ALWAYS \
  CALL(startRFU);				\
  CALL(get_utilsoption);			\
  CALL(get_utils_basic);			\
  CALL(push_utilsoption);			\
  CALL(params_utilsoption);			\
  CALL(del_utilsoption);			\
  CALL(pid)


#if defined compatibility_to_R_h 
#define UTILSCALLS				\
  CALL(setoptionsRFU);		      		\
  CALL(getoptionsRFU);		      		\
  CALL(attachRFUoptions);			\
  CALL(detachRFUoptions);			\
  CALL(RFUoptions);		     		\
  CALL(attachSetNGet);		      		\
  CALL(parallel);			      	\
  UTILS_ALWAYS;			     		\
  UTILS_LOCAL

extern SEXP Information, Filecoding, Filename, Next, Missings, Precise, Doubled;
#else
#define UTILSCALLS  UTILS_ALWAYS; UTILS_LOCAL
#endif
  
#if defined CALL
#undef CALL
#endif
#define CALL(what) extern what##_type Ext_##what
UTILSCALLS;


typedef
struct option_type option_type;

void includeXport(void);
void WhichOptionList(bool local, option_type **global,
			     utilsoption_type **utils);


void FREEglobal(void);
void load_utilsoptions(utilsoption_type *S, int local);
Uint startLocal(int cores);

void getLoptions(SEXP sublist, int i, bool local);
void setLoptions(int i, int j, SEXP el, char name[LEN_OPTIONNAME], 
		 bool isList, bool local);


  
#define check_cuda check_7_5 // the lowest number for which code exists

#endif
