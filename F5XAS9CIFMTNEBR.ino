#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define SensorPin 0          //pH meter Analog output to Arduino Analog Input 0
unsigned long int avgValue;  //Store the average value of the sensor feedback
float b;
int buf[10],temp;
float pHval;
// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 7

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
 
void setup()
{
  // start serial port
  lcd.begin(16,2);

  pinMode(13,OUTPUT);
  lcd.print("Welcome to the AMO");
  Serial.begin(300);
  Serial.println("Dallas Temperature IC Control Library Demo");
  
  // Start up the library
  
  sensors.begin();
  lcd.clear();
  delay(300);
  
  lcd.setCursor(0,0);
}
 
 
void loop()
{
  
  
  getPH();
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");

  Serial.print("Temperature is: ");
  Serial.print(sensors.getTempFByIndex(0)); // Why "byIndex"?
  lcd.setCursor(0,0);
  lcd.print(sensors.getTempFByIndex(0));
  lcd.setCursor(6,0);
  lcd.print("F");
  lcd.setCursor(0,1);
  lcd.print(pHval);
  lcd.setCursor(6,1);
  lcd.print("pH");
}

void getPH()
{
  for(int i=0;i<10;i++)       //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);
    
  }
  for(int i=0;i<9;i++)        //sort the analog from small to large
  {
    for(int j=i+1;j<10;j++)
    {
      if(buf[i]>buf[j])
      {
        temp=buf[i];
        buf[i]=buf[j];
        buf[j]=temp;
      }
    }
  }
  avgValue=0;
  for(int i=2;i<8;i++)                      //take the average value of 6 center sample
    avgValue+=buf[i];
  float phValue=(float)avgValue*5.0/1024/6; //convert the analog into millivolt
  phValue=3.5*phValue; //convert the millivolt into pH value
  pHval = phValue;
  Serial.print("    pH:");  
  Serial.print(phValue,2);
  Serial.println(" ");
  digitalWrite(13, HIGH);       
  delay(800);
  digitalWrite(13, LOW); 

}


