#include <ESP32Servo.h>

class ServoMotor
{
  Servo servo;
  int pin;

public:

  ServoMotor(int servo_pin){
    pin = servo_pin;
    Attach();
  }

  void Attach(){  servo.attach(pin);  }
  void Detach(){  servo.detach();     }
  
  void Open()   {  servo.write(180);  }
  void Close()  {  servo.write(0);   }
};