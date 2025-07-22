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



#ifndef chaploblocker_H
#define chaploblocker_H 1

//
#include <R.h>
#include <Rinternals.h>

//extern "C" {
#ifdef __cplusplus
extern "C" {
#endif
  SEXP fixcoding(SEXP values);
  SEXP codeSNPs(SEXP M, SEXP Start, SEXP RedoCoding, SEXP SNPxIND);
  SEXP decodeSNPs(SEXP CM);
  SEXP factorSNPs(SEXP M, SEXP Start, SEXP End);
  SEXP colSumsEqualSNPs(SEXP  CM, SEXP start, SEXP CV, SEXP Select);
  SEXP intersect(SEXP A, SEXP B);

  void loadoptions(int *n);
  //  SEXP attachoptions(void);
  void detachoptions(void);
  //  SEXP copyoptions(void);

  
#ifdef __cplusplus
}
#endif

#endif /* _chaploblocker_H */

