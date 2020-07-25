#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

static const int RXPin = 7, TXPin = 6;
static const uint32_t GPSBaud = 9600;
const int button = 8;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

float flat, flong;

int counter = 0;

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  pinMode(button, INPUT_PULLUP);

  while (!Serial);
  Serial.println("LoRa Sender");
  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  
  LoRa.setTxPower(20);
  Serial.println("Setup Success");
}

void loop()
{
//  Serial.println("Loop");
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
//    Serial.println("Available");
    if (gps.encode(ss.read()))
//      Serial.println("Encode");
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    flat = gps.location.lat();
    flong = gps.location.lng();
    Serial.print(flat, 6);
    Serial.print(F(","));
    Serial.println(flong, 6);
    Serial.println(counter);
    counter++;
    if(counter % 20 == 0){
      LoRa.beginPacket();
      // LoRa.print("a");
      LoRa.print(flat, 6);
      LoRa.print('$');
      LoRa.print(flong, 6);
      LoRa.print("\n");
      LoRa.endPacket();
      Serial.println("Send Via LoRa");
    }
  }
  else
  {
    Serial.println(F("INVALID"));
  }
}
