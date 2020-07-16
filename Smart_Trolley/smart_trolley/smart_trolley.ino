/*The circuit:
 * LCD RS pin to digital pin 12/22
 * LCD Enable pin to digital pin 11/24
 * LCD D4 pin to digital pin 5/26
 * LCD D5 pin to digital pin 4/28
 * LCD D6 pin to digital pin 3/30
 * LCD D7 pin to digital pin 2/32
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 for lcd*/
#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>
#include <LiquidCrystal.h>

SoftwareSerial espPort(10, 11); // RX, TX
ESP esp(&espPort, &Serial, 9);
REST rest(&esp);
boolean wifiConnected = false;

const int ESP8266_CHPD = 9;


LiquidCrystal lcd(22, 24, 26, 28, 30, 32);

char response[266];
char buff[64];
String strId,strData,strCode;
String strData_Last1,strData_Last2;

void(* resetFunc) (void) = 0;                          //declare reset function at address 0


void clearBuffer(void) {
       for (int i = 0;i<266;i++ ) {
         response[i]=0;
       }
}

void wifiCb(void* response)
{
  uint32_t status;
  RESPONSE res(response);

  if(res.getArgc() == 1) {
    res.popArgs((uint8_t*)&status, 4);
    if(status == STATION_GOT_IP) {
      Serial.println("WIFI CONNECTED");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("SYSTEM CONNECTED");
      lcd.setCursor(0, 1);
      lcd.print("LETS SHOP ^-^");
      lcd.setCursor(16, 0);
      lcd.print("READY TO SCAN");
      lcd.setCursor(16, 1);
      lcd.print("SCAN BARCODE");
      wifiConnected = true;
    } else {
      wifiConnected = false;
    }    
  }
}
/*
Barcode Scanner                                                        
  This code reads the input from a ps/2 keyboard or keyboard-like        
  device (e.g. a barcode scanner), translates the scan-codes into        
  numbers (only numbers from 0 to 9 can be used at the moment)           
  It is nowhere near a complete implementation of the ps/2 protocol,     
  but it should give you a starting point.                               
  mys .// Benjamin Maus ( benjamin.maus <at> allesblinkt.com )          
  2007                                                                   
*/
int A=0;
int a=0;
int SCAN_ENTER = 0x5a; int SCAN_BREAK = 0xf0;
int breakActive = 0;
int clockPin = 3; // Clock is only output. 
int dataPin = 2; // The data pin is bi-directional
        // But at the moment we are only interested in receiving   
int ledPin = 13;  // When a SCAN_ENTER scancode is received the LED blink
int clockValue = 0; byte dataValue;
byte scanCodes[38] = {0x45,0x16,0x1e,0x26,0x25,0x2e,0x36,0x3d,0x3e,0x46,0x1c,0x32,0x21,0x23,0x24,0x2b,0x34,0x33,0x43,0x3b,0x42,0x4b,0x3a,0x31,0x44,0x4d,0x15,0x2d,0x1b,0x2c,0x3c,0x2a,0x1d,0x22,0x35,0x1a,0x49,0x29}; 
char characters[38] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','.',' '};
int quantityCodes = 38;
char buffer[64] = {}; // This saves the characters (for now only numbers) 
int bufferPos = 0; 
int bufferLength = 64;
int sum=0;


void setup() {
  
  lcd.begin(16, 2);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  
lcd.setCursor(0, 0);
lcd.print("F.G. MART");
lcd.setCursor(0, 1);
lcd.print("WELCOME");
delay(2000);
lcd.setCursor(0, 0);
lcd.print("SYSTEM STARTED ");
lcd.setCursor(0, 1);
lcd.print("ESTABLISHING");
lcd.setCursor(16, 0);
lcd.print("CONNECTION");
espPort.begin(19200);
  
  esp.enable();
  delay(500);
  esp.reset();
  delay(500);
  while(!esp.ready());

  Serial.println("ARDUINO: setup rest client");
  if(!rest.begin("api.thingspeak.com")) {
    Serial.println("ARDUINO: failed to setup rest client");
    while(1);
  }

  /*setup wifi*/
  Serial.println("ARDUINO: setup wifi");
  esp.wifiCb.attach(&wifiCb);

  //esp.wifiConnect("not ur dads wifi","panditji9818228256@@%#$$");
 esp.wifiConnect("Shivam","12121212");
  Serial.println("ARDUINO: system started");
  
  pinMode(dataPin, INPUT);                                               
  pinMode(clockPin, INPUT);                                              
  pinMode(ledPin, OUTPUT);
  pinMode(7,INPUT);                                               
  Serial.begin(19200);                                                    
}

void loop() {

  esp.process();

  if(wifiConnected) { 
  dataValue = dataRead();                                                
  // If there is a break code, skip the next byte                        
  if (dataValue == SCAN_BREAK) {                                         
    breakActive = 1;                                                     
  }                                                                      
  // Translate the scan codes to numbers                                 
  // If there is a match, store it to the buffer                         
  for (int i = 0; i < quantityCodes; i++) {                              
    byte temp = scanCodes[i];                                            
    if(temp == dataValue){                                               
      if(!breakActive == 1){                                             
        buffer[bufferPos] = characters[i];                               
        bufferPos++;                                                     
      }                                                                  
    }                                                                    
  }                                                                      
  //Serial.print('*'); // Output an asterix for every byte               
  // Print the buffer if SCAN_ENTER is pressed.                          
  if(dataValue == SCAN_ENTER){                                          
    // Read the buffer
    if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='1'&&buffer[4]=='0'&&buffer[5]=='8'&&buffer[6]=='8'&&buffer[7]=='0'&&buffer[8]=='9'&&buffer[9]=='8'&&buffer[10]=='4'&&buffer[11]=='7'&&buffer[12]=='2')
       {
        A=0;
        Serial.print("Set Wet                                                   1 Piece         Rs.190      ");//lcd
     
        while(A==0)
         {
           Serial.println("Do you want to add this product to your cart");
           lcd.setCursor(0, 0);
           lcd.print("SET WET DEO     ");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.190  ");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=9;
                upload();
                A=1;
                sum=sum+190;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("NOT ADDED");
           lcd.setCursor(0, 1);
           lcd.print("PLZ PUT IT BACK");
           lcd.setCursor(16, 0);
           lcd.print("READY TO SCAN");
           lcd.setCursor(16, 1);
           lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=19;
                upload();
                A=1;
                sum=sum-190;
                delay(1000);
               }
            }
       }
    if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='1'&&buffer[4]=='0'&&buffer[5]=='2'&&buffer[6]=='3'&&buffer[7]=='0'&&buffer[8]=='1'&&buffer[9]=='4'&&buffer[10]=='3'&&buffer[11]=='9'&&buffer[12]=='0')
      { A=0;
        Serial.print("Cinthol Deo Stick                                         1 Piece         Rs.69      ");
      
       while(A==0)
         {
           Serial.println("Do you want to add this product to your cart");//lcd
           
           lcd.setCursor(0, 0);
           lcd.print("CINTHOL DEO TUBE");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.69   ");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=10;
                upload();
                A=1;
                sum=sum+69;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=20;
                upload();
                A=1;
                sum=sum-69;
                delay(1000);
               }
            }                                             
      }
    if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='1'&&buffer[4]=='2'&&buffer[5]=='0'&&buffer[6]=='7'&&buffer[7]=='5'&&buffer[8]=='0'&&buffer[9]=='3'&&buffer[10]=='5'&&buffer[11]=='3'&&buffer[12]=='5')
      { A=0;
        Serial.print("Odomos mosquito repellent cream                               1 Piece         Rs.27     ");                                                   
       
        while(A==0)
        {
          Serial.println("Do you want to add this product to your cart");//lcd
           
           lcd.setCursor(0, 0);
           lcd.print("ODOMOS(20% off) ");
           lcd.setCursor(0, 1);
           lcd.print("OFFER PRICE=Rs27");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=1;
                upload();
                A=1;
                sum=sum+27;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=11;
                upload();
                A=1;
                sum=sum-27;
                delay(1000);
               }
            }      
      }
    if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='6'&&buffer[4]=='0'&&buffer[5]=='4'&&buffer[6]=='5'&&buffer[7]=='3'&&buffer[8]=='7'&&buffer[9]=='1'&&buffer[10]=='0'&&buffer[11]=='2'&&buffer[12]=='5')
      { A=0;
        Serial.print("Pure sound portable speaker                   1 Piece         Rs.1699    ");                                                   
     
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
          
           lcd.setCursor(0, 0);
           lcd.print("SPEAKERS(5% off)");
           lcd.setCursor(0, 1);
           lcd.print("WITH OFF=Rs1699 ");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=2;
                upload();
                A=1;
                sum=sum+1699;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=12;
                upload();
                A=1;
                sum=sum-1699;
                delay(1000);
               }
            }       
      } 
     if(buffer[0]=='0'&&buffer[1]=='8'&&buffer[2]=='7'&&buffer[3]=='1'&&buffer[4]=='8'&&buffer[5]=='2'&&buffer[6]=='9'&&buffer[7]=='1'&&buffer[8]=='2'&&buffer[9]=='1'&&buffer[10]=='4'&&buffer[11]=='9'&&buffer[12]=='3'&&buffer[13]=='9')
      { A=0;
        Serial.print("Philips CFL                   1 Piece         Rs.180    ");                                                   
     
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
          
           lcd.setCursor(0, 0);
           lcd.print("PHILIPS CFL");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.180");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=3;
                upload();
                A=1;
                sum=sum+180;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=13;
                upload();
                A=1;
                sum=sum-180;
                delay(1000);
               }
            }       
      }
     if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='1'&&buffer[4]=='4'&&buffer[5]=='2'&&buffer[6]=='5'&&buffer[7]=='0'&&buffer[8]=='1'&&buffer[9]=='1'&&buffer[10]=='6'&&buffer[11]=='6'&&buffer[12]=='9')
      { A=0;
        Serial.print("Camlin white board marker ink                   1 Piece         Rs.35    ");                                                   
      
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
          
           lcd.setCursor(0, 0);
           lcd.print("CAMLIN INK");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.35");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=4;
                upload();
                A=1;
                sum=sum+35;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=14;
                upload();
                A=1;
                sum=sum-35;
                delay(1000);
               }
            }       
      }  
     if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='1'&&buffer[4]=='1'&&buffer[5]=='5'&&buffer[6]=='7'&&buffer[7]=='0'&&buffer[8]=='0'&&buffer[9]=='0'&&buffer[10]=='2'&&buffer[11]=='1'&&buffer[12]=='4')
      { A=0;
        Serial.print("Good Night mat                   1 Piece         Rs.54    ");                                                   
      
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
           //lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("GOOD NIGHT MAT  ");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.54   ");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=5;
                upload();
                A=1;
                sum=sum+54;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=15;
                upload();
                A=1;
                sum=sum-54;
                delay(1000);
               }
            }       
      } 
    if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='8'&&buffer[4]=='0'&&buffer[5]=='0'&&buffer[6]=='7'&&buffer[7]=='4'&&buffer[8]=='6'&&buffer[9]=='6'&&buffer[10]=='1'&&buffer[11]=='1'&&buffer[12]=='4')
      { A=0;
        Serial.print("LIYO water bottle                    1 litre         Rs.20    ");                                                   
    
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
           
           lcd.setCursor(0, 0);
           lcd.print("WATER BOTTLE");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.20");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=6;
                upload();
                A=1;
                sum=sum+20;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=16;
                upload();
                A=1;
                sum=sum-20;
                delay(1000);
               }
            }       
      } 
  if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='1'&&buffer[4]=='4'&&buffer[5]=='1'&&buffer[6]=='4'&&buffer[7]=='0'&&buffer[8]=='0'&&buffer[9]=='0'&&buffer[10]=='0'&&buffer[11]=='6'&&buffer[12]=='3')
      { A=0;
        Serial.print("BIKANO aloo bhujia                   1 kg         Rs.222    ");                                                   
    
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
           
           lcd.setCursor(0, 0);
           lcd.print("ALOO BHUJIA     ");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.222  ");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=7;
                upload();
                A=1;
                sum=sum+222;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=17;
                upload();
                A=1;
                sum=sum-222;
                delay(1000);
               }
            }       
      } 
 if(buffer[0]=='8'&&buffer[1]=='9'&&buffer[2]=='0'&&buffer[3]=='4'&&buffer[4]=='0'&&buffer[5]=='4'&&buffer[6]=='3'&&buffer[7]=='9'&&buffer[8]=='0'&&buffer[9]=='1'&&buffer[10]=='0'&&buffer[11]=='1'&&buffer[12]=='5')
      { A=0;
        Serial.print("TATA salt                   1 kg         Rs.18    ");                                                   
       while(A==0)
       {
       Serial.println("Do you want to add this product to your cart");//lcd
           
           lcd.setCursor(0, 0);
           lcd.print("TATA SALT");
           lcd.setCursor(0, 1);
           lcd.print("PRICE = Rs.18");
           lcd.setCursor(16, 0);
           lcd.print("  @    @    @  ");
           lcd.setCursor(16, 1);
           lcd.print("ADD,LEAVE,REMOVE");
           if(digitalRead(4)==LOW)
              {
                Serial.println("Added to your Cart");//lcd
                a=8;
                upload();
                A=1;
                sum=sum+18;
                delay(1000);
               }
           if(digitalRead(5)==LOW)
              {
                 Serial.println("Not added to your Cart");//lcd
                A=1;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("NOT ADDED");
                lcd.setCursor(0, 1);
                lcd.print("PLZ PUT IT BACK");
                lcd.setCursor(16, 0);
                lcd.print("READY TO SCAN");
                lcd.setCursor(16, 1);
                lcd.print("SCAN BARCODE");
                delay(1000);
              }
           if(digitalRead(6)==LOW)
              {
                Serial.println("Deleted from your Cart");//lcd
                a=18;
                upload();
                A=1;
                sum=sum-18;
                delay(1000);
               }
            }       
      } 

       if(buffer[0]=='S'&&buffer[1]=='U'&&buffer[2]=='M')
       {
        a=20;
        Serial.print("check");
        upload();
        }
        
    int i=0;                                                             
    if (buffer[i] != 0) {                                                
      while(buffer[i] != 0) {
        Serial.print(buffer[i]);                                      
        buffer[i] = 0;                                                   
        i++;                                                             
      }                                                                  
    }                                                                    
    Serial.println("");
    bufferPos = 0;                                                                                                
  }                                                                      
  // Reset the SCAN_BREAK state if the byte was a normal one             
  if(dataValue != SCAN_BREAK){                                           
    breakActive = 0;                                                     
  }                                                                      
  dataValue = 0;
}
}




//fuction to get data from barcode
int dataRead() {
  byte val = 0;                                                          
  // Skip start state and start bit                                      
  while (digitalRead(clockPin));  // Wait for LOW.                       
  // clock is high when idle                                             
  while (!digitalRead(clockPin)); // Wait for HIGH.                      
  while (digitalRead(clockPin));  // Wait for LOW.                       
  for (int offset = 0; offset < 8; offset++) {                           
    while (digitalRead(clockPin));         // Wait for LOW               
    val |= digitalRead(dataPin) << offset; // Add to byte                
    while (!digitalRead(clockPin));        // Wait for HIGH              
  }                                                                      
// Skipping parity and stop bits down here.                            
  while (digitalRead(clockPin));           // Wait for LOW.              
  while (!digitalRead(clockPin));          // Wait for HIGH.             
  while (digitalRead(clockPin));           // Wait for LOW.              
  while (!digitalRead(clockPin));          // Wait for HIGH.             
  return val;                                                            
}



//fuction to send data at thingspeak
void upload()
{
    sprintf(buff, "https://api.thingspeak.com/update.json?api_key=DBJ4UDT4FSQOY77C&field1=%d",a);              // field x last Data
    Serial.println(buff);
    rest.get((const char*)buff);
    Serial.println("ARDUINO: send Update Data");
    lcd.clear();
    lcd.setCursor(0, 0);
    if(a==1||a==2||a==3||a==4||a==5||a==6||a==7||a==8||a==9||a==10)
    {lcd.print("ADDED TO CART^-^");
      }
      if(a==11||a==12||a==13||a==14||a==15||a==16||a==17||a==18||a==19)
      {lcd.print("REMOVED FRM CART");
        }
     if(a==20)
      {lcd.setCursor(0, 0);
      lcd.print("TOTAL AMOUNT=");
      lcd.setCursor(0, 1);
      lcd.print(sum);
        }
    lcd.setCursor(16, 0);
    lcd.print("SENDING DATA");
    for(int q=15;q>9;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(20, 1);
    lcd.print("SEC NEEDED");
    delay(1000);
      }
    for(int q=9;q>0;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(17, 1);
    lcd.print("   SEC NEEDED");
    delay(1000);
      }
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("READY TO SCAN");
    lcd.setCursor(0, 1);
    lcd.print("AGAIN");
    lcd.setCursor(16, 1);
    lcd.print("SCAN BARCODE");
  }

//(format to post the request)
//    sprintf(buff, "https://api.thingspeak.com/update.json?api_key=DBJ4UDT4FSQOY77C&field1=%d",a);              // field x last Data
//    Serial.println(buff);
//    rest.get((const char*)buff);
//    Serial.println("ARDUINO: send Update Data");
//    delay(15000);
  
