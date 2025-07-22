/*
 Authors 
 Martin Schlather, martin.schlather@uni-mannheim.de

 Copyright (C) 2016-2023 Martin Schlather

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




// ACHTUNG: Reihenfolge nicht aendern!
//#include "intrinsics.h"
//
#include "Basic_RandomFieldsUtils.h"
//#include "zzz_RFU.h"
//#include "chaploblocker.h"
#include "options.h"
//#include "errors_messages.h"
//#include "kleinkram.h"

// MMaintainer: Torsten Pook <torsten.pook@uni-goettingen.de>



const char * prefixLlist[prefixLN] = {"blocker", "blocker_messages"};


// IMPORTANT: all names of general must be at least 3 letters Long !!!
const char *blocker[blockerN] = {"(unused)" };
const char *messages[messagesN] = {"(none)"}; 
  

option_type OPTIONS_LOCAL = {
blocker_START,
  messages_START
};


const char **allLoptions[prefixLN] = {blocker, messages};
int allLoptionsN[prefixLN] = {blockerN, messagesN};

