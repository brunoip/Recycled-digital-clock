#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

int pinDato = 9;  // Pin conectado a DS pin 14 de 74HC595
int pinClock = 16;  // Pin conectado a SHCP pin 11 de 74HC595
int pinRegistro = 10; // Pin conectado a STCP pin 12 de 74HC595

int dataGREEN2 = B00000000;
int dataRED2   = B00010010;

int dataGREEN1 = B01101000;
int dataRED1   = B01110000;
                           //       0           1         2         3           4          5           6         7          8          9         
int firstDigit[ 2 ][ 10 ] = { { B00000000, B00000000 ,B00001100 ,B00001100, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 }, 
                              { B00000000, B00010010 ,B00001110 ,B00010110, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 }};

int secondDigit[ 2 ][ 10 ] = {{ B11010000, B00000000 ,B01110000 ,B01100000, B10100000, B11100000, B11110000, B00000000, B11110000, B10100000 },
                              { B11100000, B01100000 ,B10100000 ,B11100000, B01100000, B11000000, B11000000, B11100000, B11100000, B11100000 }};

int thirdDigit[ 2 ][ 10 ] = { { B00000111, B00000110 ,B00000011 ,B00000111, B00000110, B00000101, B00000101, B00000111, B00000111, B00000111 },
                              { B00001101, B00000000 ,B00001110 ,B00000110, B00000011, B00000111, B00001111, B00000000, B00001111, B00000011 }};

int forthDigit[ 2 ][ 10 ] =  {{ B01101000, B00000000 ,B00111000 ,B00110000, B01010000, B01110000, B01111000, B00000000, B01111000, B01010000 },
                              { B01110000, B00110000 ,B01010000 ,B01110000, B00110000, B01100000, B01100000, B01110000, B01110000, B01110000 }};

int pinGreen = 8;
int pinRed = 7;
 
void setup() {
  
  pinMode (pinDato, OUTPUT);
  pinMode (pinClock, OUTPUT);
  pinMode (pinRegistro, OUTPUT);

  pinMode (pinGreen, OUTPUT);
  pinMode (pinRed, OUTPUT);

  //Initialize the serial port, wire library and RTC module
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  //If we remove the comment from the following line, we will set up the module time and date with the computer one
  //RTC.adjust(DateTime(__DATE__, __TIME__));
}
 
void loop() {
  
  DateTime now = RTC.now();
  int parsedHour=now.hour()>=13?now.hour()-12:now.hour();

  int decenasHoras=parsedHour/10;
  int unidadesHoras=parsedHour-( decenasHoras*10 );

  int decenasMinutos=now.minute()/10;
  int unidadesMinutos=now.minute()-(decenasMinutos*10 );
  
  writeDisplay(decenasHoras,unidadesHoras,decenasMinutos,unidadesMinutos, false, true);
}

void writeDisplay(int mil, int cen, int dec, int uni, bool dot1, bool dot2)
{
  int dataGREEN2 = B00000000;
  int dataRED2   = B00000000;
  
  int dataGREEN1 = B00000000;
  int dataRED1   = B00000000;

  dataGREEN2 = firstDigit[0][mil] | secondDigit[0][cen];
  dataRED2   = firstDigit[1][mil] | secondDigit[1][cen];

  dataGREEN1 = thirdDigit[0][dec] | forthDigit[0][uni];
  dataRED1   = thirdDigit[1][dec] | forthDigit[1][uni];

  if(dot1)
    dataGREEN2 = dataGREEN2 | B00000001;

  if(dot2)
    dataGREEN1 = dataGREEN1 | B10000000;

  digitalWrite(pinGreen,HIGH);
  digitalWrite(pinRed,LOW);
  
  digitalWrite(pinRegistro,LOW);

  shiftOut(pinDato, pinClock, MSBFIRST, dataGREEN1);
  shiftOut(pinDato, pinClock, MSBFIRST, dataGREEN2);
 
  digitalWrite(pinRegistro,HIGH);

  delay (12);

  digitalWrite(pinGreen,LOW);
  digitalWrite(pinRed,HIGH);
  
  digitalWrite(pinRegistro,LOW);

  shiftOut(pinDato, pinClock, MSBFIRST, dataRED1);
  shiftOut(pinDato, pinClock, MSBFIRST, dataRED2);
 
  digitalWrite(pinRegistro,HIGH);

  delay (12);
}
