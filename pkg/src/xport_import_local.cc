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
#include "Basic_RandomFieldsUtils.h"
#include "options_RFU.h"
#include "compatibility.SEXP.h"
#include "zzz_RFU.h"

#include "options.h"

#define importfrom "RandomFieldsUtils"

KEY_type *PIDKEY_LOCAL[PIDMODULUS];


#if defined CALL
#undef CALL
#endif
#define CALL(what) what##_type Ext_##what = NULL
UTILSCALLS;


#if defined CALL
#undef CALL
#endif

#if defined compatibility_to_C_h
#define CALL(what) Ext_##what = (what##_type) what; 
#else 
#define CALL(what) Ext_##what = (what##_type) R_GetCCallable(importfrom, #what)
#endif


void includeXport() {
  UTILSCALLS;
} 

#if defined compatibility_to_C_h  
extern utilsoption_type OPTIONS;
#endif
void WhichOptionList(bool VARIABLE_IS_NOT_USED local, option_type **global,
		     utilsoption_type **utils // readonly
		     ) {
  //WhichOptionsList considers 'local' whenever possible, otherwise alternatives
  option_type *G = &OPTIONS_LOCAL;
  utilsoption_type *U = NULL;
#if defined compatibility_to_R_h
  KEY_type *KT = KEYT_LOCAL();
  if (KT == NULL) BUG;
  U = &(KT->global_utils);  // readonly!
  if (local) G = &(KT->global);
#else
  U = &OPTIONS;
#endif
  if (global != NULL) *global = G;
  if (utils != NULL) *utils = U;
}



void load_utilsoptions(utilsoption_type *S, int local) {
  int params[N_UTILS_PARAM];
  Ext_params_utilsoption(local, params);
  S->solve.n_pivot_idx = params[PIVOT_IDX_N];
  S->solve.pivot_idx = S->solve.n_pivot_idx == 0 ? NULL
    : (int*) MALLOC(S->solve.n_pivot_idx * sizeof(*(S->solve.pivot_idx)));
  Ext_get_utilsoption(S, local);
}



void options_localcopy(KEY_type *KT, option_type *options);
void options_copy(KEY_type *KT, bool keep_messages) {
  const bool local = false;
  option_type *options;
  WhichOptionList(local, &options, NULL);
  
  messages_options m;
  if (keep_messages)
    MEMCOPY(&m, &(KT->global.messages), sizeof(messages_options));

  MEMCOPY(&(KT->global), options, sizeof(option_type));
// pointer auf NULL setzten
  if (keep_messages)
    MEMCOPY(&(KT->global.messages), &m, sizeof(messages_options));

  options_localcopy(KT, options);
  load_utilsoptions(&(KT->global_utils), false);
}


void options_localdel(KEY_type *KT);
void options_delete(KEY_type *KT) {
  options_localdel(KT);
  Ext_del_utilsoption(&(KT->global_utils));
}


void FREElocal(KEY_type  *KT);
void FREEglobal() {
  KEY_type *KT = KEYT_LOCAL(); 
  FREE(KT->ToIntDummy);
  KT->ToIntN = 0;
  FREElocal(KT);
}



//Uint startRFU();
void startUtils() {
#if defined compatibility_to_C_h
  startRFU();
#else
    install_default();
#endif  
  includeXport();
  MEMSET(PIDKEY_LOCAL, 0, PIDMODULUS * sizeof(KEY_type *));
}




void KEY_type_init(KEY_type *KT) {
  // ACHTUNG!! setzt nur die uninteressanten zurueck. Hier also gar ncihts.
  KT->next = NULL; // braucht es eigentlich nicht
  KT->doshow = true;
  KT->ToIntDummy = NULL;
  KT->ToIntN = 0;
  options_copy(KT, false);
}

void KEY_type_all_delete(KEY_type **S) {
  KEY_type *KT = *S;
  options_delete(KT);
  FREEglobal();
  UNCONDFREE(*S);
}



void PIDKEY_LOCAL_DELETE() {
  for (int kn=0; kn<PIDMODULUS; kn++) {
    KEY_type *KT = PIDKEY_LOCAL[kn];
    while (KT != NULL) {
      KEY_type *q = KT;
      KT = KT->next;
      KEY_type_all_delete(&q);
    }
    PIDKEY_LOCAL[kn] = NULL;
  }
}



KEY_type *KEYT_LOCAL() {
  int mypid;
  Ext_pid(&mypid);
   KEY_type *p = PIDKEY_LOCAL[mypid % PIDMODULUS];
  //   printf("%d %d %ld\n", mypid,  PIDMODULUS, p);
  if (p == NULL) {
     KEY_type *neu = (KEY_type *) XCALLOC(1, sizeof(KEY_type));
     assert(neu != NULL);

    PIDKEY_LOCAL[mypid % PIDMODULUS] = neu;
    neu->visitingpid = mypid;    
    if (PIDKEY_LOCAL[mypid % PIDMODULUS] != neu) { // another process had same idea
      FREE(neu);
      return KEYT_LOCAL(); // ... and try again
    }
    neu->pid = mypid;
    neu->visitingpid = 0;
    neu->ok = true;
    if (PIDKEY_LOCAL[mypid % PIDMODULUS] != neu) BUG;
    KEY_type_init(neu);       
    return neu;
  }

   
  while (p->pid != mypid && p->next != NULL) {
      //    printf("pp = %d\n", p->pid);
    p = p->next;
  }
  //  printf("pp m = %d %d\n", p->pid, mypid);
  if (p->pid != mypid) {
    if (!p->ok || p->visitingpid != 0) {
      PRINTF("pid collision %d %d\n",  p->ok, p->visitingpid);
      BUG;
      return KEYT_LOCAL();
    }
    p->visitingpid = mypid;
    p->ok = false;
    if (p->visitingpid != mypid || p->ok) {
      return KEYT_LOCAL();
    }
   KEY_type *neu = (KEY_type *) XCALLOC(1, sizeof(KEY_type));
   neu->pid = mypid;
    if (!p->ok && p->visitingpid == mypid) {
      p->next = neu;
      p->visitingpid = 0;
      p->ok = true;      
      return neu;
    }
    FREE(neu);
    p->visitingpid = 0;
    p->ok = true;
    KEY_type_init(neu); 
   return KEYT_LOCAL();
  }
   return p;
}




#if defined compatibility_to_R_h  


void finalizeLoptions(int local) {  
  if (!local) {
    //    option_type *options;
    //   utilsoption_type *utils;
    // WhichOptionList(local, &options, &utils);
  }
}


void setoptionsRFUlocal(int i, int j, SEXP el, char name[LEN_OPTIONNAME], 
		       bool isList, bool local) {
   utilsoption_type *utils;
   WhichOptionList(local, NULL, &utils);
   if (!local && Ext_parallel()) 
     ERR1("Option '%.25s' can be set only through 'RFoptions' at global level",
	  allLoptions[i][j]);
   Ext_setoptionsRFU(i, j, el, name, isList, utils);
}


void getoptionsRFUlocal(SEXP sublist, int i, bool local) {  
  //printf("get local RFU %d\n", i);
    utilsoption_type *utils;
   WhichOptionList(local, NULL, &utils);
   Ext_getoptionsRFU(sublist, i, utils);
}


Uint startLocal(int cores);


#ifdef __cplusplus
extern "C" {
#endif
  
void detachoptions() {
  PIDKEY_LOCAL_DELETE();
  Ext_detachRFUoptions(prefixLlist, prefixLN);
}

  
SEXP LocalOptions(SEXP options) {
  return Ext_RFUoptions(options, (char*) pkg);
}

SEXP copyoptions() {
  KEY_type *KT = KEYT_LOCAL();
  options_copy(KT, true);
  return R_NilValue;
}  

void loadoptions(int *n) {   
#if ! defined DO_PARALLEL
  Uint simd_info =  startLocal(1);
#else
  Uint simd_info = startLocal(*n != NA_INTEGER && *n > 0 ? *n : 1)
#endif
   
  Ext_attachRFUoptions((char *) pkg, prefixLlist, prefixLN,
		       allLoptions, allLoptionsN,
		       setLoptions, getLoptions, finalizeLoptions, NULL,
		       setoptionsRFUlocal, getoptionsRFUlocal,
		       -10, false,
		       GPU_NEEDS, // from configure.ac
		       simd_info,
		       LOCAL_VERSION,
		       RFU_VERSION,
		       MEMisALIGNED_NaN
		       );
  // Ext_attachSetNGet((char*) "miraculix", (char *) "RandomFieldsUtils",
  //		    setoptionsRFU, getoptionsRFU);
  finalizeLoptions(false);
}


#ifdef __cplusplus
}
#endif


#endif
