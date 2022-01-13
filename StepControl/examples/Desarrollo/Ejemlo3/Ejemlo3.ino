//Cargar librerias necesarias para el programa
#include <Wire.h>      // libreria para bus I2C
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#include <TimerOne.h>
#include <Servo.h>      // incluye libreria de Servo
#include <StepControl.h>


//Variables de la pnatalla OLED
#define ANCHO 128       // 128 columnas
#define ALTO 64         // 64 filas
#define OLED_RESET 4    // necesario por la libreria pero no usado
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto para la OLED SSD1306

//Variables del ENCODER
#define DT  2       // DT a pin digital 2 (DT en modulo) Pin de la interrupcion 
#define CLK  3      // CLK a pin digital 3 (CLK en modulo)
#define btn 8       // Swiche del encoder al pin 4
#define LED_btn 13  // LED al pulsar el Swiche Pin 13
bool menu;
volatile int posicion_encoder = 0; // Posicion del encoder
int ant_posicion = 0;    // Valor anterior de la posicion del encoder

//Variables SERVO-MOTOR
Servo servo1;     // crea objeto
int PINSERVO = 7;   // pin 7 conectado a señal del servo
int PULSOMIN = 550;    // pulso minimo en microsegundos
int PULSOMAX = 2200;    // pulso maximo en microsegundos
int gradosServo = 0;
int grados = 0;
double g = 0;
int p = 0;

//Variables MOTOR PASO-A-PASO
int gradosPaP = 0;
double pasos = 0;
StepControl pasoApaso = StepControl(9,10,11,12);

void setup() {
  // Inicializamos pantalla OLED:
  Wire.begin();           // inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inicia la pantalla con la direeción en el bus I2C
  oled.clearDisplay();    // Borra la pantalla
  oled.setTextColor(SSD1306_WHITE);  // Pantalla a un solo color
  mostrarPantalla();
  set_cursor();

  //Inicializamos interrupciones para el ENCODER:
  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);
  pinMode(btn, INPUT);    // No se activa la resistencia Pull Up
  pinMode(LED_btn, OUTPUT);  //
  attachInterrupt(digitalPinToInterrupt(DT), encoder, LOW);  // Se habilita interrupcion en pin 0 modo LOW

  //Inicializamos SERVO-MOTOR:
  servo1.attach(PINSERVO, PULSOMIN, PULSOMAX);
  servo1.write(0);    // ubica el servo a 0 grados

  //Inicializamos puerto serial:
  Serial.begin(9600);
  Serial.println("INICIO DE PROGRAMA");  // Mensaje inicial en Puerto Serie
}

void loop() {
  // put your main code here, to run repeatedly: 
  menu = true;
  set_cursor();
  if (digitalRead(btn) == LOW)     // Si es oprimido el Swich del Encoder
  {
    delay(500);
    menu = false;
    digitalWrite(LED_btn, HIGH); // Enciende el LED del Pin 13
    
    switch (posicion_encoder) {
      
      case 0:
        oled.clearDisplay();       // limpia pantalla
        oled.setTextSize(1);      // establece tamano de texto en 2
        oled.setCursor(10, 30);   // ubica cursor en coordenadas 10,5
        oled.print("PRUEBA MOTOR PaP");  // escribe en pantalla el texto
        oled.display();
        //paso completo 4*6 = 24; 24*15 =360 grados, teniendo en cuenta
        //que en la simulacion el motor gira 15 grados por paso
        pasoApaso.clockwiseStep(2048);
        delay(2000);
        pasoApaso.counterclockwiseStep(2048);
        break;

        
      case 1:
        oled.clearDisplay();       // limpia pantalla
        oled.setTextSize(1);      // establece tamano de texto en 2
        oled.setCursor(10, 30);   // ubica cursor en coordenadas 10,5
        oled.print("PRUEBA SERVO-MOTOR");  // escribe en pantalla el texto
        oled.display();
        servo1.write(0);    // ubica el servo a 0 grados
        delay(1000);      // espera de 1 seg.
        servo1.write(180);    // ubica el servo a 180 grados
        delay(1000);      // espera de 1 seg.
        servo1.write(0);    // ubica el servo a 0 grados
        delay(1000);      // espera de 1 seg.
        break;

        
      case 2:
        grados = 0;
        posicion_encoder = gradosPaP;
        oled.clearDisplay();       // limpia pantalla
        oled.setTextSize(1);      // establece tamano de texto en 2
        while (digitalRead(btn) == HIGH) {
          oled.setCursor(10, 30);   // ubica cursor en coordenadas 10,5
          oled.print("AZIMUT: ");  // escribe en pantalla el texto
          oled.print(posicion_encoder);  // escribe en pantalla el texto
          oled.display();
          delay(100);
          oled.clearDisplay();
          grados = posicion_encoder;
        }

        if (grados > gradosPaP) {
          g = grados - gradosPaP;
          pasos = (2048 * g) / 360;
          p = (int) round(pasos);
          Serial.println("el grado ingresado es mayor al actual");
          pasoApaso.clockwiseStep(p);
        } else if (grados < gradosPaP) {
          g = gradosPaP - grados;
          pasos = (2048 * g) / 360;
          p = (int) round(pasos);
          Serial.println("el grado ingresado es menor al actual");
          pasoApaso.counterclockwiseStep(p);
        }
        gradosPaP = grados;
        posicion_encoder = 2;
        break;

        
      case 3:
        grados = 0;
        posicion_encoder = gradosServo;
        oled.clearDisplay();       // limpia pantalla
        oled.setTextSize(1);      // establece tamano de texto en 2
        while (digitalRead(btn) == HIGH) {
          oled.setCursor(10, 30);   // ubica cursor en coordenadas 10,5
          oled.print("ELEVACION: ");  // escribe en pantalla el texto
          oled.print(posicion_encoder);  // escribe en pantalla el texto
          oled.display();
          delay(100);
          oled.clearDisplay();
          grados = posicion_encoder;
        }
        
        servo1.write(grados);    // ubica el servo a 0 grados
        delay(1000);      // espera de 1 seg.

        gradosServo = grados;
        posicion_encoder = 3;
        break;
    }
    digitalWrite(LED_btn, LOW); // apaga el LED del Pin 13
  }
}

void mostrarPantalla()     // Función para mostrar en pantalla el valor del contador
{
  oled.clearDisplay();       // limpia pantalla
  oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  oled.setTextSize(1);      // establece tamano de texto en 2
  oled.setCursor(10, 5);   // ubica cursor en coordenadas 10,5
  oled.print("POSICIONAMIENTO");      // escribe en pantalla el texto
  oled.setCursor (10, 15);  // ubica cursor en coordenas 10,15
  oled.setTextSize(0);      // establece tamano de texto en 3
  oled.print("Prueba PaP"); // escribe en pantalla el texto
  oled.setCursor (10, 25); // ubica cursor en coordenas 10,25
  oled.print("Prueba Servo"); // escribe en pantalla el texto
  oled.setCursor (10, 35); // ubica cursor en coordenas 10,35
  oled.print("Azimut = "); // escribe en pantalla el texto
  oled.setCursor (10, 45); // ubica cursor en coordenas 10,45
  oled.print("Elevacion = "); // escribe en pantalla el texto
  oled.display();
}

void set_cursor()     // Función para mostrar en pantalla el valor del contador
{
  mostrarPantalla();
  //oled.clearDisplay();       // limpia pantalla
  oled.cp437(true);
  oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  oled.setTextSize(0);      // establece tamano de texto en 3
  switch (posicion_encoder) {
    case 0:
      oled.setCursor (2, 15);  // ubica cursor en coordenas 10,15
      break;
    case 1:
      oled.setCursor (2, 25);  // ubica cursor en coordenas 10,15
      break;
    case 2:
      oled.setCursor (2, 35);  // ubica cursor en coordenas 10,15
      break;
    case 3:
      oled.setCursor (2, 45);  // ubica cursor en coordenas 10,15
      break;
  }
  oled.write(16); // escribe en pantalla el texto
  oled.display();
}


void encoder()   //Funcion de interrupción que determina el sentido del giro del encoder
{
  static unsigned long ultimaInterrupcion = 0;  // variable static con ultimo valor de
  // tiempo de interrupcion
  unsigned long tiempoInterrupcion = millis();  // variable almacena valor de func. millis

  if (tiempoInterrupcion - ultimaInterrupcion > 5) // No lee posibles rebotes en pulsos menores a 5 mseg.
  {
    if (menu) {
      if (digitalRead(CLK) == HIGH && posicion_encoder < 3)     // si CLK es HIGH, rotacion a la derecha
      {
        posicion_encoder++ ;        // incrementa posicion del menu en 1
        Serial.println(posicion_encoder);
      }
      else if (digitalRead(CLK) == LOW && posicion_encoder > 0)
      { // si CLK es LOW, rotacion a la izquierda
        posicion_encoder-- ;        // decrementa posicion del menu en 1
        Serial.println(posicion_encoder);
      } else {
        Serial.println("El menu no tiene mas posiciones");
      }
      ultimaInterrupcion = tiempoInterrupcion;  //actualiza valor del tiempo
    } else {
      if (digitalRead(CLK) == HIGH)     // si CLK es HIGH, rotacion a la derecha
      {
        posicion_encoder++ ;        // incrementa posicion del menu en 1
        Serial.println(posicion_encoder);
      }
      else
      { // si CLK es LOW, rotacion a la izquierda
        posicion_encoder-- ;        // decrementa posicion del menu en 1
        Serial.println(posicion_encoder);
      }
    }
  }
}
