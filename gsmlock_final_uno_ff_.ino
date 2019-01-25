#include <TinyGPS++.h>

#include <stdlib.h>
#include <string.h>

#include <SoftwareSerial.h>
SoftwareSerial gsmserial(4, 5);
//SoftwareSerial gpsserial(2, 3);
//const int rs = 45, en
///= 46, d4 = 48, d5 = 49, d6 = 50, d7 = 51;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
TinyGPSPlus gps; // create gps object

float startlat, startlong, stoplat, stoplong;
bool check = false, numsent = false, loc = false;




int main_state = 0;
int sec_state = 0;
int ter_state = 0;

int c1 = 10;
int c2 = 11;
int c3 = 12;
int c4 = 13;

int r1 = 6;
int r2 = 7;
int r3 = 8;
int r4 = 9;

char c, cl = '+ClIP: ';
String w = "", wp = "";
String gsm_no  = "1234567890";

//unsigned long OTP, otpc1 = 123, otpc2 = 986;
unsigned long otpc1 = 123, otpc2 = 986;
String OTP,eotp="";
//unsigned long enteredOTP = 0;
bool expireOTP = true;

int flag = 0;



void ReceiveCall();
void SendOTP();
void OTP_gen();

void callscreen();
void EnterOTP();
void start_GPS();
void stop_GPS();




void setup() {
  //Serial.begin(9600);
  //gpsserial.begin(9600);
  gsmserial.begin(9600);
  


  pinMode(A3, OUTPUT);                 //MAIN LOCK
  pinMode(A2, OUTPUT);                   //SECONDARY LOCK
  pinMode(A1, OUTPUT);                  //TERTIARY LOCK
  pinMode(A0,OUTPUT);
  
  pinMode(A4,OUTPUT);
  pinMode(A5,OUTPUT);
  
  pinMode(r1, INPUT_PULLUP);
  pinMode(r2, INPUT_PULLUP);
  pinMode(r3, INPUT_PULLUP);
  pinMode(r4, INPUT_PULLUP);
  pinMode(c1, OUTPUT);
  pinMode(c2, OUTPUT);
  pinMode(c3, OUTPUT);
  pinMode(c4, OUTPUT);

  

  //lcd.begin(16, 2);

  digitalWrite(A3, LOW);
  digitalWrite(A2, LOW);
  digitalWrite(A1, LOW);
  digitalWrite(A0, LOW);
  
  //callscreen();

  //gsmserial.println("AT");
  //delay(400);
}

void loop() {
  if (expireOTP == true) {
    delay(10000);
    digitalWrite(A5, HIGH);
    digitalWrite(A4, LOW);
    delay(2000);
    
    ReceiveCall();
    EnterOTP();
    if (check == true && expireOTP == false)
    {
      digitalWrite(c4, LOW);
      //      lcd.clear();
      //      lcd.setCursor(0,0);
      //      lcd.print(" MAIN: UNLOCKED");
      digitalWrite(A4, HIGH);
      digitalWrite(A5, LOW);
      digitalWrite(A3, HIGH);
      main_state = !main_state;
      //start_GPS();         //start location monitoring
      while (digitalRead(A3) == HIGH)
      { if (digitalRead(r2) == LOW)           // open /close sec lock
        { delay(200);
          digitalWrite(A2, (sec_state) ? HIGH : LOW);
          sec_state = !sec_state;
        }
        if (digitalRead(r3) == LOW)          // open /close sec lock
        { delay(200);
          digitalWrite(A1, (ter_state) ? HIGH : LOW);
          digitalWrite(A0, (ter_state) ? HIGH : LOW);
          ter_state = !ter_state;
        }
        if (digitalRead(r1) == LOW)            //  call for main lock to close
        { delay(200);
          if (digitalRead(A2) == LOW && digitalRead(A1) == LOW) //  check if other locks are closed or not
          { digitalWrite(A3, LOW);     // close main_lock
            //stop_GPS();
            //                          lcd.clear();
            //                          lcd.setCursor(3,0);
            //                          lcd.print("MAIN LOCKED!");
            //stop_GPS();
            
            digitalWrite(A4, LOW);

            delay(5000);
            digitalWrite(A5, HIGH);
            //t1.stop(locatevent);     //stop sending location to the admin
            
          }
        }
        expireOTP = true;
        check=false;
      }
    }
    if (flag == 1 )
    {
      //     lcd.clear();
      //      lcd.setCursor(3,0);
      //      lcd.print("TIME OUT!!");
      //      lcd.setCursor(4,1);
      //      lcd.print("CALL AGAIN");
      delay(2000);
      //Serial.println("TIME OUT!!");
      gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
      delay(1000);  // Delay of 1000 milli seconds or 1 second
      gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
      delay(1000);
      gsmserial.println("TIME OUT!!! Call Again!");// The SMS text you want to send
      delay(100);
      gsmserial.println((char)26);// ASCII code of CTRL+Z
      delay(1000);
      digitalWrite(A5, LOW);
      delay(500);
      digitalWrite(A5, HIGH);
      expireOTP = true;
      flag = 0;

    }
  }
}

void start_GPS() {
//  while (true) {
//    while (gpsserial.available()) {
//      gps.encode(gpsserial.read());
//      if (gps.location.isUpdated()) {
//
//        startlat = gps.location.lat();
//        String latitude = String(startlat);
//        startlong = gps.location.lng();
//        String longitude = String(startlong);
//        Serial.println("Starting Position:");
//        Serial.println("Latitude= " + latitude);
//        //Serial.print(gps.location.lat(), 6);
//        Serial.println(" Longitude= " + longitude);
//        //Serial.println(gps.location.lng(), 6);
//        gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//        delay(1000);  // Delay of 1000 milli seconds or 1 second
//        gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
//        delay(1000);
//        gsmserial.println("Starting Location:- Latitude:" + latitude + " ,Longitude:" + longitude); // The SMS text you want to send
//        delay(100);
//        gsmserial.println((char)26);// ASCII code of CTRL+Z
//        delay(1000);
//        loc = true;
//        break;
//      }
//    }
//    if (loc == true) {
//      loc = false;
//      break;
//    }
//
//  }
//  while (true){
//  while(Serial.available()){
//    String strtest=Serial.readString();
//        gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//        delay(1000);  // Delay of 1000 milli seconds or 1 second
//        gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
//        delay(1000);
//        gsmserial.println("UNLOCKED!! Starting Location:-" + strtest); // The SMS text you want to send
//        delay(100);
//        gsmserial.println((char)26);// ASCII code of CTRL+Z
//        delay(1000);
//        loc = true;
//      }
//    if (loc == true) {
//      loc = false;
//      break;
//    }
//
//  }
}

void stop_GPS() {
//  while (true) {
//    while (gpsserial.available()) {
//      gps.encode(gpsserial.read());
//      if (gps.location.isUpdated()) {
//
//        stoplat = gps.location.lat();
//        String latitude = String(stoplat);
//        stoplong = gps.location.lng();
//        String longitude = String(stoplong);
//        Serial.println("Starting Position:");
//        Serial.println("Latitude= " + latitude);
//        //Serial.print(gps.location.lat(), 6);
//        Serial.println(" Longitude= " + longitude);
//        double finaldist = gps.distanceBetween(startlat, startlong, stoplat, stoplong);
//        String final_dist = String(finaldist);
//        Serial.println("Final Distance:" + final_dist);
//        //Serial.println(gps.location.lng(), 6);
//        gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//        delay(1000);  // Delay of 1000 milli seconds or 1 second
//        gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
//        delay(1000);
//        gsmserial.println("Stopping Location:- Latitude:" + latitude + " ,Longitude:" + longitude + " ; Final Distance:" + final_dist); // The SMS text you want to send
//        delay(100);
//        gsmserial.println((char)26);// ASCII code of CTRL+Z
//        delay(1000);
//        loc = true;
//        break;
//      }
//    }
//while (true){
//  while(Serial.available()){
//    String strtest=Serial.readString();
//        gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
//        delay(1000);  // Delay of 1000 milli seconds or 1 second
//        gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
//        delay(1000);
//        gsmserial.println("LOCKED!! Stopping Location:-" + strtest); // The SMS text you want to send
//        delay(100);
//        gsmserial.println((char)26);// ASCII code of CTRL+Z
//        delay(1000);
//        loc = true;
//      }
//    if (loc == true) {
//      loc = false;
//      break;
//    }

//  }
}


void OTP_gen()
{
  OTP = rand() % 100000 + 100000;
  expireOTP = false;
}


void SendOTP() {
  //  gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  //  delay(1000);  // Delay of 1000 milli seconds or 1 second
  //  gsmserial.println("AT+CMGS="+gsm_no+"\r"); // Replace x with mobile number
  //  delay(1000);
  //  gsmserial.println("OTP is:"+OTP);// The SMS text you want to send
  //  delay(100);
  //  gsmserial.println((char)26);// ASCII code of CTRL+Z
  //  delay(1000);
  //  gsmserial.println("AT+CMGF=0");
  //  Serial.println(OTP);
}

void ReceiveCall()
{

  // wait for call and get the phoneno. of the user and update in 'user_no'
  gsmserial.println("AT");
  delay(300);
  //Serial.print(gsmserial.read());

  while (gsmserial.available()) {
    otpc1++;
    otpc2++;
    otpc1*=otpc2;
    ///Serial.println(1);
    if (gsmserial.read() == cl) {
      for (int i = 0; i < 15; i++) {
        c = gsmserial.read();
        w = w + c;
      }
      if (w == wp) {
        gsm_no = wp;
        //Serial.println(gsm_no);
        gsmserial.println("ATH");
        gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
        delay(1000);  // Delay of 1000 milli seconds or 1 second
        gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
        delay(1000);
        //OTP = otpc1 % 1000000;
        expireOTP = false;
        OTP = String(otpc1%1000000);
        gsmserial.println("OTP is:" + OTP); // The SMS text you want to send
        delay(100);
        gsmserial.println((char)26);// ASCII code of CTRL+Z
        delay(1000);
        //Serial.println(OTP);
        otpc1 = 123;
        otpc2 = 986;
        //Serial.println(gsmserial.read());
        numsent = true;
      }
      wp = w;
      w = "";
    }
  }

  if (numsent == false) {
    ReceiveCall();
  }

}


void callscreen() {
//  //    while(!gsmserial.available()){
//  //    lcd.clear();
//  //    lcd.setCursor(0,0);
//  //    lcd.print("CALL:1234567890");
//  //    lcd.setCursor(2,1);
//  //    lcd.print("TO UNLOCK!");
//  //    }
}

void EnterOTP() {

  int useOTP[6];
  int count = 0;
  int tcount = 0;
  int i = 0;
  eotp="";
  //enteredOTP=0;
  //  lcd.clear();
  //  lcd.setCursor(0,0);
  //  lcd.print("ENTER OTP:");
  //  lcd.setCursor(0,1);
  //  lcd.print("PRESS OK");
  //  lcd.setCursor(10,0);

  while (flag == 0 && check == false && expireOTP == false) {
    digitalWrite(c1, LOW);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    if (digitalRead(r1) == LOW)
    { delay(200);
      //useOTP[i] = 1;
      eotp+="1";
      count++;
      i++;

      //lcd.print("1");
    }
    if (digitalRead(r2) == LOW)
    { delay(200);
      //useOTP[i] = 4;
      eotp+="4";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("4");
    }
    if (digitalRead(r3) == LOW)
    { delay(200);
      //useOTP[i] = 7;
      eotp+="7";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("7");
    }

    digitalWrite(c1, HIGH);
    digitalWrite(c2, LOW);
    digitalWrite(c3, HIGH);
    digitalWrite(c4, HIGH);
    if (digitalRead(r1) == LOW)
    { delay(200);
      //useOTP[i] = 2;
      eotp+="2";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("2");
    }
    if (digitalRead(r2) == LOW)
    { delay(200);
      //useOTP[i] = 5;
      eotp+="5";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("5");
    }
    if (digitalRead(r3) == LOW)
    { delay(200);
      //useOTP[i] = 8;
      eotp+="8";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("8");
    }
    if (digitalRead(r4) == LOW)
    { delay(200);
      //useOTP[i] = 0;
      eotp+="0";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("0");
    }

    digitalWrite(c1, HIGH);
    digitalWrite(c2, HIGH);
    digitalWrite(c3, LOW);
    digitalWrite(c4, HIGH);
    if (digitalRead(r1) == LOW)
    { delay(200);
      //useOTP[i] = 3;
      eotp+="3";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("3");
    }
    if (digitalRead(r2) == LOW)
    { delay(200);
      //useOTP[i] = 6;
      eotp+="6";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("6");
    }
    if (digitalRead(r3) == LOW)
    { delay(200);
      //useOTP[i] = 9;
      eotp+="9";
      count++; i++;
      //lcd.setCursor(10+i,0);
      //lcd.print("9");
    }
    if (count == 6)
    {

      //enteredOTP = useOTP[0] * 100000  +  useOTP[1] * 10000 + useOTP[2] * 1000 + useOTP[3] * 100 + useOTP[4] * 10 + useOTP[5];

      if (eotp == OTP)
        check = true;


      else {
        check = false;
        //lcd.clear();
        //lcd.setCursor(3,0);

        //lcd.print("CALL AGAIN!!");
        delay(2000);
        //lcd.print("WRONG OTP!");
        //lcd.setCursor(2,1);
        //Serial.println("CALL AGAAIN!!!!!!");
        gsmserial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
        delay(1000);  // Delay of 1000 milli seconds or 1 second
        
        gsmserial.println("AT+CMGS=" + gsm_no + "\r"); // Replace x with mobile number
        delay(1000);
        gsmserial.println("Wrong OTP, CALL AGAIN!!! "+eotp);// The SMS text you want to send
        delay(100);
        gsmserial.println((char)26);// ASCII code of CTRL+Z
        delay(1000);
        
        digitalWrite(A5, LOW);
        delay(500);
        digitalWrite(A5, HIGH);

        count = 0;
        expireOTP = true;
        //        lcd.clear();
        //        lcd.setCursor(0,0);
        //        lcd.print("ENTER OTP:");
        //        lcd.setCursor(0,1);
        //        lcd.print("PRESS OK");
        //        lcd.setCursor(10,0);
      }

    }

    delay(100);
    tcount++;
    if (tcount >= 1200)
      flag = 1;
  }
//  digitalWrite(c1, LOW);
//  digitalWrite(c2, LOW);
//  digitalWrite(c3, HIGH);
//  digitalWrite(c4, LOW);
//
//  enteredOTP = useOTP[0] * 100000  +  useOTP[1] * 10000 + useOTP[2] * 1000 + useOTP[3] * 100 + useOTP[4] * 10 + useOTP[5];
//
//  if (enteredOTP == OTP)
//    check = true;
//  else
//    check = false;


}
