/*
 * SAIS V1.x.x ( HARDWARE )
 * TerraeMotus V1.0 ( SOFTWARE )
 * PRUEBA PARA OBTENER FRECUENCIA DE UN SISMO
 * 25/05/23
 * MAIN CODER: RICARDO ESTRADA
 */

 
 // librerias
#include <SPI.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

//Directivas
#define activado 4
#define direccion_lcd 0x3F
#define filas 2
#define columnas 16
#define vibe 6

//Variables
float mov;
int intensidad;
String NUM_MOVIl[] {/*#1:*/"54129169",/*#2:*/"35559812",/*#3:*/"33883857",/*#4:*/"41060404"/*#5:*/}; //# Numeros de celulares 
int cant_num = 4;

//Constructor
ADXL345 adxl = ADXL345();
LiquidCrystal_I2C PANTALLA(direccion_lcd, columnas, filas); 
SoftwareSerial SIM900(7, 8); // Configuración de los pines serial por software

//Funciones
float intensidad_sismo();
int escala_sismica();
void enviarSMS();

void setup() {
  //Setup ADXL345
  analogReference(EXTERNAL);
   Serial.begin(19200);            
   Serial.println("Iniciar");
   Serial.println();
   adxl.powerOn();           
   adxl.setRangeSetting(16);
   //Setup Visualizadores
   pinMode(activado, OUTPUT);
   pinMode(vibe, INPUT);
   digitalWrite(activado, HIGH);
   PANTALLA.init();
  PANTALLA.backlight();
  //Setup SIM900
  SIM900.begin(19200);  // Arduino se comunica con el SIM900 a una velocidad de 19200bps
  digitalWrite(9, HIGH);
  delay(1000);
  digitalWrite(9, LOW);
  delay(5000);
  delay(20000);
  SIM900.print("AT+CMGF=1\r");                // Comando AT para configurar el SIM900 en modo texto
  delay(200);
  
}

void loop() {
  PANTALLA.setCursor(0,0);
  PANTALLA.print("   Intensidad   ");
  if(digitalRead(vibe)==HIGH){
  intensidad = intensidad_sismo();
  Serial.println(intensidad);
  PANTALLA.setCursor(5,1);
  PANTALLA.print("    ");
  PANTALLA.setCursor(5,1);
  PANTALLA.print(intensidad);
  escala_sismica();
  }
  PANTALLA.setCursor(0,1);
  PANTALLA.print("                ");
}

float intensidad_sismo(){
  mov = 0;
  int x,y,z;
  float suma_x=0;
  float suma_y=0;
  float suma_z=0;
  adxl.readAccel(&x, &y, &z);
   if( abs(x) < 25 && abs(y) < 25 && abs(z) > 25){ // SAIS EN PLANO X,Y HORIZONTAL
    PANTALLA.setCursor(14,0);
   PANTALLA.print("XY");
    for(int t=0; t<1000; t++){
      adxl.readAccel(&x, &y, &z);
   suma_x = suma_x+abs(x);
   suma_y = suma_y+abs(y);
   delay(1);
   }
   }
   if( abs(x) > 25 && abs(y) < 25 && abs(z) < 25){  // SAIS EN PLANO Y,Z HORIZONTAL
    PANTALLA.setCursor(14,0);
   PANTALLA.print("YZ");
    for(int t=0; t<1000; t++){
      adxl.readAccel(&x, &y, &z);
   suma_x = suma_z+abs(z);
   suma_y = suma_y+abs(y);
   delay(1);
    }
   }
   if( abs(x) < 25 && abs(y) > 25 && abs(z) < 25){  // SAIS EN PLANO X,Z HORIZONTAL
    PANTALLA.setCursor(14,0);
   PANTALLA.print("XZ");
    for(int t=0; t<1000; t++){
      adxl.readAccel(&x, &y, &z);
   suma_x = suma_x+abs(x);
   suma_y = suma_z+abs(z);
   delay(1);
    }
   }
  mov = abs(suma_x+suma_y+suma_z)/1000;
  Serial.print(x);Serial.print(",");Serial.print(y);Serial.print(",");Serial.println(z);
  return mov; 
  }
  
int escala_sismica(){
  if(intensidad > 2){
    enviarSMS();
  }
}
  void enviarSMS(){
    digitalWrite(activado, HIGH);
    for( int i = 0; i < cant_num; i++){
      delay(1000);
  SIM900.println("AT + CMGS = \""+NUM_MOVIl[i]+"\"");
  SIM900.println("Sismo de intensidad: ");
  SIM900.println(intensidad); 
  delay(200);  
  SIM900.println((char)26);
  delay(200);
  SIM900.println();
    }
    digitalWrite(activado,LOW);
}
