/** 
 * @file        rgb-led-lighting-shield-pal-i2c-psoc.hpp
 * @brief       RGB LED LIGHTING SHIELD I2C Platform Abstraction Layer PSoC
 * @date        November 2020
 * @copyright   Copyright (c) 2020 Infineon Technologies AG
 * 
 * SPDX-License-Identifier: MIT
 */

#ifndef RGB_LED_LIGHTING_SHIELD_PAL_I2C_PSOC_HPP_
#define RGB_LED_LIGHTING_SHIELD_PAL_I2C_PSOC_HPP_

#include "rgb-led-lighting-shield-conf.hpp"

#if (RGB_LED_LIGHTING_SHIELD_FRAMEWORK == RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6)

#include "rgb-led-lighting-shield-pal-i2c.hpp"
#include "rgb-led-lighting-shield-reg.hpp"
#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include "string.h"

/**
 * @addtogroup mtbPal
 * @{
 */

using namespace ledDali;

class I2CPALPSoC : virtual public I2CPAL
{
    public:
        I2CPALPSoC();
        ~I2CPALPSoC();
        Error_t init();
        Error_t deinit();
        Error_t setClockFreq(uint32_t clockHz);
        Error_t read(uint16_t addr, uint8_t command, uint16_t *data, uint16_t length);
        Error_t read(uint16_t addr, uint8_t command, uint16_t *data);
        Error_t write(uint16_t addr, uint8_t command, uint16_t *data, uint16_t length);
        Error_t write(uint16_t addr, uint8_t command, uint16_t data);
        Error_t writeDMX(uint16_t addr, uint8_t command);

    private:
        void convertData(uint8_t command, uint16 *buffer, uint16_t length, uint8_t *result);
        uint16_t    		 _i2cFreq = 100000;
        cyhal_i2c_t 		_I2C;
        cyhal_i2c_cfg_t 	_I2C_cfg;
};

/**
 * @}
 */

#endif /** RGB_LED_LIGHTING_SHIELD_FRMWK_PSOC6 **/
#endif /** RGB_LED_LIGHTING_SHIELD_PAL_I2C_PSOC_HPP_ **/
