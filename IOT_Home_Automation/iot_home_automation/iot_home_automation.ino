#include <SoftwareSerial.h>
#include <espduino.h>
#include <rest.h>
#include <LiquidCrystal.h>


int a=1,b=0,c=0,d=0,e=0,f=0,A=0,count=0,val,trigger=1;
int sensor=A1;
float tempc;  //variable to store temperature in degree Celsius
float tempf;  //variable to store temperature in Fahreinheit 
float vout;  //temporary variable to hold sensor reading
SoftwareSerial espPort(10, 11); // RX, TX
ESP esp(&espPort, &Serial, 9);
REST rest(&esp);
boolean wifiConnected = false;

const int ESP8266_CHPD = 9;

const int relay1 =  2;
const int relay2 =  3;
const int relay3 =  4;
const int relay4 =  5;
int camera=6;

LiquidCrystal lcd(22, 24, 26, 28, 30, 32);

boolean data1 = false;

char response[266];
char buff[100];
String strId,strData,strCode;
String strData_Last1;

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
 lcd.print("WIFI Connected");
 lcd.setCursor(0, 1);
 lcd.print("System Connected");
 lcd.setCursor(16, 0);
 lcd.print("#************#");
 lcd.setCursor(16, 1);
 lcd.print("Ready to operate");
 delay(1000);
      wifiConnected = true;
    } else {
      wifiConnected = false;
    }    
  }
}

void setup() {

 lcd.begin(16, 4);
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Home Automation");
 lcd.setCursor(16, 0);
 lcd.print("By F.G. Engg.  ");
 delay(1000);
  

  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(camera, OUTPUT);
  
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,LOW);
  digitalWrite(relay3,LOW);
  digitalWrite(relay4,LOW);
  
  Serial.begin(19200);
  espPort.begin(19200);

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Hardware Checked");
 lcd.setCursor(0, 1);
 lcd.print("Founded---");
 lcd.setCursor(16, 0);
 lcd.print("WIFI Card, Temp.");
 lcd.setCursor(16, 1);
 lcd.print("Sensor & Switch");
 delay(1000);

 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("Hardware OK--");
 lcd.setCursor(0, 1);
 lcd.print("System OK--");
 lcd.setCursor(16, 0);
 lcd.print("Searching WIFI");
 lcd.setCursor(16, 1);
 lcd.print("For Connection");
 
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
 
 // esp.wifiConnect("not ur dads wifi","panditji9818228256@@%#$$");
 //esp.wifiConnect("Shivam","12121212");
 esp.wifiConnect("shivam","00000000");
  Serial.println("ARDUINO: system started");
 
  
}

void loop() {

loop_start:  
  
  esp.process();  

  
  if(wifiConnected) {
 temp();      
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("WIFI OK|Temp");
 lcd.print(tempc);
 lcd.setCursor(0, 1);
 lcd.print("#************#");
 lcd.setCursor(16, 0);
 lcd.print("Fetching Data");
 lcd.setCursor(16, 1);
 lcd.print("From Server");
 delay(500);

    
      sprintf(buff, "/channels/142811/fields/1/last");                                    // field x last Data
      Serial.println(buff);
           
      rest.get((const char*)buff);
      //Serial.println("ARDUINO: send get");

      (rest.getResponse(response, 266) == HTTP_STATUS_OK);
      
            Serial.println("ARDUINO: GET successful");
            Serial.println(response);
                              
            strId = "";
            strData = "";
            strCode = "";  
            getData();            //GET DATA                    
                 
          //Control OutPut
 lcd.setCursor(16, 0);
 lcd.print("Sucessfull      ");
 lcd.setCursor(16, 1);
 lcd.print("Taking Actions ");
 delay(500);
if (strId == "0"){
  digitalWrite(relay1,LOW);
  digitalWrite(relay2,LOW);
  digitalWrite(relay3,LOW);
  digitalWrite(relay4,LOW);
  data1 = true;
  Serial.println("All switchs Off");
  lcd.setCursor(16, 0);
  lcd.print("All switchs Off ");
  lcd.setCursor(16, 1);
  lcd.print("yah Energy Saved");
  delay(500);
  b=0;
  c=0;
  d=0;
  e=0;
  f=0;
          }

if (strId == "11"){
  digitalWrite(relay1,HIGH);
  digitalWrite(relay2,HIGH);
  digitalWrite(relay3,HIGH);
  digitalWrite(relay4,HIGH);
  data1 = true;
  Serial.println("ALL ON");
  b=1;
  c=1;
  d=1;
  e=1;
  f=1;
  lcd.setCursor(16, 0);
  lcd.print("All switchs On  ");
  lcd.setCursor(16, 1);
  lcd.print("Max Energy Used ");
  delay(500);
          }

if (strId == "1"){
  digitalWrite(relay1,HIGH);
  data1 = true;
  Serial.println("1 ON"); 
  lcd.setCursor(16, 0);
  lcd.print("Switch 1 ON     ");
  lcd.setCursor(16, 1);
  lcd.print("1 -> (@)        ");
  delay(500);
  b=1;
   }

if (strId == "2"){
  digitalWrite(relay1,LOW);
  data1 = true;
  Serial.println("1 OFF"); 
  lcd.setCursor(16, 0);
  lcd.print("Switch 1 OFF    ");
  lcd.setCursor(16, 1);
  lcd.print("1 -> ( )        ");
  delay(500);
  b=0;
  }

if (strId == "3"){
  digitalWrite(relay2,HIGH);
  data1 = true;
  Serial.println("2 ON");
  lcd.setCursor(16, 0);
  lcd.print("Switch 2 ON     ");
  lcd.setCursor(16, 1);
  lcd.print("2 -> (@)        ");
  delay(500);
  c=1;
  }

if (strId == "4"){
  digitalWrite(relay2,LOW);
  data1 = true;
  Serial.println("2 OFF"); 
  lcd.setCursor(16, 0);
  lcd.print("Switch 2 OFF    ");
  lcd.setCursor(16, 1);
  lcd.print("2 -> ( )        ");
  delay(500);
  c=0;
  }

if (strId == "5"){
  digitalWrite(relay3,HIGH);
  data1 = true;
  Serial.println("3 ON");
  lcd.setCursor(16, 0);
  lcd.print("Switch 3 ON     ");
  lcd.setCursor(16, 1);
  lcd.print("3 -> (@)        ");
  delay(500);
  d=1;
  }

if (strId == "6"){
  digitalWrite(relay3,LOW);
  data1 = true;
  Serial.println("3 OFF"); 
  lcd.setCursor(16, 0);
  lcd.print("Switch 3 OFF    ");
  lcd.setCursor(16, 1);
  lcd.print("3 -> ( )        ");
  delay(500);
  d=0;
  }

if (strId == "7"){
  digitalWrite(relay4,HIGH);
  data1 = true;
  Serial.println("4 ON");
  lcd.setCursor(16, 0);
  lcd.print("Switch 4 ON     ");
  lcd.setCursor(16, 1);
  lcd.print("4 -> (@)        ");
  delay(500);
  e=1;
  }

if (strId == "8"){
  digitalWrite(relay4,LOW);
  data1 = true;
  Serial.println("4 OFF");
  lcd.setCursor(16, 0); 
  lcd.print("Switch 4 OFF    ");
  lcd.setCursor(16, 1);
  lcd.print("4 -> ( )        ");
  delay(500);
  e=0;
}

if (strId == "20")
{
  trigger=1;
  lcd.setCursor(16, 0); 
  lcd.print("Trigger Reset   ");
  lcd.setCursor(16, 1);
  lcd.print("                ");
  delay(500);
  }

if (strId == "22")
{
  digitalWrite(camera,LOW);
  data1 = true;
  Serial.println("4 OFF");
  lcd.setCursor(16, 0); 
  lcd.print("Camera Off      ");
  lcd.setCursor(16, 1);
  lcd.print("                ");
  delay(500);
  }


  if (strId == "21")
{
  digitalWrite(camera,HIGH);
  data1 = true;
  lcd.setCursor(16, 0); 
  lcd.print("Camera On       ");
  lcd.setCursor(16, 1);
  lcd.print("                ");
  delay(500);
  }

count++;
if(count==15)
{
  tempupdate();
  count=0;
}
upload();

if(tempc>55 && trigger<=3)
{
alertme();
trigger++;
  }

} //if WifiConnected
} 


  
  void getData(){
   int i=0,j=0,k=0; 
     
         for (i = 0; i < 10; i++){         
        
          
          if((response[i] == '\r') || (response[i] == '\n')) { 
          }
          else{          
             strId += response[i];       
          }
          
          if (response[i] == '\n'){ 
               i++;         
               break;
          }                    
        }
        
        Serial.println("");
        Serial.print("ID : ");
        Serial.print(strId);
        }


    void upload()
    {
    sprintf(buff, "https://api.thingspeak.com/update.json?api_key=EYA65YD40PYDG0EZ&field1=%d&field2=%d&field3=%d&field4=%d&field6=%d",b,c,d,e,a);              // field x last Data
    Serial.println(buff);
    rest.get((const char*)buff);
    Serial.println("ARDUINO: send Update Data");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WIFI OK|Temp");
    lcd.print(tempc);
    lcd.setCursor(0, 1);
    lcd.print("#************#");
    lcd.setCursor(16, 0);
    lcd.print("Sending Feedback");
    for(int q=15;q>9;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(18, 1);
    lcd.print("  Sec Needed");
    delay(1000);
      }
    for(int q=9;q>0;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(17, 1);
    lcd.print("   Sec Needed");
    delay(1000);
      }
    }

void temp()
{
vout=analogRead(sensor);
vout=(vout*500)/1023;
tempc=vout; // Storing value in Degree Celsius
tempf=(vout*1.8)+32; // Converting to Fahrenheit 
}

void tempupdate()
{
vout=analogRead(sensor);
vout=(vout*500)/1023;
tempc=vout; // Storing value in Degree Celsius
tempf=(vout*1.8)+32; // Converting to Fahrenheit 
int T=tempc;
Serial.print("in DegreeC=");
Serial.print("\t");
Serial.print(tempc);

sprintf(buff, "https://api.thingspeak.com/update.json?api_key=EYA65YD40PYDG0EZ&field5=%d",T);              // field x last Data
Serial.println(buff);
rest.get((const char*)buff);
Serial.println("ARDUINO: send Update Data");

    lcd.setCursor(16, 0);
    lcd.print("Sending Temp.   ");
    for(int q=15;q>9;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(18, 1);
    lcd.print("  Sec Needed");
    delay(1000);
      }
    for(int q=9;q>0;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(17, 1);
    lcd.print("   Sec Needed");
    delay(1000);
      }

  }

 void alertme()
 {
sprintf(buff, "https://api.thingspeak.com/update.json?api_key=5HB9M1VA4AK42YKA&field1=1");              // field x last Data
Serial.println(buff);
rest.get((const char*)buff);
Serial.println("ARDUINO: send Update Data");

    lcd.setCursor(16, 0);
    lcd.print("Sending Alert   ");
    for(int q=15;q>9;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(18, 1);
    lcd.print("  Sec Needed");
    delay(1000);
      }
    for(int q=9;q>0;q--)
    {
    lcd.setCursor(16, 1);
    lcd.print(q);
    lcd.setCursor(17, 1);
    lcd.print("   Sec Needed");
    delay(1000);
      }
  }
