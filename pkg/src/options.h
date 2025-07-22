
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



#ifndef CHaploBlocker_options_H
#define CHaploBlocker_options_H 1 


#include "xport_import.h"
//#include "zzz_RFU.h"


#define blockerN 1
typedef struct haploblocker_options {
  double ANY_diff_value;
} blocker_options;

#define blocker_START { 0.0 }

#define messagesN 1
extern const char * messages[messagesN];
struct messages_options{ 
  bool warn_dummy;
};
#define messages_START {			\
    true					\
      }


typedef struct option_type {
  blocker_options blocker;
  messages_options messages;
} option_type;
extern option_type OPTIONS_LOCAL;


#define prefixLN 2
extern const char * prefixLlist[prefixLN], **allLoptions[prefixLN];
extern int allLoptionsN[prefixLN];

typedef
struct KEY_type KEY_type;
struct KEY_type {
 KEY_type_default;
};
KEY_type *KEYT_LOCAL();


#endif
