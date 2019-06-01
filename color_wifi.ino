/**
 * Remote control RGB LED
 * ESP 8226
 * Common annode.
 * Missing corner = blue with Oomlout RGB square LED, with blue at bottom, clockwise pinout is R G B Common+.
 * Used 270 ohm resistors for all 3 colors, but can use 100 ohm for brighter.
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "wifi_config.h"
 
#define SERVER_PORT 80

int redPin = D5;
int greenPin = D6;
int bluePin = D7;

// comment out this line if using a common cathode LED instead
#define COMMON_ANODE

// ESP8226 pins all use 10-bit software analog read
short fullBright = 1023;

short red;
short green;
short blue;

void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(115200);
  Serial.println("Setup complete");
  red = green = blue = fullBright;
  setColor(red, green, blue);
}

short readValue(){
  int buffSize = 5;
  char valueStr[buffSize];
  // default the buffer to the null terminated string 0, so we default to a value of 0
  valueStr[0] = '0';
  valueStr[1] = '\0';
  int i = 0;
  int nextCh;
  // read to the end of the input / line
  while(Serial.available() > 0 && (nextCh = Serial.read()) != '\n') {
    if(i < buffSize -1){
      valueStr[i++] = (char) nextCh;
    }
  }

  // terminate the string
  valueStr[i++] = '\0';
  Serial.print("got valueStr ");
  Serial.println(valueStr);
  // convert the characters to a value (special value of m = max brightness)
  int value = (valueStr[0] == 'm') ? fullBright : (short) atoi(valueStr);
  Serial.print("got value ");
  Serial.println(value);
  return value;
}

void loop()
{
  int color;
  if(Serial.available() > 0){
      color = Serial.read();
      switch(color) {
        case 'r':
          red = readValue();
          break;
        case 'g':
          green = readValue();
          break;
        case 'b':
          blue = readValue();
          break;
        case 'w': // white
          red = green = blue = fullBright;
          break;
        case 'o': // off
          red = green = blue = 0;
          break;
        default:
          Serial.println("Unknown color");
      }
      
      Serial.print("Setting color ");
      Serial.print((char) color);
      Serial.print(": ");
      Serial.print(red);
      Serial.print(", ");
      Serial.print(green);
      Serial.print(", ");
      Serial.println(blue);
      setColor(red, green, blue);
  }

  delay(1000);
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = fullBright - red;
    green = fullBright - (green / 2);
    blue = fullBright - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
