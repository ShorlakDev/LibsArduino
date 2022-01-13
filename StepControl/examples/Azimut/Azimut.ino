//Cargar librerias necesarias para el programa
#include <Wire.h>      // libreria para bus I2C
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#include <TimerOne.h>
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
volatile int posicion_encoder = 0; // Posicion del encoder

//Variables MOTOR PASO-A-PASO
int gradosPaP = 0;
int grados = 0;
double g = 0;
int p = 0;
double pasos = 0;
StepControl pasoApaso = StepControl(9,10,11,12);

void setup() {
  // Inicializamos pantalla OLED:
  Wire.begin();           // inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inicia la pantalla con la direeción en el bus I2C
  oled.clearDisplay();    // Borra la pantalla
  oled.setTextColor(SSD1306_WHITE);  // Pantalla a un solo color

  //Inicializamos interrupciones para el ENCODER:
  pinMode(DT, INPUT);
  pinMode(CLK, INPUT);
  pinMode(btn, INPUT);    // No se activa la resistencia Pull Up
  attachInterrupt(digitalPinToInterrupt(DT), encoder, LOW);  // Se habilita interrupcion en pin 2 modo LOW
}

void loop() {
       
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
    pasoApaso.clockwiseStep(p);
  }else if (grados < gradosPaP) {
    g = gradosPaP - grados;
    pasos = (2048 * g) / 360;
    p = (int) round(pasos);
    pasoApaso.counterclockwiseStep(p);
  }
  gradosPaP = grados;
}

void encoder()   //Funcion de interrupción que determina el sentido del giro del encoder
{
  static unsigned long ultimaInterrupcion = 0;  // variable static con ultimo valor de
  // tiempo de interrupcion
  unsigned long tiempoInterrupcion = millis();  // variable almacena valor de func. millis

  if (tiempoInterrupcion - ultimaInterrupcion > 5) // No lee posibles rebotes en pulsos menores a 5 mseg.
  {
    if (digitalRead(CLK) == HIGH)     // si CLK es HIGH, rotacion a la derecha
    {
      posicion_encoder++ ;        // incrementa posicion del menu en 1
    }
    else
    { // si CLK es LOW, rotacion a la izquierda
      posicion_encoder-- ;        // decrementa posicion del menu en 1
    }
  }
}
