##
 #  @filename   :   main.cpp
 #  @brief      :   1.54inch e-paper display demo
 #  @author     :   Yehui from Waveshare
 #
 #  Copyright (C) Waveshare     September 9 2017
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documnetation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to  whom the Software is
 # furished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 # THE SOFTWARE.
 ##

import epd1in54
import time

gImage_imag= [
    
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XF1,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0X8F,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XFE,0X01,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XCF,0XFF,0XFF,0XFF,0XFF,0XE1,0XCF,0XFF,0X89,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XF1,0XF8,0X00,0X7F,0XFF,0XFF,0XFF,0XFF,0XFF,0XC7,0XF7,0XFF,
    0X3F,0XFF,0XC0,0XC7,0XFF,0X88,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XF0,0X00,
    0X3F,0XFF,0XFF,0XFF,0XFF,0XFF,0XC7,0XFD,0XFE,0X3F,0XFF,0XC4,0X47,0XFF,0XF8,0XFF,
    0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XF0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XC4,
    0X01,0XE1,0X60,0XE1,0XE4,0X47,0XE1,0XC0,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,
    0XE0,0X00,0X1F,0XFF,0XFF,0XFF,0XFF,0XFF,0XC4,0X00,0XC0,0XC0,0X40,0XF8,0X47,0XC0,
    0X80,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XE1,0XE3,0X00,0X00,0X00,0X00,0X01,
    0XFF,0XC4,0X44,0X60,0X44,0X40,0X70,0XC7,0XC6,0X08,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XF1,0XC3,0XF3,0X0F,0XFF,0XFF,0XFF,0XFC,0XFF,0XC4,0X44,0X60,0X44,0X40,0X61,
    0XC7,0XC7,0X88,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XC3,0X3B,0X0F,0XFF,0XFF,
    0XFF,0XFE,0XFF,0XC4,0X44,0X40,0X44,0X47,0XC3,0XC7,0XC7,0X88,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XF1,0XC3,0X0F,0X0F,0XFF,0XFF,0XFF,0XFE,0XFF,0XC4,0X44,0X44,0X04,
    0X44,0XC7,0XC7,0XC4,0X88,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XF1,0XC3,0X0F,0X0F,
    0XFF,0XFF,0XFF,0XFE,0XFF,0XC4,0X40,0X60,0X00,0X60,0X40,0XC0,0X60,0X00,0XFF,0XE3,
    0XFF,0XFF,0X8F,0XFF,0XFE,0X01,0XFF,0X1F,0XFF,0XFF,0XF1,0XFE,0XFF,0XFF,0XFF,0XFE,
    0XC0,0X02,0XFF,0XFF,0XFF,0X03,0XFF,0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XF0,0X01,0XFF,
    0X9F,0XFF,0XFF,0XF1,0XFE,0XFF,0XFF,0XFF,0XFF,0X00,0X02,0XFF,0XFF,0XF8,0X00,0X7F,
    0XFF,0XFF,0XFF,0XFF,0X8F,0XFF,0XE0,0X01,0XE0,0X80,0X0F,0XFF,0XF1,0XFE,0XFF,0XFF,
    0XFF,0XFF,0X00,0X02,0XFF,0XFF,0XF0,0X00,0X3F,0XFF,0XE7,0XFF,0XFF,0X8F,0XFF,0XE0,
    0X03,0XE0,0X00,0X07,0XFF,0XF1,0XFE,0XFF,0XFF,0XFF,0XFF,0X80,0X06,0XFF,0XFF,0XE0,
    0XCC,0X1F,0XFF,0XE0,0XFF,0XFF,0X8F,0XFF,0XE2,0X71,0XE0,0X00,0X07,0XFF,0XF1,0XFE,
    0XFF,0XFF,0XFF,0XFF,0XC0,0X0E,0XFF,0XFF,0XE3,0XC7,0X1F,0XFF,0XE0,0X3F,0XFF,0X8F,
    0XFF,0XE7,0X39,0XE0,0X00,0X0F,0XFF,0XF1,0XFE,0XFF,0XFF,0XFF,0XFF,0XC0,0X0E,0XFF,
    0XFF,0XE7,0XE7,0X8F,0XFF,0XF0,0X07,0XFF,0X8F,0XFF,0XE7,0X38,0XFF,0XFF,0XFF,0XFF,
    0XF1,0XFE,0XFF,0XFF,0XFF,0XFF,0X00,0X1E,0XFF,0XFF,0XE7,0XE7,0X8F,0XFF,0XFE,0X01,
    0XFF,0X8F,0XFF,0XE3,0X10,0XFF,0XFF,0XFF,0XFF,0XF1,0XFE,0XFF,0XFF,0XFF,0XFC,0X00,
    0X3E,0XFF,0XFF,0XE3,0XC7,0X8F,0XFF,0XFF,0XC0,0X7F,0X8F,0XFF,0XE3,0X01,0XFF,0X1F,

]

def test():
    print('start\n')
    epd = epd1in54.EPD()
    epd.init(epd.lut_full_update)
    print('epd init finish \n')
    epd.clear_frame_memory(0xff)
    epd.set_frame_memory(gImage_imag,200,200, 0, 0)
    epd.display_frame()

    epd.delay_ms(2000)







