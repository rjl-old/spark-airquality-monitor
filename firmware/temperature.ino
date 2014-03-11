// ------------------------------------------------------------
// Reads temperature and sets a coloured LED and servo position
// ------------------------------------------------------------

int TEMPERATURE = A0;   // Temperature sensor
int SERVO = A1;         // Servo ES08A
int LEDONBOARD = D7;           // Onboard LED
int LEDRED = D1;
int LEDGREEN = D0;

Servo myservo;          // a Servo class for servo ES08A
int angle = 0;          // server angle (degrees)

double TEMP_LOW = 18.0;        // Comfort low threshold
double TEMP_HIGH = 21.0;        // Comfort high threshold
double TEMP_MIN = 14.0;       // Temperature range minimum
double TEMP_MAX = 23.0;       // Temperature range maximum

int voltage = 0;        // Temperature sensor voltage
double tempC = 0;       // Calculated temperature (degC)
double tempOld = 0;     // Previous temperature (for smoothing)
float alpha = 0.8;      // smoothing constant
int red = 0;            // LED red value (0-255)

long previousMillis = 0;// store the last time temperature was updated
long interval = 1000;   // interval at which to update (milliseconds)

void setup()
{
  Spark.variable("temperature", &tempC, DOUBLE); // temperature (degC)
  Spark.variable("TEMP_LOW", &TEMP_LOW, DOUBLE); // temperature (degC)
  Spark.variable("TEMP_HIGH", &TEMP_HIGH, DOUBLE); // temperature (degC)
  Spark.variable("TEMP_MIN", &TEMP_MIN, DOUBLE); // temperature (degC)
  Spark.variable("TEMP_MAX", &TEMP_MAX, DOUBLE); // temperature (degC)
  Spark.function("program", setParameter);      // program the device

  pinMode(TEMPERATURE, INPUT);  // temperature sensor
  pinMode(LEDONBOARD, OUTPUT);  // onboard LED
  pinMode(LEDRED, OUTPUT);      // onboard LED
  pinMode(LEDGREEN, OUTPUT);    // onboard LED
  myservo.attach(SERVO);        // Attach the servo to A1
  
  RGB.control(true);            // Take control of the onboard multicolour LED
}

void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis > interval) {
        previousMillis = currentMillis; 
        // get the temperaure sensor voltage (0-4095)
        voltage = analogRead(TEMPERATURE);
        //Convert the reading into degree celcius
        tempC = alpha*tempOld + (1-alpha)*(((voltage * 3.3)/4095) - 0.5) * 100;
        tempOld = tempC;

        setTemperatureLED(tempC);
        setServoAngle(tempC);
        setComfortLED(tempC);
        flashOnboard(10);
    }
}

void setTemperatureLED(double temp)
// set the colour of the onboard multicour LED depending on temperature
{
        red = constrain(map(tempC, TEMP_MIN, TEMP_MAX, 0, 255), 0, 255);
        RGB.color(red, 0, 255-red);
}

void setComfortLED(double temp)
// set the status LED red or green depending on temperature
{
    if (temp < TEMP_LOW || temp > TEMP_HIGH)
    {
        digitalWrite(LEDRED, HIGH);
        digitalWrite(LEDGREEN, LOW);
    }
    else
    {
        digitalWrite(LEDRED, LOW);
        digitalWrite(LEDGREEN, HIGH);
    }
}

void setServoAngle(double temp)
// set the angle of the servo depending on temperature
{
        angle = constrain(180*(tempC - TEMP_MIN)/(TEMP_MAX - TEMP_MIN), 0, 180);
        myservo.write(angle);
}

int setParameter(String command)
// parse command for instruction and set parameters
// COMMANDS:
//   LOW: Set comfort low threshold (degC)
//   HII: Set Comfort high threshold (degC)
//   INC: Set update interval (milliseconds)
{
    boolean success = false;
    if(command.startsWith("INC:")) 
    {
        interval = parseCommand(command);
        success = true;
    }
    else if(command.startsWith("LOW:"))
    {
        TEMP_LOW = (double) parseCommand(command);
        success = true;
    }
    else if(command.startsWith("HII:"))
    {
        TEMP_HIGH = (double) parseCommand(command);
        success = true;
    }
    else if(command.startsWith("MIN:"))
    {
        TEMP_MIN = (double) parseCommand(command);
        success = true;
    }
    else if(command.startsWith("MAX:"))
    {
        TEMP_MAX = (double) parseCommand(command);
        success = true;
    }
    
    
    if (success)
    {
        flashOnboard(500);
        return 1;
    }
    else 
    {
        flashOnboard(200);
        delay(100);
        flashOnboard(200);
        delay(100);
        return -1;
    }
}
    
int parseCommand(String command)
// get integer DDDDD from XXDDDDD
{
    return command.substring(4, command.length()).toInt();
}

void flashOnboard(int duration)
// flash the onboard led for the specified number of milliseconds
{
    digitalWrite(LEDONBOARD, HIGH);
    delay(duration);
    digitalWrite(LEDONBOARD, LOW);
}