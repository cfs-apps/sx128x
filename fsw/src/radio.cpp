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
**    Implement the Transmit Demo Class methods
**
**  Notes:
**    1. Refactored Stanford's Lora_tx.cpp and Lora_rx.cpp functions into 
**       this object that runs within the context of a cFS Basecamp library.
**       This object provides demo configuration and operations commands and
**       uses a child task
**       to manage a transfer. 
**    2. Bridges SX128X C++ library and the main app and Basecamp's app_c_fw
**       written in C.  
**
*/

/*
** Include Files:
*/

#include <string.h>
#include "SX128x_Linux.hpp"
extern "C"
{
   #include "sx128x_lib.h"
   #include "radio.h"
}

/**********************/
/** Global File Data **/
/**********************/

// Pins based on hardware configuration
SX128x_Linux *Radio = NULL;


/*******************************/
/** Local Function Prototypes **/
/*******************************/

/******************************************************************************
** Function: RADIO_Constructor
**
** Initialize the Radio object to a known state
**
** Notes:
**   1. This must be called prior to any other function.
**
*/
bool RADIO_Constructor(const char *SpiDevStr, uint8_t SpiDevNum, const RADIO_Pin_t *RadioPin)
{
   bool RetStatus = false;
   
   SX128x_Linux::PinConfig PinConfig;
   
   PinConfig.busy  = RadioPin->Busy;
   PinConfig.nrst  = RadioPin->Nrst;
   PinConfig.nss   = RadioPin->Nss;
   PinConfig.dio1  = RadioPin->Dio1;
   PinConfig.dio2  = RadioPin->Dio2;
   PinConfig.dio3  = RadioPin->Dio3;
   PinConfig.tx_en = RadioPin->TxEn;
   PinConfig.rx_en = RadioPin->RxEn;
   
   try
   {
      Radio = new SX128x_Linux(SpiDevStr, SpiDevNum, PinConfig);
      RetStatus = true;
   }
   catch (...)
   {
      RetStatus = false;
   }
   
   return RetStatus;
   
} /* End RADIO_Constructor() */


/******************************************************************************
** Function: RADIO_SetLowNoiseAmpMode
**
** Set the radio power amp sensitivity mode
**
** Notes:
**   None
**
*/
bool RADIO_SetLowNoiseAmpMode(uint16_t LowNoiseAmpMode)
{
      
   bool RetStatus = false;
   
   if (SX128X_Initialized())
   {  
      Radio->SetLNAGainSetting(SX128x::RadioLnaSettings_t(LowNoiseAmpMode));
      RetStatus = true;
   }
   return RetStatus;
   
} /* RADIO_SetLowNoiseAmpMode() */


/******************************************************************************
** Function: RADIO_SetModulationParams
**
** Set the radio Lora parameters
**
** Notes:
**   None
**
*/
bool RADIO_SetModulationParams(uint8_t SpreadingFactor,
                               uint8_t Bandwidth,
                               uint8_t CodingRate)
{
   
   bool RetStatus = false;
   
   if (SX128X_Initialized())
   {
      SX128x::ModulationParams_t ModulationParams;
      
      ModulationParams.PacketType                  = SX128x::PACKET_TYPE_LORA;
      ModulationParams.Params.LoRa.CodingRate      = (SX128x::RadioLoRaCodingRates_t)CodingRate;
      ModulationParams.Params.LoRa.Bandwidth       = (SX128x::RadioLoRaBandwidths_t)Bandwidth;
      ModulationParams.Params.LoRa.SpreadingFactor = (SX128x::RadioLoRaSpreadingFactors_t)SpreadingFactor;

      Radio->SetModulationParams(ModulationParams);
      RetStatus = true;
   }
   
   return RetStatus;
   
} /* RADIO_SetModulationParams() */


/******************************************************************************
** Function: RADIO_SetPowerAmpRampTime
**
** Set the radio power amp ramp time
**
** Notes:
**   None
**
*/
bool RADIO_SetPowerAmpRampTime(uint16_t PowerAmpRampTime)
{
   
   bool RetStatus = false;
   
   if (SX128X_Initialized())
   {
      Radio->SetTxParams(0, SX128x::RadioRampTimes_t(PowerAmpRampTime));
      RetStatus = true;
   }
   return RetStatus;
   
} /* End RADIO_SetPowerAmpRampTime() */


/******************************************************************************
** Function: RADIO_SetPowerRegulatorMode
**
** Set the radio power regulator mode mode
**
** Notes:
**   None
**
*/
bool RADIO_SetPowerRegulatorMode(uint16_t PowerRegulatorMode)
{
   
   bool RetStatus = false;
   
   if (SX128X_Initialized())
   {
      Radio->SetRegulatorMode(static_cast<SX128x::RadioRegulatorModes_t>(PowerRegulatorMode));
      RetStatus = true;
   }
   return RetStatus;
   
} /* End RADIO_SetPowerRegulatorMode() */


/******************************************************************************
** Function: RADIO_SetRadioFrequency
**
** Set the radio frequency
**
** Notes:
**   1. Assumes frequency (Hz) value has been validated 
**
*/
bool RADIO_SetRadioFrequency(uint32_t Frequency)
{
   
   bool RetStatus = false;
   
   if (SX128X_Initialized())
   {
      Radio->SetRfFrequency(Frequency);
      RetStatus = true;
   }
   return RetStatus;
   
} /* End RADIO_SetRadioFrequency() */


/******************************************************************************
** Function: RADIO_SetSpiSpeed
**
** Set the SPI speed
**
** Notes:
**   1. Not intended to be a ground command and assumes the Radio has been
**      initialized and speed value has been validated 
**
*/
bool RADIO_SetSpiSpeed(uint32_t SpiSpeed)
{
   
   Radio->SetSpiSpeed(SpiSpeed);
   
   return true;
   
} /* End RADIO_SetSpiSpeed() */


/******************************************************************************
** Function: RADIO_SetStandbyMode
**
** Set the radio standby mode
**
** Notes:
**   None
**
*/
bool RADIO_SetStandbyMode(uint16_t StandbyMode)
{
   
   bool RetStatus = false;
   
   if (SX128X_Initialized())
   {
      Radio->SetStandby(SX128x::RadioStandbyModes_t(StandbyMode));
      RetStatus = true;
   }
   return RetStatus;
   
} /* End RADIO_SetStandbyMode() */
