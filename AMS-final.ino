#include <OneWire.h>                           //protocol to interface sensor w/ arduino
#include <DallasTemperature.h>                 //does calculations for temp sensor from mV to degrees
#include <LiquidCrystal.h>                     //control and interface library for LCD display
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);         //defines array of pins for LCD interface

#define SensorPin 0                            //pH meter Analog output to Arduino Analog pin 0
unsigned long int avgValue;                    //Store the average value of the sensor feedback
float b;
int buf[10],temp;                              //lists that store pH vaues while calculating
float pHval;                                   //final pH value that is transmitted to LCD Display

#define ONE_WIRE_BUS 7                         // data wire for temp sensor


OneWire oneWire(ONE_WIRE_BUS);                 //oneWire protocol for temperature sensor
 

DallasTemperature sensors(&oneWire);           //oneWire reference for Dallas Temperature
 
void setup()
{
  lcd.begin(16,2);                             //define size of LCD

  pinMode(13,OUTPUT);                          // onBoard LED on arduino
  lcd.print("Initializing...");                //Initializing message on LCD
  
  sensors.begin();                             //begins requesting data from temp sensor
  
  delay(300);                                  //display Initialize message for 300ms
  lcd.clear();                                 //clear display
  lcd.setCursor(0,0);                          //set print to first column, first row
}
 
 
void loop()
{
  
  
  getPH();                                    //pH calculation function, see below
  lcd.setCursor(0,0);                         //reset display
  /*
  TEMPERATURE PRINT TO LCD
  */ 
  lcd.print(sensors.getTempFByIndex(0) + 1);      //print temp value from temp sensor
  lcd.setCursor(6,0);
  lcd.print("F"); //units
  /*
  pH PRINT TO LCD
  */
  lcd.setCursor(0,1);
  lcd.print(pHval + 1);                         //print pH val given by getPH function (see below)
  lcd.setCursor(6,1);
  lcd.print("pH");                          //units to do away with confusion
}

void getPH()
{
  for(int i=0;i<10;i++)                     //Get 10 sample value from the sensor for smooth the value
  { 
    buf[i]=analogRead(SensorPin);          //read values and print them to list
    
  }
  for(int i=0;i<9;i++)                     //sort the analog from small to large
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
  phValue=3.5*phValue;                      //convert the millivolt into pH value
  pHval = phValue;                          //set final pH value to float variable for printing to LCD
   

}


