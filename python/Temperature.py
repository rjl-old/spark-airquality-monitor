from Core import Core

class Temperature(Core):
    def __init__(self, usr, pwd, name):
        Core.__init__(self, usr, pwd, name)

    def setLow(self, temperature):
        self.function('program', 'LOW:%s' % temperature)
        
    def setHigh(self, temperature):
        self.function('program', 'HII:%s' % temperature)
        
    def setMin(self, temperature):
        self.function('program', 'MIN:%s' % temperature)
        
    def setMax(self, temperature):
        self.function('program', 'MAX:%s' % temperature)
                    
    def setInterval(self, interval):
        interval *= 1000
        self.function('program', 'INC:%s' % interval)
    
    def setRGB(self, state):
        self.function('program', 'RGB:%s' % state)
    
    def temperature(self):
        return self.variable('temperature')
        
    def light(self):
        return self.variable('light')
