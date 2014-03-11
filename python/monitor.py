import datetime, time
from tempodb import Client, DataPoint
from Temperature import Temperature
from config import USER, PWD, API_KEY, API_SECRET

UPDATE_INTERVAL = 60 # seconds

tempoDB = Client(API_KEY, API_SECRET)
core = Temperature(USER, PWD, 'mutant_jetpack')
core.setInterval(UPDATE_INTERVAL)
core.setRGB(0)
print core

while core.isConnected():
    tempc = core.temperature() 
    date = datetime.datetime.utcnow()
    if tempc:
        tempoDB.write_key("TEMPERATURE", [DataPoint(date, tempc)])
        print "%s, %1.2f" % (datetime.datetime.now(), tempc)
        time.sleep(UPDATE_INTERVAL)