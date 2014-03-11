import json
from hammock import Hammock as Spark

# timeout for info requests (TODO: might get fixed by Spark)
TIMEOUT = 1 #seconds

# api dispatcher
spark = Spark("https://api.spark.io/v1")

class Core():
    # Spark Core class
            
    def __init__(self, usr, pwd, name=None):
        # set authorisation code, device id, and device name
        self.auth = self._get_auth(usr, pwd)
        if self.auth and name:
            self.name = name
            self.device_id = self._get_device_id(name)
        else:
            self.name = None
            self.device_id = None
    
    def _get_auth(self, usr, pwd):
        # get the authorisation code and return it as a dictionary
        access_token = None
        try:
            r = spark.access_tokens.GET(auth=(usr, pwd))
            access_token = r.json()[0]['token']
        except:
            pass
        return {'access_token': access_token}
            
    def _get_device_id(self, name=None):
        # get the device id for the named core and return is as a string
        try:
            r = spark.devices.GET(params=self.auth)
            device = [device for device in r.json() if device.get('name') == name][0]
            return device.get('id')
        except:
            return None

    def _get_info(self):
        # get the device id and return it as a dictionary (empty if none or timeout)
        try:
            r = spark.devices(self.device_id).GET(params=self.auth, timeout=TIMEOUT)
            return r.json()
        except:
            return {}
                    
    def isConnected(self):
        # return True if the core is connected, False otherwise
        return self._get_info().get('connected', False)
    
    def getVariables(self):
        # return the device variables as a dictionary, empty if none
        return self._get_info().get('variables', {})

    def getFunctions(self):
        # return the device functions as a dictionary, empty if none
        return self._get_info().get('functions', {})

    def variable(self, variable):
        # return the value of 'variable' or None
        try:
            r = spark.devices(self.device_id, variable).GET(params=self.auth)
            return r.json().get('result')
        except:
            return None

    def function(self, function, parameter):
        # run 'function' on the device with 'parameter'
        try:
            parameters = {'args': parameter}
            r = spark.devices(self.device_id, function).POST(params=self.auth, data=parameters)
        except:
            pass
    
    def __str__(self):
        string = ""
        string += "Spark Core: %s (%s)\n" % (self.name, self.device_id)
        string += "Variables :\n"
        for v in self.getVariables().keys():
            string += "- %s: %s\n" % (v, self.variable(v))
        string += "Functions :\n" 
        for f in self.getFunctions():
            string += "- %s\n" % (f)
        string += "Connected : %s\n" % self.isConnected() 
        return string

