#include "StepControl.h"

StepControl::StepControl(uint8_t _IN1, uint8_t _IN2, uint8_t _IN3, uint8_t _IN4){
  IN1 = _IN1;
  IN2 = _IN2;
  IN3 = _IN3;
  IN4 = _IN4;
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN1, base[0][0]);
  digitalWrite(IN2, base[0][1]);
  digitalWrite(IN3, base[0][2]);
  digitalWrite(IN4, base[0][3]);
  
}

void StepControl::clockwiseStep(uint8_t steps){

  int pasos = steps;
  int m = 0; // variable que guarda cuantos ciclos ha echo, para agragar el ultimo paso faltante segun su posicion
  float check = pasos / 4;
  int p1 = (int) round(check);
  int pasitos = 0;

  //Si vamos a dar 4 pasos o multiplos de 4.
  if (pasos % 4 == 0) {
    for (int j = 0; j < p1; j++){
      for (int i = 0; i < 4; i++){
        digitalWrite(IN1, paso[i][0]);
        digitalWrite(IN2, paso[i][1]);
        digitalWrite(IN3, paso[i][2]);
        digitalWrite(IN4, paso[i][3]);
        delay(demora);
      }

      //Se le debe agregar un paso mas fuera del for, para que de los 4 pasos completos.
      digitalWrite(IN1, paso[0][0]);
      digitalWrite(IN2, paso[0][1]);
      digitalWrite(IN3, paso[0][2]);
      digitalWrite(IN4, paso[0][3]);
      delay(demora);
    }
  }else{

    //Si se requiere dar menos de 3 pasos del motor
    if (pasos < 4){
      for (int i = 0; i < pasos; i++){
        digitalWrite(IN1, paso[i][0]);
        digitalWrite(IN2, paso[i][1]);
        digitalWrite(IN3, paso[i][2]);
        digitalWrite(IN4, paso[i][3]);
        delay(demora);
        m++;

        //Aqui se va realizando la rotacion de la variable h,
        //dependiendo de los pasos que va realizando el motor,
        if (h > 2){
          h = 0;
        }else{
          h = h + 1;
        }
      }

      //Paso faltante controlado con la variable m.
      digitalWrite(IN1, paso[m][0]);
      digitalWrite(IN2, paso[m][1]);
      digitalWrite(IN3, paso[m][2]);
      digitalWrite(IN4, paso[m][3]);
      delay(demora);

      //Se actualiza la matriz paso con respecto a la matriz base,
      //utilizando la variable h, la cual me da la posicion actual
      //del rotor con respecto a las bobinas.
      for (int i = 0; i < 4; i++){
        paso[i][0] = base[h][0];
        paso[i][1] = base[h][1];
        paso[i][2] = base[h][2];
        paso[i][3] = base[h][3];
        if (h > 2){
          h = 0;
        }else{
          h = h + 1;
        }
       }
      }else{

        //Si se va a dar mas de 4 pasos y no son multiplos  de 4,
        //se van realizando de a 4 pasos, hasta la varible p1, la cual 
        //representa el numero de pasos en que son multiplos de 4.
        for (int j = 0; j < p1; j++){
          for (int i = 0; i < 4; i++){
            digitalWrite(IN1, paso[i][0]);
            digitalWrite(IN2, paso[i][1]);
            digitalWrite(IN3, paso[i][2]);
            digitalWrite(IN4, paso[i][3]);
            delay(demora);
          }

          digitalWrite(IN1, paso[0][0]);
          digitalWrite(IN2, paso[0][1]);
          digitalWrite(IN3, paso[0][2]);
          digitalWrite(IN4, paso[0][3]);
          delay(demora);
        }

        //Se obtiene los pasos extra que se deben realizar,
        //a los pasos dados anterioremente, esto se realiza,
        //utilizando el residuo del numero de pasos dividido 4.  
        pasitos = pasos % 4;
        for (int i = 0; i < pasitos; i++){
          digitalWrite(IN1, paso[i][0]);
          digitalWrite(IN2, paso[i][1]);
          digitalWrite(IN3, paso[i][2]);
          digitalWrite(IN4, paso[i][3]);
          delay(demora);
          m++;
          if (h > 2) {
            h = 0;
          } else{
            h = h + 1;
          }
        }
        
        digitalWrite(IN1, paso[m][0]);
        digitalWrite(IN2, paso[m][1]);
        digitalWrite(IN3, paso[m][2]);
        digitalWrite(IN4, paso[m][3]);
        delay(demora);

        for (int i = 0; i < 4; i++){
          paso[i][0] = base[h][0];
          paso[i][1] = base[h][1];
          paso[i][2] = base[h][2];
          paso[i][3] = base[h][3];
          if (h > 2){
            h = 0;
          }else{
            h = h + 1;
          }
        }
      }
    }


  delay(200);
  //Apagar motor:
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}

void StepControl::counterclockwiseStep(uint8_t steps){
  /*
  De manera analoga al giro horario, se hace antihorario, pero en 
  este caso se recorren las matrices de forma inversa, o se utiliza
  la variable de control m, para realizar el muestreo de salida de forma
  antihoraria. Tambien se puede observar que la variable h tambien se
  actualiza de manera inversa.
  */
  int pasos = steps;
  float check = pasos / 4;
  int p1 = (int) round(check);
  int pasitos = 0;
  int m = 3;
  if (pasos % 4 == 0) {
    for (int j = 0; j < p1; j++)
    {
      for (int i = 3; i >= 0; i--)
      {
        digitalWrite(IN1, paso[i][0]);
        digitalWrite(IN2, paso[i][1]);
        digitalWrite(IN3, paso[i][2]);
        digitalWrite(IN4, paso[i][3]);
        delay(demora);
      }
    }
  } else {
    if (pasos < 4) {
      for (int i = 0; i < pasos; i++)
      {
        digitalWrite(IN1, paso[m][0]);
        digitalWrite(IN2, paso[m][1]);
        digitalWrite(IN3, paso[m][2]);
        digitalWrite(IN4, paso[m][3]);
        delay(demora);
        m--;
        if (h == 0) {
          h = 3;
        }else {
          h = h - 1;
         }
       }
      for (int i = 0; i < 4; i++) {
        paso[i][0] = base[h][0];
        paso[i][1] = base[h][1];
        paso[i][2] = base[h][2];
        paso[i][3] = base[h][3];
        if (h > 2){
            h = 0;
        }else{
            h = h + 1;
        }
      }
    } else {
      
      for (int j = 0; j < p1; j++)
      {
        for (int i = 3; i >= 0; i--)
        {
          digitalWrite(IN1, paso[i][0]);
          digitalWrite(IN2, paso[i][1]);
          digitalWrite(IN3, paso[i][2]);
          digitalWrite(IN4, paso[i][3]);
          delay(demora);
        }
      }

      pasitos = pasos % 4;
      for (int i = 0; i < pasitos; i++)
      {
        digitalWrite(IN1, paso[m][0]);
        digitalWrite(IN2, paso[m][1]);
        digitalWrite(IN3, paso[m][2]);
        digitalWrite(IN4, paso[m][3]);
        delay(demora);
        m--;
        if (h == 0) {
          h = 3;
        }else {
          h = h - 1;
         }
      }
      for (int i = 0; i < 4; i++) {
        paso[i][0] = base[h][0];
        paso[i][1] = base[h][1];
        paso[i][2] = base[h][2];
        paso[i][3] = base[h][3];
        if (h > 2){
            h = 0;
        }else{
            h = h + 1;
        }
      } 
    }
  }

  delay(200);
  //Apagar motor:
  digitalWrite(IN1, 0);
  digitalWrite(IN2, 0);
  digitalWrite(IN3, 0);
  digitalWrite(IN4, 0);
}
