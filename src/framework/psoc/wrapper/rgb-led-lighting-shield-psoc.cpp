/** 
 * @file        rgb-led-lighting-shield-psoc.cpp
 * @brief       RGB LED LIGHTING SHIELD Library KIT PSoC
 * @date        November 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#include "rgb-led-lighting-shield-psoc.hpp"

#if (RGB_LED_LIGHTING_SHIELD_FRAMEWORK == RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6)

I2CPAL * custom_i2cpal = nullptr;

RGBShieldPSoC::RGBShieldPSoC() : RGBShield((custom_i2cpal = new I2CPALPSoC()))
{

}

RGBShieldPSoC::~RGBShieldPSoC()
{
	if(nullptr != custom_i2cpal)
	{
		delete custom_i2cpal;
		custom_i2cpal = nullptr;
	}
}

#endif /** RGB_LED_LIGHTING_SHIELD_FRAMEWORK = RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6 */
