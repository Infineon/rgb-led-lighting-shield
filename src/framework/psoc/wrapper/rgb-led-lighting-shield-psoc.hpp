/** 
 * @file        rgb-led-lighting-shield-psoc.hpp
 * @brief       RGB LED LIGHTING SHIELD Library KIT PSoC
 * @date        November 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef RGB_LED_LIGHTING_SHIELD_PSOC_HPP_
#define RGB_LED_LIGHTING_SHIELD_PSOC_HPP_

#include "rgb-led-lighting-shield-conf.hpp"

#if (RGB_LED_LIGHTING_SHIELD_FRAMEWORK == RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6)

#include "rgb-led-lighting-shield.hpp"
#include "rgb-led-lighting-shield-pal-i2c-psoc.hpp"

/**
 * @addtogroup mtbApi
 * @{
 */

class RGBShieldPSoC : virtual public RGBShield
{
	public:
		RGBShieldPSoC();
		~RGBShieldPSoC();
};

/**
 * @}
 */

#endif /** RGB_LED_LIGHTING_SHIELD_FRAMEWORK == RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6 */
#endif /** RGB_LED_LIGHTING_SHIELD_PSOC_HPP_ **/
