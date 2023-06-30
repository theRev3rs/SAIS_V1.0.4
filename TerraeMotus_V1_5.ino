/*
 SAIS V1.0.4 ( HARDWARE )
 TerraeMotus V1.0 ( SOFTWARE )
 PRUEBA PARA OBTENER FRECUENCIA DE UN SISMO
 25/05/23
 MAIN CODER: RICARDO ESTRADA
*/
// librerias
#include <Wire.h>
#include <SparkFun_ADXL345.h>
#include <SoftwareSerial.h>

//Directivas
#define activado 4
#define vibe 6

//Variables
float mov;
int intensidad;
String NUM_MOVIL[] = {/*#1:*/"54129069",/*#2:*/"35559812",/*#3:*/"41060404"/*#4:*//*#5:*/}; //# Numeros Telefonicos
int Cantidad_Movil = 3;

//Constructor
ADXL345 adxl = ADXL345();
SoftwareSerial SIM900(7, 8); // Configuración de los pines serial por software

//Funciones
float intensidad_sismo();
int escala_sismica();
void inializacion();
void activacion();
void enviarSMS();

void setup() {
//Setup ADXL345
analogReference(EXTERNAL);
adxl.powerOn();
adxl.setRangeSetting(16);

//Setup Visualizadores
pinMode(9, OUTPUT);
pinMode(activado, OUTPUT);
pinMode(vibe, INPUT);

//Setup SIM900
// Arduino se comunica con el SIM900 a una velocidad de 19200bps
delay(500);
Serial.begin(19200);
SIM900.begin(19200);
delay(1000);
digitalWrite(9, HIGH);
delay(1000);
digitalWrite(9, LOW);
delay(5000);
Serial.println("Activando....");
delay(20000); //Tiempo de espera para que conexión a la red del modulo SIM
inializacion();
activacion();
digitalWrite(activado, HIGH);
delay(100);
digitalWrite(activado, LOW);
delay(100);
digitalWrite(activado, HIGH);
delay(100);
digitalWrite(activado, LOW);
}

void loop() {
if (digitalRead(vibe) == HIGH) {
Serial.println("Sismo detectayado");
intensidad = intensidad_sismo();
Serial.println(intensidad);
delay(500);
if(intensidad > 2){
enviarSMS();
}
}
}

float intensidad_sismo() {
mov = 0;
int x, y, z;
float suma_x = 0;
float suma_y = 0;
float suma_z = 0;
adxl.readAccel(&x, &y, &z);
for (int t = 0; t < 1000; t++) {
adxl.readAccel(&x, &y, &z);
suma_x = suma_x + abs(x);
suma_y = suma_y + abs(y);
delay(1);
}
mov = abs(suma_x + suma_y + suma_z) / 1000;
return mov;
}

void inializacion() {
Serial.println("Conectado a la RED");
SIM900.println("AT+CMGF=1\r");
delay(1000);
SIM900.println("AT+CNMI=2,2,0,0,0\r");
delay(2000);
SIM900.println("AT+CMGD=1,4");
delay(1000);
SIM900.println("AT+CMGL=\"ALL\",0");
delay(2000);
Serial.println("Comandos AT enviados");
}

void activacion() {
for (int n = 0; n < Cantidad_Movil; n++) {
delay(1000);
SIM900.println("AT + CMGS = \"" + NUM_MOVIL[n] + "\"");
delay(100);
SIM900.println("En linea...");
delay(100);
SIM900.println((char)26);
delay(9000);
Serial.println();
Serial.print("Mensaje No: ");
Serial.print(n + 1);
Serial.println(" enviado");
}
}

void enviarSMS() {
for (int n = 0; n < Cantidad_Movil; n++) {
delay(1000);
SIM900.println("AT + CMGS = \"" + NUM_MOVIL[n] + "\"");
delay(100);
SIM900.print("Sismo detectado, Intensidad: ");
SIM900.println(intensidad);// Enviamos la intensidad del Sismo
Serial.println("Enviando mensaje");
delay(100);
SIM900.println((char)26);
delay(9000);
Serial.println();
Serial.println("Mensaje enviado al numero: " + NUM_MOVIL[n]);
}
digitalWrite(activado, HIGH);
delay(100);
digitalWrite(activado, LOW);
delay(100);
}
