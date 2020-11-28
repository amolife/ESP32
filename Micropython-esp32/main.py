import machine,network,math,uos,dht,esp,micropython,utime,ntptime
from machine import Pin

print("loading main.py··· ···")

Wifiname = 'CMCC-Stone'
Wifipwd = 'shiyongqian'

wlan = network.WLAN(network.STA_IF)
wlan.active(True);
wlan.connect(Wifiname, Wifipwd)
wlan.ifconfig()
machine.freq()

print("Setting up wifi to Connect %s···" % Wifiname)

d = dht.DHT11(machine.Pin(23))
d.measure()
d.temperature() # eg. 23 (°C)
d.humidity()    # eg. 41 (% RH)

ledtime_off = (0.2, 1, 0.2, 1, 0.2, 1, 0.2, 1)
ledtime_on = (0.1, 0.1)
led = Pin(2,Pin.OUT)
for i in ledtime_off:
    led.off()
    utime.sleep(i)
    for j in ledtime_on:
        led.on()
        utime.sleep(j)
    
print("Finish loading main.py, Ready to go.")
print(wlan.ifconfig())

import set_ntptime

## -------------------------
# import picoweb
# app = picoweb.WebApp(__name__)
# @app.route("/")
# def index(req, resp):
#     yield from picoweb.start_response(resp)
#     yield from resp.awrite('T:' + str(d.temperature()) + '°C' + 'H:' + str(d.humidity()) + '%RH')
# import ulogging as logging
# logging.basicConfig(level=logging.INFO)
# app.run(debug=True, host=wlan.ifconfig()[0])
## -------------------------

#import picoweb_webapp_DHT11

#import epddemo
#epddemo.test()

from esp32_gpio_lcd import GpioLcd
from utime import sleep_ms, ticks_ms
sheshidu_symbol = bytearray([0x07,0x05,0x07,0x00,0x00,0x00,0x00,0x00])
IP_symbol = bytearray([0x00,0x17,0x15,0x17,0x14,0x14,0x14,0x00])

# def test_LCD():
#     """Test function for verifying basic functionality."""
#     print("Running test_main")
lcd = GpioLcd(rs_pin=Pin(4),
                   enable_pin=Pin(17),
                   d4_pin=Pin(5),
                   d5_pin=Pin(18),
                   d6_pin=Pin(21),
                   d7_pin=Pin(22),
                   num_lines=2, num_columns=16)
#     lcd.move_to(0, 0)
#     lcd.custom_char(1, IP_symbol)  # this '1' means storage ID is 1
#     lcd.putchar(chr(1))  # this '1' means storage ID is 1
#     lcd.move_to(1, 0)
#     lcd.putstr(str(wlan.ifconfig()[0]))
#     lcd.move_to(0, 1)
#     lcd.putstr('T:' + str(d.temperature()) + ' ' + 'C')
#     lcd.move_to(4, 1)
#     lcd.custom_char(0, sheshidu_symbol)  # this '0' means storage ID is 0
#     lcd.putchar(chr(0))  # this '0' means storage ID is 0
#     lcd.move_to(8, 1)
#     lcd.putstr('H:' + str(d.humidity()) + '%RH')

lcd.move_to(0, 0)
lcd.putstr(set_ntptime.AliNTP[19::])
utime.sleep(2)  
lcd.clear()
while True:
    d.measure()
    utime.sleep(2)
#    test_LCD()
  
    lcd.move_to(0, 0)
    lcd.custom_char(1, IP_symbol)  # this '1' means storage ID is 1
    lcd.putchar(chr(1))  # this '1' means storage ID is 1
    lcd.move_to(1, 0)
    lcd.putstr(str(wlan.ifconfig()[0]))
    lcd.move_to(0, 1)
    lcd.putstr('T:' + str(d.temperature()) + ' ' + 'C')
    lcd.move_to(4, 1)
    lcd.custom_char(0, sheshidu_symbol)  # this '0' means storage ID is 0
    lcd.putchar(chr(0))  # this '0' means storage ID is 0
    lcd.move_to(8, 1)
    lcd.putstr('H:' + str(d.humidity()) + '%RH')
    
#     import picoweb
#     app = picoweb.WebApp(__name__)
#     @app.route("/")
#     def index(req, resp):
#         yield from picoweb.start_response(resp)
#         yield from resp.awrite('T:' + str(d.temperature()) + '°C' + 'H:' + str(d.humidity()) + '%RH')
#     import ulogging as logging
#     logging.basicConfig(level=logging.INFO)
#     app.run(debug=True, host=wlan.ifconfig()[0])
