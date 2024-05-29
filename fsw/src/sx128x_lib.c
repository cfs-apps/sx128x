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

#include "lib_cfg.h"
#include "sx128x_lib.h"
#include "radio.h"

/***********************/
/** Macro Definitions **/
/***********************/

/* Convenience macros */
#define  INITBL_OBJ   (&IniTbl)


/**********************/
/** File Global Data **/
/**********************/

/* 
** Must match DECLARE ENUM() declaration in lib_cfg.h
** Defines "static INILIB_CfgEnum IniCfgEnum"
*/

DEFINE_ENUM(Config, LIB_CONFIG)

/**********************/
/** File Global Data **/
/**********************/

static INITBL_Class_t     IniTbl;
static SX128X_LIB_Class_t Sx128xLib;

/*******************************/
/** Local Function Prototypes **/
/*******************************/

static bool InitRadio(void);


/******************************************************************************
** Entry function
**
** Notes:
**   1. This is called by the cFE during lib/app loading and initialization and
**      serves as the object constructor in the Basecamp design model.
**   2. RetStatus is used to notify the cFE if there was a critical error 
**      loading the library. If this happens then the symbols required by the
**      app using the library will fail to load because the symbols won't be
**      defined. Sx128xLib.Initialized indicates to the app whether the library
**      is ready for use by an app.  
*/
uint32_t SX128X_LibInit(void)
{

   uint32 RetStatus = OS_SUCCESS; 
   
   memset((void*)&Sx128xLib, 0, sizeof(SX128X_LIB_Class_t));
   
   /* IniTbl will report errors */
   if (INITBL_Constructor(INITBL_OBJ, SX128X_LIB_INI_FILENAME, &IniCfgEnum))
   {
      
      if (InitRadio())
      {
         Sx128xLib.Initialized = true;
         OS_printf("SX128X Library Initialized. Version %d.%d.%d\n",
                   SX128X_LIB_MAJOR_VER, SX128X_LIB_MINOR_VER, SX128X_LIB_PLATFORM_REV);
      }
      else
      {
         OS_printf("Error creating SX128X Library radio object. Version %d.%d.%d\n",
                   SX128X_LIB_MAJOR_VER, SX128X_LIB_MINOR_VER, SX128X_LIB_PLATFORM_REV);         
      }
   }
   else
   {

      // Keep library loaded: RetStatus = OS_ERROR;
      Sx128xLib.Initialized = false;

      OS_printf("Error loading SX128X Library JSON initialization parameters. Version %d.%d.%d\n",
                SX128X_LIB_MAJOR_VER, SX128X_LIB_MINOR_VER, SX128X_LIB_PLATFORM_REV);
            
   }
   
   return RetStatus;

} /* End SX128X_LibInit() */


/******************************************************************************
** Function: SX128X_Initialized
**  
*/
bool SX128X_Initialized(void)
{
   return Sx128xLib.Initialized;
}


/******************************************************************************
** Function: InitRadio
**
*/
static bool InitRadio(void)
{
   
   bool RetStatus = false;
   RADIO_Pin_t RadioPin;

   RadioPin.Busy = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_BUSY);
   RadioPin.Nrst = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_NRST);
   RadioPin.Nss  = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_NSS);
   RadioPin.Dio1 = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_DIO1);
   RadioPin.Dio2 = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_DIO2);
   RadioPin.Dio3 = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_DIO3);
   RadioPin.TxEn = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_TX_EN);
   RadioPin.RxEn = INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_PIN_RX_EN);
   
   RetStatus = RADIO_Constructor(INITBL_GetStrConfig(INITBL_OBJ, CFG_RADIO_SPI_DEV_STR),
                                 INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_SPI_DEV_NUM),
                                 &RadioPin);
   
   if (RetStatus)
   {
      RADIO_SetSpiSpeed(INITBL_GetIntConfig(INITBL_OBJ, CFG_RADIO_SPI_SPEED));
   }
   
   return RetStatus;
   
} /* InitRadio() */

