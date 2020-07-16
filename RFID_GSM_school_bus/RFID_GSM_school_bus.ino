//#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
// VARIABLE DECLARATION

char input[12];
int count = 0, flag1 = 1, flag2 = 1, flag3 = 1, B;
long int lat, lon, lat1, lat2, lon1, lon2; // create variable for latitude and longitude object
char buff[50];

SoftwareSerial em(5, 6);
SoftwareSerial gpsSerial(10, 11); // create gps sensor connection rx,tx
TinyGPS gps;                      // create gps object

// SETUP FUNCTION
void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  em.begin(9600);        // START SERIAL AT BAUD RATE OF 9600 BITS/SEC
  gpsSerial.begin(9600); // connect gps sensor

  pinMode(12, OUTPUT);
  pinMode(4, INPUT_PULLUP);
}

// LOOP FUNCTION

void loop()
{

  em.listen();
  if (em.available()) // CHECK FOR AVAILABILITY OF SERIAL DATA
  {
    count = 0; // Reset the counter to zero
    while (em.available() && count < 12)
    {
      input[count] = em.read(); // Read 1 Byte of data and store it in the input[] variable
      count++;                  // increment counter
      delay(5);
    }
    // PRINTING RFID TAG
    for (int i = 0; i < 12; i++)
      Serial.print(input[i]);
    Serial.println();
    tone(12, 2000, 1000);
    delay(3000);
  }

  if ((input[11] == 'F') && (flag1 == 1))
  {

    Serial.println("Welcome Ms Ayushi Enter");
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r"); // AT+CNGF command sets the GSM modem in SMS Text Mode or SMS PDU Mode
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r"); //Set the GSM modem to Text Mode SMS and send a message to GSM number +919557020692.
    delay(1000);
    Serial.print("Your Ward Ms. Ayushi boarded the school bus \r"); // Message that you have to send on above GSM number
    delay(1000);
    Serial.write(0x1A); // To define end of message
    delay(1000);
    flag1 = 0;
    input[11] = 0;
  }
  else if ((input[11] == 'F') && (flag1 == 0))
  {

    Serial.println("Welcome Ms Ayushi Exit");
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r");
    delay(1000);
    Serial.print("Your Ward Ms. Ayushi deboarded the school bus \r");
    delay(1000);
    Serial.write(0x1A);
    delay(1000);
    flag1 = 1;
    input[11] = 0;
  }
  else if ((input[11] == '2') && (flag2 == 1))
  {

    Serial.println("Welcome Ms. Akriti Enter");
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r");
    delay(1000);
    Serial.print("Your Ward Ms. Akriti boarded the school bus \r");
    delay(1000);
    Serial.write(0x1A);
    delay(1000);
    flag2 = 0;
    input[11] = 0;
  }
  else if ((input[11] == '2') && (flag2 == 0))
  {
    Serial.println("Welcome Ms. Akriti Exit");
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r");
    delay(1000);
    Serial.print("Your Ward Ms. Akriti deboarded the school bus \r");
    delay(1000);
    Serial.write(0x1A);
    delay(1000);
    flag2 = 1;
    input[11] = 0;
  }
  else if ((input[11] == '6') && (flag3 == 1))
  {

    Serial.println("Welcome Ms. Alia Enter");
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r");
    delay(1000);
    Serial.print("Your Ward Ms. Alia boarded the school bus \r");
    delay(1000);
    Serial.write(0x1A);
    delay(1000);
    flag2 = 0;
    input[11] = 0;
  }
  else if ((input[11] == '6') && (flag3 == 0))
  {

    Serial.println("Welcome Ms. Alia Exit");
    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r");
    delay(1000);
    Serial.print("Your Ward Ms. Alia deboarded the school bus \r");
    delay(1000);
    Serial.write(0x1A);
    delay(1000);
    flag2 = 1;
    input[11] = 0;
  }

  B = digitalRead(4);
  if (B == 0)
  {

    tone(12, 5000, 1000);
    gpsSerial.listen();
    if (gpsSerial.available())
    { // check for gps data
      if (gps.encode(gpsSerial.read()))
      {                               // encode gps data
        gps.get_position(&lat, &lon); // get latitude and longitude
        // display position
        lat1 = lat / 1000000;
        lat2 = lat % 1000000;
        lon1 = lon / 1000000;
        lon2 = lon % 1000000;
        sprintf(buff, "www.google.com.ph/maps/place/%ld.%ld,%ld.%ld", lat1, lat2, lon1, lon2);
        Serial.print(buff);
        Serial.print("+");
        delay(2000);
      }
    }

    Serial.print("\r");
    delay(1000);
    Serial.print("AT+CMGF=1\r");
    delay(1000);
    Serial.print("AT+CMGS=\"+917906706094\"\r");
    delay(1000);
    Serial.print(buff);
    delay(1000);
    Serial.write(0x1A);
    delay(1000);
  }
}
