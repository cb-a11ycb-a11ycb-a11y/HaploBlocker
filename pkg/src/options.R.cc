
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
#include "options.h"
#include "kleinkram.h"


#if defined compatibility_to_R_h


void setLoptions(int i, int j, SEXP el, char name[LEN_OPTIONNAME],
		  bool VARIABLE_IS_NOT_USED isList, bool local) {

  if (!local && Ext_parallel())
    ERR0("'RFoptions' may not be set from a parallel process.");
  
  option_type *options;
  utilsoption_type *utils;
  WhichOptionList(local, &options, &utils);
  
  switch(i) {
  case 0: {// blocker
    blocker_options *gp;
    gp = &(options->blocker);
    switch(j) {
    case 0: gp->ANY_diff_value = POS0NUM; break;
     default: BUG;
    }}
    break;
  case 1: {
    messages_options *gp = &(options->messages);
    switch(j) {
    case 0: gp->warn_dummy = LOGI; break;
    default: BUG;
    }}
    break;   
  default: BUG;
  }
}



void getLoptions(SEXP sublist, int i, bool local) {
  int  k = 0;
  option_type *options;
  utilsoption_type *utils;
  WhichOptionList(local, &options, &utils);

  switch(i) {
  case 0 : {
    blocker_options *p = &(options->blocker);
    ADD(ScalarReal(p->ANY_diff_value));
  }
    break;

    case 1 : {
   messages_options *p = &(options->messages);
   ADD(ScalarLogical(p->warn_dummy));
  }
    break;   
 
  default : BUG;
  }
    assert (i == prefixN - 1);
}


#endif
