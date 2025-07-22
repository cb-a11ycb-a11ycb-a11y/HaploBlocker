
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


#if !defined compatibility_to_R_h
#include <random>
#include <stdarg.h>
#endif


#include "def.h"
#include "Basic_RandomFieldsUtils.h"
#include "options_RFU.h"
#include "errors_messages.h"


#if defined compatibility_to_C_h

double ownNA = 0.0,
  ownNaN = 0.0;


double gauss_random(double mu, double sigma) {
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::normal_distribution<> d{mu,sigma};
  return (d(gen));
}

double uniform_random(double a, double b) {
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_real_distribution<> d{a, b};
  return (d(gen));
}

double poisson_random(double lambda) {
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::poisson_distribution<> d{lambda};
  return (d(gen));
}

#endif


int stopIfNotIntI(Long i, Long line, const char *file) {
  if (i > MAXINT || i < -MAXINT)
    ERR3("value (%ld) not an integer at line %ld in %s\n",
	 (long) i, (long) line, file); // long OK
  return (int) i;
}
  

Uint stopIfNotUIntI(Long i, Long line, const char *file) {
  if (i > UINT_MAX || i < 0)
    ERR3("value (%ld) not an unsigned integer at line %ld in %s\n",
	 (long) i, (long)line, file); // long OK
  return (Uint) i;
}
  

int stopIfNotAnyIntI(Long i, Long line, const char *file) {
  // INT_MIN == NA !
  if (i > INT_MAX || i < INT_MIN)
    ERR3("value (%ld) not an integer at line %ld in %s\n",
	 (long) i, (long) line, file); // long OK
  return (int) i;
}
  


void *notNull(void *X, int line, const char *file) {
  if (X!=NULL) return X;
  ERR2("No memory was allocated at line %d of %s.\n", line, file);
  return NULL;
}
