
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


#include "Basic_HaploBlocker.h"

#if defined compatibility_to_R_h
#include "options.h"

void options_localcopy(KEY_type *KT, option_type *options){
  assert(blockerN==6 && messagesN == 1);
  
}

void options_localdel(KEY_type *KT){
}


void FREElocal(KEY_type VARIABLE_IS_NOT_USED *KT){
}

/*

SEXP attachoptions() { // no print commands!!!
  #ifdef SCHLATHERS_MACHINE
  PRINTF("floating point double precision: %s\n",
#if defined DO_FLOAT
	 "no"
#else
	 "yes"
#endif
	 );
#endif
#if defined ReturnAttachMessage
  ReturnAttachMessage(HaploBlocker, true);
#else
  return R_NilValue;
#endif
}
*/


#endif
