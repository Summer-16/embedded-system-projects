// VARIABLE DECLARATION

char input[12];
int count = 0, ir = 0, check = 0, m1 = 100, m2 = 100, s1, s2, s3;

// SETUP FUNCTION

void setup()
{
  Serial.begin(9600); // START SERIAL AT BAUD RATE OF 9600 BITS/SEC
  pinMode(12, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
}

// LOOP FUNCTION

void loop()
{
  Serial.println("Touch your card");
  if (Serial.available()) // CHECK FOR AVAILABILITY OF SERIAL DATA
  {
    count = 0; // Reset the counter to zero
    /* Keep reading Byte by Byte from the Buffer till the RFID Reader Buffer is	empty 
		   or till 12 Bytes (the ID size of our Tag) is read */
    while (Serial.available() && count < 12)
    {
      input[count] = Serial.read(); // Read 1 Byte of data and store it in the input[] variable
      count++;                      // increment counter
      delay(5);
    }
    // PRINTING RFID TAG
    for (int i = 0; i < 12; i++)
      Serial.print(input[i]);
    Serial.println();
    tone(12, 2000, 1000);
    check = 1;
    Serial.println("Place your glass");
    delay(3000);
  }
  ir = digitalRead(A0);
  if (check == 1)
  {
    Serial.println("Check1");
    if (input[10] == '1' && ir == 1 && m1 > 2) //Brown
    {
      Serial.println("Check2");
      while (1)
      {
        Serial.println("Check While");
        Serial.println("Mr. Brown");
        Serial.println("Balance Rs. ");
        Serial.print(m1);
        s1 = digitalRead(6);
        s2 = digitalRead(7);
        s3 = digitalRead(8);
        if (s1 == 0)
        {
          Serial.println("Half Glass");
          digitalWrite(9, HIGH);
          Serial.println("Pump ON");
          delay(3000);
          digitalWrite(9, LOW);
          Serial.println("Pump OFF");
          m1 = m1 - 2;
          Serial.println("Balance Rs. ");
          Serial.print(m1);
          delay(5000);
          break;
        }
        else if (s2 == 0)
        {
          Serial.println("Full Glass");
          digitalWrite(9, HIGH);
          Serial.println("Pump ON");
          delay(4000);
          digitalWrite(9, LOW);
          Serial.println("Pump OFF");
          m1 = m1 - 2;
          Serial.println("Balance Rs. ");
          Serial.print(m1);
          delay(5000);
          break;
        }
        else if (s3 == 0)
        {
          Serial.println("Cancel");
          Serial.println("Balance Rs. ");
          Serial.print(m1);
          delay(5000);
          break;
        }
      }
    }

    /************************************************************************/
    else if (input[10] == '4' && ir == 1 && m2 > 2) // charls
    {
      Serial.println("Check2");
      while (1)
      {
        Serial.println("Check While");
        Serial.println("Mr. Charls");
        Serial.println("Balance Rs. ");
        Serial.print(m2);
        s1 = digitalRead(6);
        s2 = digitalRead(7);
        s3 = digitalRead(8);
        if (s1 == 0)
        {
          Serial.println("Half Glass");
          digitalWrite(9, HIGH);
          Serial.println("Pump ON");
          delay(3000);
          digitalWrite(9, LOW);
          Serial.println("Pump OFF");
          m2 = m2 - 2;
          Serial.println("Balance Rs. ");
          Serial.print(m2);
          delay(5000);
          break;
        }
        else if (s2 == 0)
        {
          Serial.println("Full Glass");
          digitalWrite(9, HIGH);
          Serial.println("Pump ON");
          delay(4000);
          digitalWrite(9, LOW);
          Serial.println("Pump OFF");
          m2 = m2 - 2;
          Serial.println("Balance Rs. ");
          Serial.print(m2);
          delay(5000);
          break;
        }
        else if (s3 == 0)
        {
          Serial.println("Cancel");
          Serial.println("Balance Rs. ");
          Serial.print(m2);
          delay(5000);
          break;
        }
      }
    }
    /********************************************************************************************/
    else if (input[10] == '7') //Admin
    {
      Serial.println("Touch Employee Card");
      while (1)
      {
        if (Serial.available()) // CHECK FOR AVAILABILITY OF SERIAL DATA
        {
          count = 0; // Reset the counter to zero
          /* Keep reading Byte by Byte from the Buffer till the RFID Reader Buffer is empty 
       or till 12 Bytes (the ID size of our Tag) is read */
          while (Serial.available() && count < 12)
          {
            input[count] = Serial.read(); // Read 1 Byte of data and store it in the input[] variable
            count++;                      // increment counter
            delay(5);
          }
        }
        break;
      }
      if (input[10] == '7')
      {
        Serial.println("Mr. Brown");
        while (1)
        {
          s1 = digitalRead(6);
          s2 = digitalRead(7);
          s3 = digitalRead(8);
          if (s1 == 0)
          {
            Serial.println("Rs. 50 is added");
            m1 = m1 + 50;
          }
          else if (s2 == 0)
          {
            Serial.println("Rs. 50 is deducted");
            m1 = m1 - 50;
          }
          else if (s3 == 0)
          {
            Serial.println("Exit");
            delay(3000);
            break;
          }
        }
      }
      else if (input[10] == '4')
      {
        Serial.println("Mr. Charls");
        while (1)
        {
          s1 = digitalRead(6);
          s2 = digitalRead(7);
          s3 = digitalRead(8);
          if (s1 == 0)
          {
            Serial.println("Rs. 50 is added");
            m2 = m2 + 50;
          }
          else if (s2 == 0)
          {
            Serial.println("Rs. 50 is deducted");
            m2 = m2 - 50;
          }
          else if (s3 == 0)
          {
            Serial.println("Exit");
            delay(3000);
            break;
          }
        }
      }
    }
    else
    {
      Serial.println("Check your Balance or Glass");
      delay(5000);
    }
    check = 0;
  }
}
