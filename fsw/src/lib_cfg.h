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
**    Define configurations for the SX128X Library
**
**  Notes:
**    1. These macros can only be built with the library and can't
**       have a platform scope because the same file name is used for
**       all libs/apps following the object-based application design.
**    2. This must be named lib_cfg.h and not app_cfg.h because apps
**       need to add the library sourre directory to their include
**       search path and they'd get a conflict with app_cfg.h.
**
*/
#ifndef _lib_cfg_
#define _lib_cfg_

/*
** Includes
*/

#include "app_c_fw.h"
#include "sx128x_lib_platform_cfg.h"

/******************************************************************************
** SX128X Application Macros
*/

/*
** Versions:
**
** 0.9 - Prototype until a minimal version is working with hardware 
*/

#define  SX128X_LIB_MAJOR_VER   0
#define  SX128X_LIB_MINOR_VER   9


/******************************************************************************
** JSON init file definitions/declarations.
**    
*/

#define CFG_RADIO_SPI_DEV_STR  RADIO_SPI_DEV_STR
#define CFG_RADIO_SPI_DEV_NUM  RADIO_SPI_DEV_NUM
#define CFG_RADIO_SPI_SPEED    RADIO_SPI_SPEED
#define CFG_RADIO_PIN_BUSY     RADIO_PIN_BUSY
#define CFG_RADIO_PIN_NRST     RADIO_PIN_NRST
#define CFG_RADIO_PIN_NSS      RADIO_PIN_NSS
#define CFG_RADIO_PIN_DIO1     RADIO_PIN_DIO1
#define CFG_RADIO_PIN_DIO2     RADIO_PIN_DIO2
#define CFG_RADIO_PIN_DIO3     RADIO_PIN_DIO3
#define CFG_RADIO_PIN_TX_EN    RADIO_PIN_TX_EN   
#define CFG_RADIO_PIN_RX_EN    RADIO_PIN_RX_EN

#define LIB_CONFIG(XX) \
   XX(RADIO_SPI_DEV_STR,char*) \
   XX(RADIO_SPI_DEV_NUM,uint32) \
   XX(RADIO_SPI_SPEED,uint32) \
   XX(RADIO_PIN_BUSY,uint32) \
   XX(RADIO_PIN_NRST,uint32) \
   XX(RADIO_PIN_NSS,uint32) \
   XX(RADIO_PIN_DIO1,uint32) \
   XX(RADIO_PIN_DIO2,uint32) \
   XX(RADIO_PIN_DIO3,uint32) \
   XX(RADIO_PIN_TX_EN,uint32) \
   XX(RADIO_PIN_RX_EN,uint32)

DECLARE_ENUM(Config,LIB_CONFIG)


#endif /* _lib_cfg_ */
