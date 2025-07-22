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



#include "def_rfu.h"
#include "intrinsics.h"
#if defined MSDOS_WINDOWS
//#define VC_EXTRALEAN
#include <windows.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <assert.h>


#define VARIABLE_IS_NOT_USED __attribute__ ((unused))


// achtung! windows.h zusammen mit <Rmath.h oder R.graphics>
// gibt warnung, da ERROR mehrfach definiert !
// deshalb auch in auxiliary.h nicht basic.h einbinden // obsolette ?!!
#if defined compatibility_to_R_h
#include <Rinternals.h>
#include "RandomFieldsUtils.h"
#include "Basic_RandomFieldsUtils.h"
#endif
#include "win_linux_aux.h"

#if !defined BUG
#if defined MSDOS_WINDOWS
#define BUG  perror("unrecognized OS in cpuid"); // OK
#else
#define BUG  error("unrecognized OS in cpuid"); // OK
#endif
#endif


 
void sleepMilli(int *milli) {
#if defined MSDOS_WINDOWS
  Sleep((long) *milli); // along
#else 
  usleep((useconds_t) (1000 * (unsigned long) *milli));// along
#endif
}

void sleepMicro(int *micro) {
#if defined MSDOS_WINDOWS
  Sleep((long) ((*micro + 500) / 1000));// along
#else
  usleep((useconds_t) *micro);
#endif
}

void pid(int *i)  {
#if defined MSDOS_WINDOWS
  *i = _getpid();
#else
  *i = getpid(); 
#endif
}

int parentpid=0;
bool
  parallel() {
    int mypid;
    pid(&mypid);
    return mypid != parentpid;
  }


void hostname(char **h, int *i){
#if defined MSDOS_WINDOWS
  *h[0]=0;
#else
  gethostname(*h, *i);
#endif
}


// see for a table s
// https://www.felixcloutier.com/x86/cpuid
// local file "CPUID -- CPU Identification.html" on ~/svn
uint32_t cpuid_info(unsigned int VARIABLE_IS_NOT_USED Blatt,
		    unsigned int VARIABLE_IS_NOT_USED Register) {
#if defined MINGWCPUID
   __cpuid(Blatt, s[0], s[1], s[2], s[3]);B
   #if defined SCHLATHERS_MACHINE
     unsigned int t[4];							
     if (!__get_cpuid(Blatt, t, t+1, t+2, t+3)) {				
       PRINTF("unallowed cpuid access. %.80s", CONTACT);			
       BUG;
     }
     if ((s[0] != t[0] || s[1] != t[1] || s[2] != t[2] || s[3] != t[3])) { 
       /* __get_cpuid does not seem to work for certain registers */	
       /* indeed results may differ (14 Jan 2022) ! */			
        uint32_t u[4];							
	asm volatile							
	  ("cpuid": "=a"(u[0]), "=b"(u[1]),"=c"(u[2]),			
	   "=d"(u[3]):"a"(Blatt),"c"(0));				
	PRINTF("%u %u %u %u\n%u %u %u %u\n%u %u %u %u\n",		
	       u[0],u[1],u[2],u[3],					
	       t[0],t[1],t[2],t[3],					
	       s[0],s[1],s[2],s[3]);					
	if ((s[0] != t[0] || s[1] != t[1] || s[2] != t[2] || s[3] !=t[3])) BUG; 
									     
     }	      			
   #endif
   return s[Register];
#elif defined WINCPUID
  uint32_t s[4];							
  __cpuid((int *)s, (int) Blatt);
  return s[Register];
#elif defined LINUXCPUID
  uint32_t s[4];							
  asm volatile							
    ("cpuid": "=a"(s[0]), "=b"(s[1]),"=c"(s[2]),			
     "=d"(s[3]):"a"(Blatt),"c"(0));					
  return s[Register]; 
#else
   return 0;
#endif
}



uint32_t cpuid_info(unsigned int VARIABLE_IS_NOT_USED Blatt,
		    unsigned int VARIABLE_IS_NOT_USED Register,
		    unsigned int VARIABLE_IS_NOT_USED ecx_value) {
#if defined LINUXCPUID
  uint32_t s[4];
  asm volatile							
    ("cpuid": "=a"(s[0]), "=b"(s[1]),"=c"(s[2]),			
     "=d"(s[3]):"a"(Blatt),"c"(ecx_value));					
  return s[Register]; 
#else
  BUG;
   return 0;
#endif
}


void cpuid_info_all(unsigned int VARIABLE_IS_NOT_USED Blatt,
		    unsigned int VARIABLE_IS_NOT_USED ecx_value,
		    uint32_t s[4]) {
#if defined LINUXCPUID
  asm volatile							
    ("cpuid": "=a"(s[0]), "=b"(s[1]),"=c"(s[2]),			
     "=d"(s[3]):"a"(Blatt),"c"(ecx_value));					
#else
  BUG;
#endif
}


// see https://www.felixcloutier.com/x86/cpuid
#if defined compatibility_to_C_h
#define debug_cpuid_info 0
#endif
void cpuid_info(int apic[maxIdDomains],  int logical_units[maxIdDomains],
	        int *total_domains, int *ground_freq, int *max_freq,
		bool *hyperthreader) {
  uint32_t maxleaf = cpuid_info(0x80000000, EAX,  0);
  uint32_t info;
  uint32_t info4[4];

  // CPU frequencies
  info = cpuid_info(0x16, EAX);
  *ground_freq = info & 0xFFFF;
  info = cpuid_info(0x16, EBX,  0);
  *max_freq = info & 0xFFFF;
  
#if defined compatibility_to_C_h
  if (debug_cpuid_info)
    printf("ground %u max %u maxleaf%s%u ",  // Rprint
	   *ground_freq, *max_freq,
	   maxleaf > 99 ? ">" : "=",  maxleaf < 99 ? maxleaf : 99);
#endif
  
  
  /*
  for (int j=0; j<=3; j++) {
    cpuid_info_all(0x00, j, info4);
    char *x = (char*) info4;
    for (int i=0; i<16; i++) p rintf("%d %c; ", x[i], x[i]);
    p rintf("\n");
  }
  */
    

 /*
  cpuid_info_all(0x04, 30, info4);
  p rintf("info %u %u %u %u\n", info4[0], info4[0], info4[2], info4[3] );
  uint32_t cache0 = cpuid_info(0x04, EBX, 0);
   uint32_t cache1 = cpuid_info(0x04, ECX, 1);
  */

  /*
  uint32_t hyperthread = (cpuid_info(0x01, EDX) >> 28) & 0x01;
   p rintf("hyper %u\t", hyperthread);

   uint32_t max_logical_cores = (cpuid_info(0x01, EBX) >> 16) & 0xFF;
   p rintf("log.cores %u\n", max_logical_cores);
  */


  // CPU internal number & number of logical u n i t s on same & higher level
  uint32_t readable = maxleaf; 
  if (readable < 0x1F && readable >= 0x0B &&
      (cpuid_info(0x0B, ECX) & 0xFFFF) == 0) readable=0;
  if (readable < 0x0B) {
    *total_domains = 1;
    logical_units[0] = 1;
    apic[0] = cpuid_info(0x01, EBX) >> 24;
    *hyperthreader = false; // not detectable
 #if defined compatibility_to_C_h   
    if (debug_cpuid_info) printf("hyperthreading not detectable\n");// Rprint
#endif 
  } else {
    readable = readable >= 0x1F ? 0x1F : 0x0B;
    uint32_t apicid[maxIdDomains + 1];
    uint32_t shift[maxIdDomains + 1] = {0};
    while (true) {
      *total_domains = readable == 0x0B ? 2 : maxIdDomains;      
      int i;
      for(i = 0; i<*total_domains; i++) {
	cpuid_info_all(readable, i, info4);
	int domain = (info4[ECX] >> 8) & 0xFF,
	 log_units = info4[EBX] & 0xFFFF,
	 shft = info4[EAX] & 0xF,
	 idx = domain -1;
        if (readable == 0x0B && info4[EBX] && 0xFFFF == 0) break;
       if (readable == 0x1F && !domain) break; // the killer
       //printf("%d: %u %u %u %u -> %d %d %d %d\n", i, info4[0], info4[1], info4[2], info4[3], shft, domain, log_units, idx);
      logical_units[idx] = log_units; // in the current domain
       apicid[idx] = info4[EDX];
       shift[idx+1] = shft;
       assert(shift[idx+1]);
      }
      *total_domains = i;
      if (*total_domains == 0) {
#if defined compatibility_to_C_h
	if (debug_cpuid_info)
	  printf("no domain detected at %d\n", readable);// Rprint
#endif	
	//   printf("tot=%u\n", *total_domains);
 	if (readable == 0x1F) {
	  readable = 0x0B;
	  continue;
	}	
      }
      for(i = 0; i<*total_domains; i++) {
	apic[i] = apicid[i] >> shift[i];      
#if defined compatibility_to_C_h
	if (debug_cpuid_info)
	  printf("i=%d log=%u apic=%u\t", i,logical_units[i], apic[i]);// Rprint
#endif
      }
    
      *hyperthreader = *total_domains >= 2 && logical_units[0] > 1 &&
	apic[0] != apic[1] << shift[1];
      break;
    }
    //    printf("%d %d %d %d\n", *hyperthreader, logical_units[0], apic[0], (uint32_t) (apic[1] << shift[1]));
  }
#if defined compatibility_to_C_h
  if (debug_cpuid_info) printf("\n"); // Rprint
#endif
}

void cpuid_info_freq(int *ground_freq, int *max_freq, bool *hyperthreader) {
  int apic[maxIdDomains], logical_units[maxIdDomains], total_domains;
  cpuid_info(apic, logical_units, &total_domains, ground_freq, max_freq,
	     hyperthreader);
}

