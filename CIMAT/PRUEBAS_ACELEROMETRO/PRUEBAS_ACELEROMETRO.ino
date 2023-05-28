#include <SPI.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>
ADXL345 adxl = ADXL345();
float x2;
float y2;
float x3;
float y3;
float x4;
float y4;
float x5;
float y5;
int promx;
int promy;
int promxy;
int Sensor = A0;
#define alerta 4
void setup()
{
  pinMode(alerta, OUTPUT);
  analogReference(EXTERNAL);
   Serial.begin(9600);            
   Serial.println("Iniciar");
   Serial.println();
   adxl.powerOn();           
   adxl.setRangeSetting(16);       //Definir el rango, valores 2, 4, 8 o 16
}
void loop()
{
//  int vibracion = digitalRead(Sensor);
//   if( vibracion == HIGH){
   digitalWrite(alerta, HIGH);
   int x, y, z;
   adxl.readAccel(&x, &y, &z); 
   x2 = x;
   y2 = y;
   delay(10);
   x3 = x;
   y3 = y;
   delay(10);
   x4 = x;
   y4 = y;
   delay(10);
   x5 = x;
   y5 = y;
   delay(10);
   Serial.print(x);
   Serial.print(", ");
   Serial.print(y);
   Serial.print(", ");
   Serial.println(z);
   delay(500);
   promx = abs((x+x2+x3+x4+x5));
   promy = abs((y+y2+y3+y4+y5));
   promxy = promx + promy;
   Serial.println("");
   Serial.println("Promedio de movimiento:");
   Serial.print("en eje x:" );
   Serial.println(abs(promx));
   Serial.print("en eje y:" );
   Serial.println(promy);
   Serial.print("general: " );
   Serial.println(promxy);
   
   
   if(promxy <= 20 && promxy >= 0){
    Serial.println("actividad sismica leve");
   }
   else if(promxy <= 39 && promxy >= 20){
    Serial.println("actividad sismica media");
   }
   else if(promxy >= 40){
    Serial.println("actividad sismica fuerte");
}
delay(500);
digitalWrite(alerta,LOW);
//}
}
