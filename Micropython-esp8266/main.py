import machine,network,utime,ntptime
from machine import Pin

print("***loading main.py")

wlan = network.WLAN(network.STA_IF)
wlan.active(True);
wlan.connect('CMCC-Stone', 'shiyongqian')
wlan.ifconfig()
print(wlan.ifconfig())
machine.freq()

# ntptime.NTP_DELTA = 3155644800   # 可选 UTC+8偏移时间（秒），不设置就是UTC0
# ntptime.host = 'ntp1.aliyun.com'  # 可选，ntp服务器，默认是"pool.ntp.org"
# ntptime.settime() # set the rtc datetime from the remote server
# print("*****Reading machine.datetime")
rtc = machine.RTC()
rtc.datetime() # get the date and time in UTC
# d = dht.DHT11(machine.Pin(16))
# d.measure()
# d.temperature() # eg. 23 (°C)
# d.humidity()    # eg. 41 (% RH)
x = (0.9, 0.4, 0.9, 0.4, 0.9, 0.4,0.9, 0.4,0.9)
y = (0.4,)
led16 = Pin(16,Pin.OUT)
led2 = Pin(2,Pin.OUT)
for i in x:
    led16.on()
    led2.off()
    utime.sleep(i)
    for j in y:
        led16.off()
        led2.on()
        utime.sleep(j)
led16.off()
led2.off()
print("Fuck you Fuck you Fuck you and you!!!!!!")
print(wlan.ifconfig())

print("*******Finish loading main.py, Ready to go.")

#----------*-----------------
# html_info = "T:" + str(d.temperature()) + "C" + "   " + "H:" + str(d.humidity()) + "%"
# html_info2 = "<button onclick=" + str("d.measure()") + ">refresh</button>"
# 
# import picoweb
# app = picoweb.WebApp(__name__)
# @app.route("/")
# def index(req, resp):
#     yield from picoweb.start_response(resp, content_type = "text/html")
# #    yield from resp.awrite("I can show you a table of <a href='squares'>squares</a>.")
# #    yield from resp.awrite(html_info + html_info2)
# #    yield from resp.awrite(str(d.humidity()))
#     htmlFile = open('html_1.html', 'r')
#     for line in htmlFile:
#       yield from resp.awrite(line)
# 
# #@app.route("/squares")
# #def squares(req, resp):
# #    yield from picoweb.start_response(resp)
# #    yield from app.render_template(resp, "squares.tpl", (req,))
# import ulogging as logging
# logging.basicConfig(level=logging.INFO)
# 
# app.run(debug=1, host="192.168.1.6")
#----------*-----------------