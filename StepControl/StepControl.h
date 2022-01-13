#ifndef STEP_CONTROL_H
#define STEP_CONTROL_H
#include <Arduino.h>
class StepControl{
  public:
  StepControl(uint8_t _IN1, uint8_t _IN2, uint8_t _IN3, uint8_t _IN4);
  void clockwiseStep(uint8_t steps);
  void counterclockwiseStep(uint8_t steps);     
  void stopStep();
  private:

  /* 
  Variables:
    Matriz base:
      Matriz (4x4) con la secuencia de pasos que no se va a modificar. 
      Esta es la base o matriz de referencia la cual es inmutable, y reiniciara,
      la matriz que le dara movimiento a las bobina del motor.

    Matriz paso:
      // Esta matriz contendra la posicion inmediata del rotor con respecto a las bobinas
      // esta se actualizara por cada movimiento del motor, por lo que el giro del motor,
      // siempre dara los pasos precisos.

    h:
      La variable h, es la que va a determinar como se actualiza
      la matriz de pasos, con respecto a la matriz base, de acuerdo
      a los pasos que de el motor. Esto con el objetivo de controlar
      en todo momento la posicion del rotor con respecto a las bobinas.
      
    IN1, IN2, IN3, IN4:
      Pines del modulo del motor paso a paso, generalemnte marcados con esta nomenclatura.
  */
  
  int base[4][4] =   
  {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1}
  };
  
  int paso [4][4] =   // matriz (4x4) con la secuencia de pasos
  {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 1, 1},
    {1, 0, 0, 1}
  };

  
  uint8_t IN1, IN2, IN3, IN4;
  int h = 0;
  int demora = 10;

};

#endif
