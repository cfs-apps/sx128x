/*
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as
**  published by the Free Software Foundation, either version 3 of the
**  License, or (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
**
**  Purpose:
**    Entry point function for SX128X library
**
*/

/*
** Includes
*/

#include "cfe.h"

/*
** The major and minor version numbers are release versions that are
** tagged in github. PLATFORM_REV is always released as zero and is 
** used to track changes made for a specific platform.
**
**  Version:
**    1.0.0 - Initial release
**
*/

#define SX128X_MAJOR_VER     1
#define SX128X_MINOR_VER     0
#define SX128X_PLATFORM_REV  0


/*
** Exported Functions
*/

/******************************************************************************
** Entry function
**
*/
uint32 SX128X_LibInit(void)
{

   OS_printf("SX128X Library Initialized. Version %d.%d.%d\n",
             SX128X_MAJOR_VER, SX128X_MINOR_VER, SX128X_PLATFORM_REV);
   
   return OS_SUCCESS;

} /* End SX128X_LibInit() */

