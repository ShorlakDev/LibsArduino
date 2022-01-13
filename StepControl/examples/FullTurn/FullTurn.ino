#include <StepControl.h>

StepControl pasoApaso = StepControl(9,10,11,12); //Declarar objeto StepControl

void setup() {
}

void loop() {
  pasoApaso.clockwiseStep(2048);
  delay(2000);
  pasoApaso.counterclockwiseStep(2048);
  delay(2000);
}
