# CFAF1024600B0 Family EVE Demo Code

This example Seeeduino (Arduino clone) code is for the Crystalfontz line of 7" displays powered by a Bridgetek EVE chip. These displays are driven by the powerful BT817 chip and are available in the capacitive touch option only right now. They all come with a sunlight readable backlight that boasts 1000 nits when no additional layer is on top of the display. Please refer to the datasheets for more information.

For more information on our full list of EVE displays, please click [here](https://www.crystalfontz.com/products/eve-accelerated-tft-displays.php)

## Display information
7" EVE Displays\
[CFAF1024600B0-070SC-A1](https://www.crystalfontz.com/product/CFAF1024600B0-070SC-A1) - Capacitive Touch

 
Full Functional Seeeduino Demo Kits for these products can be found here:  
[CFAF1024600B0-070SC-A1-2](https://www.crystalfontz.com/product/CFAF1024600B0-070SC-A1-2) - Capacitive Touch Dev Kit
## Navigating the Code

To toggle on or off different demonstrations, some defines in "CFA10108_defines.h" can be changed.


```c++
#define BMP_DEMO             (0)  
#define   BMP_SCROLL         (0)  
#define SOUND_DEMO           (0)  
#define   SOUND_VOICE        (0)  
#define   SOUND_PLAY_TIMES   (10)
#define LOGO_DEMO            (1)  
#define   LOGO_PNG_0_ARGB2_1 (1)  
#define BOUNCE_DEMO          (1)  
#define MARBLE_DEMO          (0)  
#define TOUCH_DEMO           (0)
#define VIDEO_DEMO           (1) 
```

`BMP_DEMO` - Toggled to 1 will look to the uSD card to pull the "SPLASH.RAW" file and display it \
`BMP_SCROLL` - Toggled to 1 will look to the uSD card to pull the "CLOUDS.RAW" file and display it scrolling accross the screen\
`LOGO_DEMO` - Toggled to 1 will display the Crystalfontz Logo from flash\
`BOUNCE_DEMO` - Toggled to 1 will show a ball bouncing around the screen\
`MARBLE_DEMO` - Toggled to 1 will look in the uSD card and pull "BLUEMARB.RAW" and demonstrate the earth rotating and bouncing around in screen in place of the ball\
`TOUCH_DEMO` - Toggled to 1 will enable the touch screen (only compatible on touch versions of the display)\
`VIDEO_DEMO` - Toggled to 1 will enable the video demo. Video files "Ice_400.avi" must have been previsously loaded into flash.


## Connection Details
#### To [CFA10098 Adapter Board](https://www.crystalfontz.com/product/cfa10098) (See kits above)
| 10098 Pin         | Seeeduino Pin| Connection Description |
|-------------------|--------------|------------------------|
| 1  (3v3)          | 3v3          | +3.3V Power            |
| 2  (GND)          | GND          | Ground                 |
| 3  (SCK)          | D13          | Serial Clock           |
| 4  (MOSI/D0)      | D11          | MOSI/D0                |
| 5  (MISO/D1)      | D12          | MISO/D1                |
| 6  (GPIO0/D2)     | DNC          | GPIO0/D2               |
| 7  (GPIO1/D3)     | DNC          | GPIO1/D2               |
| 8  (GND)          | GND or DNC   | Ground                 |
| 9  (CS)           | D9           | Chip Select            |
| 10 (INT)          | D7           | Interupt               |
| 11 (PD)           | D8           | Chip Power Down        |
| 12 (MODE/GPIO2)   | DNC          | MODE/GPIO2             |
| 13 (AUDIO)        | DNC          | Audio PWM              |
| 14 (GND)          | GND or DNC   | Ground                 |


#### To Optional [uSD Adapter Board](https://www.crystalfontz.com/product/cfa10112) 
| microSD Pin | Seeeduino Pin| Connection Description |
|-------------|--------------|------------------------|
| 1 (CS)      | D10          | SD CS                  |
| 2 (DI)      | D11          | SD MOSI                |
| 3 (DO)      | D12          | SD MISO                |
| 4 (VDD)     | 3v3          | +3.3V Power            |
| 5 (SCLK)    | D13          | SD SCLK                |
| 6 (VSS)     | GND          | Ground                 |

## Additional Accessories
Additional accessories for the products can be found at the bottom of each of the product pages. This will include 30 position FFC cables, wires, and any accessory boards that are available.

## Flash Operation
to program the built in W25Q128JVPIQ flash, set PROGRAM_FLASH_FROM_USD to (1). The program is designed to take a file called splash.a8z, bluemarb.a8z, cloud.a8z, and/or ice.avi.

Demo files can be found in "CFA10108\uSD_Files". 

To create an *.a8z file, following the below steps.
1. create a 24-bit bmp
2. load the bmp into [EVE Asset Builder](https://brtchip.com/toolchains/)
3. Set the settings
    * Use the "image Converter" tab
    * ADD the *.bmp
    * Set EVE Product Range: BT81X
    * Output Format: COMPRESSSED_RGBA_ASTC_8x8_KHR
    * Compressed: yes
    * ASTC Preset: thorough
    * Select CONVERT
4. Rename the *.bin file created to splash.a8z (or bluemarb.a8z or cloud.a8z depending on the demo)
5. Load the splash.a8z file onto the uSD connected with the Seeeduino controlling the EVE display
6. Run the program ensuring that PROGRAM_FLASH_FROM_USD = 1 (this may take a while)
7. Use the Serial monitor to get the values shown on lines 96-103 of CFA10108_defines.h
8. Update lines 96-103 of CFA10108_defines.h with the new values and change PROGRAM_FLASH_FROM_USD = 0
9. The data is stored in flash and will now show when the demos are turned on

For an in-depth guide to loading custom images on our EVE lineup of displays without an SD card (using the Seeeduino's flash memory), please refer to our blog post [available here](https://www.crystalfontz.com/blog/custom-images-on-eve-displays/).
