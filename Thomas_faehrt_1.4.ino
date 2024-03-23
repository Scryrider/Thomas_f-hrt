
// Satisfy the IDE, which needs to see the include statment in the ino too.
#include <PS4BT.h>
#include <Servo.h>
#include <SPI.h>
#include <PS4Parser.h>


#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif


bool Paired = false;


USB Usb;
BTD Btd(&Usb);
PS4BT PS4(&Btd, PAIR);

Servo Lenkung;
Servo Fahren;
Servo Tempomat;


int offset = 0;
int steer = 127;
int drive = 127;
int speed = 30;

int Hupe = 3;
const int Bllinks = 2;
int links = LOW;
const int Blrechts = 4;
int rechts = LOW;
const int trigger = L1;


bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

void setup() {
  Serial.begin(115200);
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1); // Halt
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));
  pinMode(5, OUTPUT);
  Lenkung.attach(5);

  pinMode(6, OUTPUT);
  Fahren.attach(6);

  pinMode(9, OUTPUT);
  Tempomat.attach(9);

  pinMode(Hupe, OUTPUT);
  pinMode(Bllinks, OUTPUT);
  pinMode(Blrechts, OUTPUT);

  



}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    Paired = true;
     if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117)
    {
      Serial.print(F("\r\nLeftHatX: "));
      Serial.print(PS4.getAnalogHat(LeftHatX));
      Serial.print(F("\tLeftHatY: "));
      Serial.print(PS4.getAnalogHat(LeftHatY));
      Serial.print(F("\tRightHatX: "));
      Serial.print(PS4.getAnalogHat(RightHatX));
      Serial.print(F("\tRightHatY: "));
      Serial.print(PS4.getAnalogHat(RightHatY));


      steer = PS4.getAnalogHat(LeftHatX); // read joystick

     // if (PS4.getButtonClick(RIGHT))
     // { // trim right
     //   offset += 1;
     // }

     // if (PS4.getButtonClick(LEFT))
     // { // trim left
     //   offset -= 1;
     // }

      //steer = steer + offset; // incorporate trim into steer command
      if (steer > 200) steer = 200; // enforce upper limit
      if (steer < 74) steer = 74 ; // enforce lower limit
      Lenkung.write(map(steer, 0, 255, 0, 180)); // write steer command
     
    }
    else
    {
      Lenkung.write(90);
    }


    if (PS4.getAnalogHat(LeftHatX) > 137 || PS4.getAnalogHat(LeftHatX) < 117 || PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117 || PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117 || PS4.getAnalogHat(RightHatY) > 137 || PS4.getAnalogHat(RightHatY) < 117)
    {
      drive = PS4.getAnalogHat(RightHatY);
      if (drive > 170) drive = 170; // enforce upper limit
      if (drive < 84) drive = 84; // enforce lower limit

      Fahren.write(map(drive, 0, 255, 180,  0)); // write steer command

    }
    else
    {
      Fahren.write(90);
    }

    if (PS4.getAnalogButton(L2) || PS4.getAnalogButton(R2)) { // These are the only analog buttons on the PS4 controller


      Serial.print(F("\r\nL2: "));
      Serial.print(PS4.getAnalogButton(L2));
      Serial.print(F("\tR2: "));
      Serial.print(PS4.getAnalogButton(R2));



    }

    if (PS4.getAnalogButton(L2) != oldL2Value || PS4.getAnalogButton(R2) != oldR2Value) // Only write value if it's different

      oldL2Value = PS4.getAnalogButton(L2);
      oldR2Value = PS4.getAnalogButton(R2);

    if (PS4.getButtonClick(PS))
      Serial.print(F("\r\nPS"));

    if (PS4.getButtonClick(TRIANGLE))

    {
      digitalWrite(Bllinks, !digitalRead(Bllinks));
      digitalWrite(Blrechts, !digitalRead(Blrechts));


      Serial.print(F("\r\nTriangle"));
    }



    if (PS4.getButtonClick(CIRCLE)) {
      Serial.print(F("\r\nCircle"));
    }
    if (PS4.getButtonClick(CROSS)) {

      digitalWrite(Hupe, HIGH);
      delay(300);
      digitalWrite(Hupe, LOW);
      Serial.print(F("\r\nCross"));
    }
    else
    {
      digitalWrite(Hupe, LOW);
    }

    if (PS4.getButtonClick(SQUARE)) 
    {
      
      Serial.print(PS4.getBatteryLevel());
      Serial.print(F("\r\nSquare"));

    }



    if (PS4.getButtonClick(UP)) {
      Serial.print(F("\r\nUp"));
    
      speed = speed+5;
      if (speed > 180) speed = 180;
      Tempomat.write(speed);
      Serial.print(speed);

      if (speed < 60) PS4.setLed(Red);
      else if (60 <= speed <= 120) PS4.setLed(Yellow);
      if (120 < speed) PS4.setLed(Green);
      

      
 
    } if (PS4.getButtonClick(RIGHT)) {
      Serial.print(F("\r\nRight"));
      
    } if (PS4.getButtonClick(DOWN)) {
      Serial.print(F("\r\nDown"));
      
      speed = speed-5;
      if (speed < 0) speed = 0;
      Tempomat.write(speed);
      Serial.print(speed);

      if (speed < 60) PS4.setLed(Red);
      else if (60 < speed <= 120) PS4.setLed(Yellow);
      if (120 < speed) PS4.setLed(Green);
      
    
    } if (PS4.getButtonClick(LEFT)) {
      Serial.print(F("\r\nLeft"));
      
    }


    if (PS4.getButtonClick(L1))
    {
      digitalWrite(Bllinks, !digitalRead(Bllinks));
      Serial.print(F("\r\nL1"));
    }

    if (PS4.getButtonClick(L3))
      Serial.print(F("\r\nL3"));


    if (PS4.getButtonClick(R1))
    {

      digitalWrite(Blrechts, !digitalRead(Blrechts));
      Serial.print(F("\r\nR1"));
    }

    if (PS4.getButtonClick(R3))
      Serial.print(F("\r\nR3"));








}
  //Out of range condition goes here!

}
