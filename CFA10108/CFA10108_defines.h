#ifndef __CFA10108_DEFINES_H__
#define __CFA10108_DEFINES_H__
//============================================================================
//
// Definitions specific to the CFA10108 board.
//
// 2020-07-30 Crystalfontz America, Inc. / Brent A. Crosby
//===========================================================================
//This is free and unencumbered software released into the public domain.
//
//Anyone is free to copy, modify, publish, use, compile, sell, or
//distribute this software, either in source code form or as a compiled
//binary, for any purpose, commercial or non-commercial, and by any
//means.
//
//In jurisdictions that recognize copyright laws, the author or authors
//of this software dedicate any and all copyright interest in the
//software to the public domain. We make this dedication for the benefit
//of the public at large and to the detriment of our heirs and
//successors. We intend this dedication to be an overt act of
//relinquishment in perpetuity of all present and future rights to this
//software under copyright law.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//OTHER DEALINGS IN THE SOFTWARE.
//
//For more information, please refer to <http://unlicense.org/>
//============================================================================
//My defines for the clock source
#define EVE_CLOCK_SOURCE_INTERNAL   (0)
#define EVE_CLOCK_SOURCE_EXTERNAL   (1)

//My defines for the external crystal clock (multiplying is hard)
#define EVE_EXTERNAL_CLOCK_MUL_UNUSED     (0x00)
#define EVE_EXTERNAL_CLOCK_MUL_x2_24MHz   (0x02)
#define EVE_EXTERNAL_CLOCK_MUL_x3_36MHz   (0x03)
#define EVE_EXTERNAL_CLOCK_MUL_x4_48MHz   (0x44)
#define EVE_EXTERNAL_CLOCK_MUL_x5_60MHz   (0x45)
#define EVE_EXTERNAL_CLOCK_MUL_x6_72MHz   (0x46)
#define EVE_EXTERNAL_CLOCK_MUL_x7_84MHz   (0x47)

//My defines for the touch devices.
#define EVE_TOUCH_NONE       (0)
#define EVE_TOUCH_RESISTIVE  (1)
#define EVE_TOUCH_CAPACITIVE (2)
#define EVE_CAP_DEV_DEFAULT  (0)
#define EVE_CAP_DEV_GT911    (1)
#define EVE_CAP_DEV_FT5316   (2)
//============================================================================
// These defines describe the board and EVE accelerator.
#define EVE_DEVICE           (BT817)
#define EVE_CLOCK_SOURCE     (EVE_CLOCK_SOURCE_EXTERNAL)
#define EVE_CLOCK_MUL        (EVE_EXTERNAL_CLOCK_MUL_x6_72MHz)
#define EVE_CLOCK_SPEED      ((uint32_t)(EVE_CLOCK_MUL&0x1F)*(uint32_t)12000000)
#define EVE_TOUCH_TYPE       (EVE_TOUCH_CAPACITIVE)
#define EVE_TOUCH_CAP_DEVICE (EVE_CAP_DEV_FT5316)
#define EVE_PEN_UP_BUG_FIX   (0)
//Prototype touch panels return 800x480 productiuon will not need
//calibration.
#define EVE_TOUCH_CAL_NEEDED (1)
// DEBUG_NONE (0K flash), DEBUG_STATUS (~2.8K flash) or DEBUG_GEEK (~3K flash)
#define DEBUG_LEVEL (DEBUG_STATUS)

//You can enable/disable the different demos here.
//There is not enough RAM_G space to hold all of the scrolling background,
//logo, audio, and blue marble at the same time

#define BMP_DEMO             (0)  //Background, uses uSD
#define   BMP_SCROLL         (0)  //1=scrolling background, 0=static image
#define SOUND_DEMO           (0)  //Uses uSD
#define   SOUND_VOICE        (0)  //1=VOI_8K.RAW, 0=MUS_8K.RAW
#define   SOUND_PLAY_TIMES   (10)
#define LOGO_DEMO            (1)  //Rotating logo (image in flash, no uSD)
#define LOGO_PNG_0_ARGB2_1   (1)  //Compressed ARGB is 5614 bytes smaller
#define BOUNCE_DEMO          (1)  //Ball-and-rubber-band demo.
#define MARBLE_DEMO          (0)  //Uses uSD - spinning earth
#define TOUCH_DEMO           (1)
//----------------------------------------------------------------------------
// Turn on uSD code if one of the demos above uses it.
#if ((0 != SOUND_DEMO) || (0 != BMP_DEMO) || (0 != MARBLE_DEMO))
  #define BUILD_SD           (1)
#else
  #define BUILD_SD           (0)
#endif
//----------------------------------------------------------------------------
// Throw an error if touch demo is requested for a non-touch display.
#if (EVE_TOUCH_TYPE == EVE_TOUCH_NONE)
#if (0 != TOUCH_DEMO)
  #error Cannot enable touch demo for a non-touch display.
#endif // (0 != TOUCH_DEMO)
#endif // (EVE_TOUCH_TYPE != EVE_TOUCH_NONE)
//============================================================================
// Manually control the backlight brightness by touching along an axis.
#if (0 != TOUCH_DEMO)
#define MANUAL_BACKLIGHT_DEBUG (0)
#endif // (0 != TOUCH_DEMO)
//============================================================================
// Remotely control the backlight (debug only).
#if (DEBUG_LEVEL != DEBUG_NONE)
#define REMOTE_BACKLIGHT_DEBUG (0)
#endif // (DEBUG_LEVEL != DEBUG_NONE)
//============================================================================
// Test code for Reset_EVE_Coprocessor() (debug only).
#if ((0 != LOGO_DEMO) &&( 1 == LOGO_PNG_0_ARGB2_1))
#define DEBUG_COPROCESSOR_RESET (0)
#endif // ((0 != LOGO_DEMO) &&( 1 == LOGO_PNG_0_ARGB2_1))
//============================================================================
// Wiring for prototypes.
//   ARD      | Port | 10098/EVE           | Color
// -----------+------+---------------------|--------
//  #3/D3     |  PD3 | DEBUG_LED           | Green W/W
//  #7/D7     |  PD7 | EVE_INT             | Purple
//  #8/D8     |  PB0 | EVE_PD_NOT          | Blue
//  #9/D9     |  PB1 | EVE_CS_NOT         | Brown
// #10/D10    |  PB2 | SD_CS_NOT           | Grey
// #11/D11    |  PB3 | MOSI (hardware SPI) | Yellow
// #12/D12    |  PB4 | MISO (hardware SPI) | Green
// #13/D13    |  PB5 | SCK  (hardware SPI) | orange

//Arduino style pin defines
// Interrupt from EVE to Arduino - input, not used in this example.
#define EVE_INT     (7)
// PD_N from Arduino to EVE - effectively EVE reset
#define EVE_PD_NOT  (8)
// SPI chip select - defined separately since it's manipulated with GPIO calls
#define EVE_CS_NOT  (9)
// Reserved for use with the SD card library
#define SD_CS       (10)
// Debug LED, or used for scope trigger or precise timing
#define DEBUG_LED   (3)

//Faster direct port access (specific to AVR)
#define CLR_EVE_PD_NOT        (PORTB &= ~(0x01))
#define SET_EVE_PD_NOT        (PORTB |=  (0x01))
#define CLR_EVE_CS_NOT        (PORTB &= ~(0x02))
#define SET_EVE_CS_NOT        (PORTB |=  (0x02))
#define CLR_SD_CS_NOT         (PORTB &= ~(0x04))
#define SET_SD_CS_NOT         (PORTB |=  (0x04))
#define CLR_MOSI              (PORTB &= ~(0x08))
#define SET_MOSI              (PORTB |=  (0x08))
#define CLR_MISO              (PORTB &= ~(0x10))
#define SET_MISO              (PORTB |=  (0x10))
#define CLR_SCK               (PORTB &= ~(0x20))
#define SET_SCK               (PORTB |=  (0x20))
#define CLR_DEBUG_LED         (PORTD &= ~(0x08))
#define SET_DEBUG_LED         (PORTD |=  (0x08))
//============================================================================
#endif // __CFA10108_DEFINES_H__
