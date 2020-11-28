import ntptime
def sync_ntp():
     ntptime.NTP_DELTA = 3155644800   # 可选 UTC+8偏移时间（秒），不设置就是UTC0
     ntptime.host = 'ntp1.aliyun.com'  # 可选，ntp服务器，默认是"pool.ntp.org"
     ntptime.settime()   # 修改设备时间,到这就已经设置好了

sync_ntp()

from machine import RTC
rtc = RTC()
 
print(rtc.datetime())  # (year, month, day[, hour[, minute[, second[, microsecond[, tzinfo]]]]])
AliNTP='NTP time of Aliyun:'+str(rtc.datetime()[0])+'-'+str(rtc.datetime()[1])+'-'+str(rtc.datetime()[2])+' '+str(rtc.datetime()[4])+':'+str(rtc.datetime()[5])+':'+str(rtc.datetime()[6])
print('NTP time of Aliyun:'+str(rtc.datetime()[0])+'-'+str(rtc.datetime()[1])+'-'+str(rtc.datetime()[2])+' '+str(rtc.datetime()[4])+':'+str(rtc.datetime()[5])+':'+str(rtc.datetime()[6]))