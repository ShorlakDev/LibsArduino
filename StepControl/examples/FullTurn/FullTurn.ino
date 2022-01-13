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


StepControl pasoApaso = StepControl(9,10,11,12); //Declarar objeto StepControl

void setup() {
  // Inicializamos pantalla OLED:
  Wire.begin();           // inicializa bus I2C
  oled.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Inicia la pantalla con la direeción en el bus I2C
  oled.clearDisplay();    // Borra la pantalla
  oled.setTextColor(SSD1306_WHITE);  // Pantalla a un solo color
}

void loop() {
  oled.clearDisplay();       // limpia pantalla
  oled.setTextSize(1);      // establece tamano de texto en 2
  oled.setCursor(10, 30);   // ubica cursor en coordenadas 10,5
  oled.print("PRUEBA MOTOR PaP");  // escribe en pantalla el texto
  oled.display();
  pasoApaso.clockwiseStep(2048);
  delay(2000);
  pasoApaso.counterclockwiseStep(2048);
}
