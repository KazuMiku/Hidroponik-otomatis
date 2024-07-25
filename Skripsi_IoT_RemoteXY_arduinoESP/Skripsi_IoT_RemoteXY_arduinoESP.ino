//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// you can enable debug logging to Serial at 115200
//#define REMOTEXY__DEBUGLOG    

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_SOFTSERIAL_CLOUD
#include <SoftwareSerial.h>

#include <DHT.h>

#include <BH1750.h>
#include <Wire.h>

BH1750 lightMeter;

#define DHTPIN 2
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// RemoteXY connection settings 
#define REMOTEXY_SERIAL_RX A4
#define REMOTEXY_SERIAL_TX A5
#define REMOTEXY_SERIAL_SPEED 9600
#define REMOTEXY_WIFI_SSID "UTM DARURAT WIFI"
#define REMOTEXY_WIFI_PASSWORD "bayar2000"
#define REMOTEXY_CLOUD_SERVER "cloud.remotexy.com"
#define REMOTEXY_CLOUD_PORT 6376
#define REMOTEXY_CLOUD_TOKEN "a240c1a891899ecc4159f2da5d16bf78"


#include <RemoteXY.h>

// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =   // 317 bytes
  { 255,8,0,44,0,54,1,17,0,0,0,28,1,126,200,1,1,24,0,2,
  32,71,28,14,0,2,26,31,31,79,78,0,79,70,70,0,2,2,71,28,
  14,0,2,26,31,31,79,78,0,79,70,70,0,2,92,32,28,14,1,2,
  26,31,31,79,78,0,79,70,70,0,2,62,32,28,14,1,2,26,31,31,
  79,78,0,79,70,70,0,2,32,32,28,14,1,2,26,31,31,79,78,0,
  79,70,70,0,2,2,32,28,14,1,2,26,31,31,79,78,0,79,70,70,
  0,2,62,71,28,14,0,2,26,31,31,79,78,0,79,70,70,0,2,92,
  71,28,14,0,2,26,31,31,79,78,0,79,70,70,0,67,10,121,50,14,
  5,94,26,11,67,10,168,50,14,5,94,26,11,67,66,121,50,14,5,94,
  26,11,129,24,113,19,8,77,83,117,104,117,0,129,70,113,44,8,77,107,
  101,108,101,109,98,97,112,97,110,0,129,6,24,19,8,6,87,80,48,0,
  129,96,63,19,8,6,72,0,129,66,63,19,8,6,80,51,0,129,34,63,
  19,8,6,80,50,0,129,6,63,19,8,6,80,49,0,129,36,24,19,8,
  6,87,80,49,0,129,68,24,19,8,6,87,80,50,0,129,96,24,19,8,
  6,87,80,51,0,67,66,168,50,14,5,94,26,11,129,77,160,27,8,77,
  67,97,104,97,121,97,0,129,26,160,19,8,77,84,68,83,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t Aerator_2; // =1 if switch ON and =0 if OFF
  uint8_t Aerator_1; // =1 if switch ON and =0 if OFF
  uint8_t WaterPump_3; // =1 if switch ON and =0 if OFF
  uint8_t WaterPump_2; // =1 if switch ON and =0 if OFF
  uint8_t Waterpump_1; // =1 if switch ON and =0 if OFF
  uint8_t WaterPump_0; // =1 if switch ON and =0 if OFF
  uint8_t Aerator_3; // =1 if switch ON and =0 if OFF
  uint8_t Humidity; // =1 if switch ON and =0 if OFF

    // output variables
  char suhu[11]; // string UTF8 end zero
  char TDS[11]; // string UTF8 end zero
  char kelembapan[11]; // string UTF8 end zero
  char cahaya[11]; // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)
 
/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_AERATOR_2 8
#define PIN_AERATOR_1 7
#define PIN_WATERPUMP_3 6
#define PIN_WATERPUMP_2 5
#define PIN_WATERPUMP_1 4
#define PIN_WATERPUMP_0 3
#define PIN_AERATOR_3 9
#define PIN_HUMIDITY 10

#define TdsSensorPin A1
#define VREF 5.0      // analog reference voltage(Volt) of the ADC
#define SCOUNT  30           // sum of sample point
int analogBuffer[SCOUNT];    // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25; //kalibrasi sensor tds 1

void setup() 
{
  RemoteXY_Init (); 
  
  pinMode (PIN_AERATOR_2, OUTPUT);
  pinMode (PIN_AERATOR_1, OUTPUT);
  pinMode (PIN_WATERPUMP_3, OUTPUT);
  pinMode (PIN_WATERPUMP_2, OUTPUT);
  pinMode (PIN_WATERPUMP_1, OUTPUT);
  pinMode (PIN_WATERPUMP_0, OUTPUT);
  pinMode (PIN_AERATOR_3, OUTPUT);
  pinMode (PIN_HUMIDITY, OUTPUT);

  dht.begin();

  pinMode(TdsSensorPin,INPUT);
  Wire.begin(); //SCL, SDA
  lightMeter.begin();
  // TODO you setup code
  
}

void loop() 
{ 
  RemoteXY_Handler ();
  
  digitalWrite(PIN_AERATOR_2, (RemoteXY.Aerator_2==0)?LOW:HIGH);
  digitalWrite(PIN_AERATOR_1, (RemoteXY.Aerator_1==0)?LOW:HIGH);
  digitalWrite(PIN_WATERPUMP_3, (RemoteXY.WaterPump_3==0)?LOW:HIGH);
  digitalWrite(PIN_WATERPUMP_2, (RemoteXY.WaterPump_2==0)?LOW:HIGH);
  digitalWrite(PIN_WATERPUMP_1, (RemoteXY.Waterpump_1==0)?LOW:HIGH);
  digitalWrite(PIN_WATERPUMP_0, (RemoteXY.WaterPump_0==0)?LOW:HIGH);
  digitalWrite(PIN_AERATOR_3, (RemoteXY.Aerator_3==0)?LOW:HIGH);
  digitalWrite(PIN_HUMIDITY, (RemoteXY.Humidity==0)?LOW:HIGH); 
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  dtostrf(temperature, 0, 1, RemoteXY.suhu); //nilai yang akan muncul di layar suhu dalam celcius
  dtostrf(humidity, 0, 1, RemoteXY.kelembapan); //nilai yang akan muncul dilayar kelembapan dalam persen

  float lux = lightMeter.readLightLevel();
  dtostrf(lux, 0, 1, RemoteXY.cahaya);

   static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)     //every 40 milliseconds,read the analog value from the ADC
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0; // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      float compensationCoefficient=1.0+0.02*(temperature-25.0);    //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
      float compensationVolatge=averageVoltage/compensationCoefficient;  //temperature compensation
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; //convert voltage value to tds value
      dtostrf(tdsValue, 0, 1, RemoteXY.TDS); //nilai yang akan muncul di layar tds meter
   }
}
 int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
  // TODO you loop code
  // use the RemoteXY structure for data transfer
  // do not call delay(), use instead RemoteXY_delay() 


}