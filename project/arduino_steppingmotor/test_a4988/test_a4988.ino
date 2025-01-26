#include <Arduino.h>
#include <AccelStepper.h>
#define motorInterfaceType 1

#define BOTTOM_SPEED 100

const int dirPin_Top_1 = 4;
const int stepPin_Top_1 = 3;

AccelStepper stepper_Top_1(motorInterfaceType, stepPin_Top_1, dirPin_Top_1);

void setup()
{
  // put your setup code here, to run once:
  
  stepper_Top_1.setMaxSpeed(500.0);
  stepper_Top_1.setAcceleration(20.0);
  stepper_Top_1.moveTo(55000);
  stepper_Top_1.setSpeed(500);
}

void loop()
{
  // put your main code here, to run repeatedly:
  
  if (stepper_Top_1.distanceToGo() == 0)
  {
    stepper_Top_1.moveTo(-stepper_Top_1.currentPosition());
  }

  stepper_Top_1.run();

}
