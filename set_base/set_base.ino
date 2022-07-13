#include <Servo.h>

#define   PIN_BASE    9
#define   PIN_HEAD    10
#define   ANGLE_BASEPOINT   90

Servo base,head;

void setup() {
  base.attach(PIN_BASE);
  head.attach(PIN_HEAD);
}

void loop() {
  base.write(ANGLE_BASEPOINT);
  head.write(ANGLE_BASEPOINT);
  delay(50);
  
}
