
#include <ESP32Servo.h>

class Motor
{
  Servo servo;
  int pin;

public:

  Motor(int servo_pin){
    pin = servo_pin;
    Attach();
  }

  void Attach(){  servo.attach(pin);  }
  void Detach(){  servo.detach();     }
  
  void Open()   {  servo.write(180);  }
  void Close()  {  servo.write(0);   }
};