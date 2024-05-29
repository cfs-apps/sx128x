/*
**  Copyright 2022 bitValence, Inc.
**  All Rights Reserved.
**
**  This program is free software; you can modify and/or redistribute it
**  under the terms of the GNU Affero General Public License
**  as published by the Free Software Foundation; version 3 with
**  attribution addendums as found in the LICENSE.txt
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Affero General Public License for more details.
**
**  Purpose:
**    Define the SX128x Object
**
**  Notes:
**    2. This can't include cFE or Basecamp headers
**
*/
#ifndef _sx128x_lib_
#define _sx128x_lib_

/*
** Includes
*/

#include <stdint.h>
#include <stdbool.h>

/***********************/
/** Macro Definitions **/
/***********************/


/**********************/
/** Type Definitions **/
/**********************/


typedef struct
{

   bool Initialized;
   
} SX128X_LIB_Class_t;


/************************/
/** Exported Functions **/
/************************/

/******************************************************************************
** Function: SX128X_LibInit
**
** Initialize the SX128X library to a known state
**
** Notes:
**   1. This is called by the cFE during lib/app loading and initialization and
**      serves as the object constructor in the Basecamp design model.
**
*/
uint32_t SX128X_LibInit(void);


/******************************************************************************
** Function: SX128X_Initialized
**  
*/
bool SX128X_Initialized(void);


#endif /* _sx128x_lib_ */
