/*
 * SAIS V1.0.4 ( HARDWARE )
 * TerraeMotus V1.0 ( SOFTWARE )
 * PRUEBA PARA OBTENER FRECUENCIA DE UN SISMO
 * 25/05/23
 * MAIN CODER: RICARDO ESTRADA
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
String NUM_MOVIl[] {/*#1:*/"54129169",/*#2:*/"35559812",/*#3:*/"41060404"}; //# Numeros de celulares 
int cant_num = 3;

//Constructor
ADXL345 adxl = ADXL345();
SoftwareSerial SIM900(7,8); // Configuración de los pines serial por software

//Funciones
float intensidad_sismo();
int escala_sismica();
void setup() {
  //setup Vibracion
   pinMode(vibe, INPUT);
  //Setup ADXL345
  analogReference(EXTERNAL);
   Serial.begin(9600);            
   adxl.powerOn();           
   adxl.setRangeSetting(16);
   
   //Setup Visualizadores
   pinMode(9, OUTPUT);
   pinMode(activado, OUTPUT);
   
  //Setup SIM900
    // Arduino se comunica con el SIM900 a una velocidad de 19200bps
  digitalWrite(9, HIGH);
  delay(1500);
  digitalWrite(9, LOW);
  delay(5000);
  SIM900.begin(19200);
  Serial.println("Activando");
  delay(300000);               //TIempo de inicio de SIM
  digitalWrite(activado, HIGH);
  delay(100);
  digitalWrite(activado, LOW);
  delay(100);
  digitalWrite(activado, HIGH);
  delay(100);
  Serial.println("Activado");
  SIM900.println("AT+CMGF=1\r");
  delay(200);
  for(int i =0;i<cant_num;i++){
  SIM900.println("AT + CMGS = \""+NUM_MOVIl[i]+"\"");    // Reemplazar por el número a enviar el mensaje
  delay(200);
  SIM900.println("SAIS EN LINEA");
  SIM900.print("\r"); 
delay(25000);
  // Finalizamos este comando con el caracter de sustitución (->) código ASCII 26 para el envio del SMS
  SIM900.println((char)26);
  delay(200);
  SIM900.println();
  }
  Serial.println("En linea...");
}

void loop() {
  if( digitalRead(vibe) == HIGH ){
    Serial.println("Sismo detectado");
  intensidad = intensidad_sismo();
  Serial.println(intensidad);
  //  escala_sismica();
  digitalWrite(activado, LOW);
  delay(100);
  enviarSMS();
  digitalWrite(activado, HIGH);
  delay(100);
  }
  else{
    digitalWrite(activado, !digitalRead(activado));
    delay(500);
    }
}

float intensidad_sismo(){
  mov = 0;
  int x,y,z;
  float suma_x=0;
  float suma_y=0;
  float suma_z=0;
  adxl.readAccel(&x, &y, &z);
  for(int t=0; t<1000; t++){
  adxl.readAccel(&x, &y, &z);
  suma_x = suma_x+abs(x);
  suma_y = suma_y+abs(y);
  delay(1);
   }
  mov = abs(suma_x+suma_y+suma_z)/1000;
  return mov; 
  }
  
int escala_sismica(){
  
  }
  void enviarSMS(){
  delay(100);
    SIM900.println("AT+CMGF=1\r");                // Comando AT para configurar el SIM900 en modo texto
    for(int i = 0;i < cant_num; i++){
  delay(200);
  SIM900.println("AT + CMGS = \""+NUM_MOVIl[i]+"\"");    // Reemplazar por el número a enviar el mensaje
  delay(200);
  SIM900.print("Sismo detectado, Intensidad: ");
  SIM900.println(intensidad);// Reemplazar por el texto a enviar
  Serial.println("Enviando mensaje");
  SIM900.print("\r"); 
delay(25000);
  // Finalizamos este comando con el caracter de sustitución (->) código ASCII 26 para el envio del SMS
  SIM900.println((char)26);
  delay(200);
  SIM900.println();
  Serial.println("Mensaje enviado al numero "+NUM_MOVIl[i]);
    }
  Serial.println("Terminado");
  delay(1000);
}
