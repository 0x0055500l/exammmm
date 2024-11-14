#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>  //usado para bluetooth JDY-23  

SoftwareSerial SerialBT(3, 2); // RX, TX de arduino y en JDY-23 al reves

Servo servo1;
Servo servo2;

int pinservo1=6;
int pinservo2=5;

int pinbtn1=9;
int pinbtn2=13;

int pinled_rojo=10;
int pinled_verde=11;
int pinled_gris=12;

int pos = 0;

boolean actividadd=false;
boolean encendidoAPP=false;

int valorbtn1 = 0;
int valorbtn2 = 0;

int position1 = 0;
int position2 = 0;

String inputString = "";

boolean detener=false;

int cantvec1=0;
int cantvec2=0;

int posultm1=0;
int posultm2=0;

unsigned long previousMillisLED = 0;    // Variable para manejar el tiempo del LED
unsigned long previousMillisLED2 = 0;
unsigned long previousMillisServo = 0;
unsigned long previousMillisServo2 = 0;
const long ledDuration = 3000;           // Duración del LED encendido en ms (3 segundos)
const long ledDuration2 = 5000;           // Duración del LED rojo en ms (5 segundos)
boolean ledState = false;               // Estado del LED
boolean ledState2 = false;               // Estado del LED

int estado=0;
int ultimoValorBtn1 = 0, ultimoValorBtn2 = 0;

void actividad1(){
  unsigned long currentMillis2 = millis();  // Obtiene el tiempo actual
  if (currentMillis2 - previousMillisServo >= 1200) {
    int posIn=0;
    int posFin=180;
    
    //validar donde se quedo el servo
    if(posultm1==0){
      for(pos=posIn;pos<=posFin; pos++){
        servo1.write(pos);
      }
      posultm1=180;
    } else if(posultm1==180){
      for(pos = posFin; pos >= posIn; pos--){
        servo1.write(pos);
      }
      posultm1=0;
    }
    // String mmm="mov-";
    // mmm+=cantvec1+1;
    // Serial.println(mmm);
    cantvec1+=1;
    previousMillisServo = currentMillis2;  // Actualiza el tiempo de referencia
    
  }
}
void actividad2(){
  unsigned long currentMillis2 = millis();  // Obtiene el tiempo actual
  if (currentMillis2 - previousMillisServo2 >= 1000) {
    int posIn=0;
    int posFin=90;
    
    //validar donde se quedo el servo
    if(posultm2==0){
      for(pos=posIn;pos<=posFin; pos++){
        servo2.write(pos);
      }
      posultm2=90;
    } else if(posultm2==90){
      for(pos = posFin; pos >= posIn; pos--){
        servo2.write(pos);
      }
      posultm2=0;
    }
    // String mmm="mov-";
    // mmm+=cantvec1+1;
    // Serial.println(mmm);
    cantvec2+=1;
    previousMillisServo2 = currentMillis2;  // Actualiza el tiempo de referencia
    
  }
}
void setup() {
  // put your setup code here, to run once:
  SerialBT.begin(9600); // Comunicación con el módulo Bluetooth
  Serial.begin(9600);
  servo1.attach(pinservo1);
  servo2.attach(pinservo2);
  pinMode(pinled_gris, OUTPUT); //gris
  pinMode(pinled_verde, OUTPUT); //verde
  pinMode(pinled_rojo, OUTPUT); //rojo
  pinMode(pinbtn1,INPUT); //botton 1
  pinMode(pinbtn2,INPUT); //botton 2

  //poner los servos en 0
  servo1.write(0);
  servo2.write(0);
}

void loop() {
  unsigned long currentMillisl1 = millis();  // Obtiene el tiempo actual      
  unsigned long currentMillisl2 = millis();  // Obtiene el tiempo actual      
  valorbtn1= digitalRead(pinbtn1);
  valorbtn2= digitalRead(pinbtn2);
  if(valorbtn1==1 && ultimoValorBtn1 == 0){
    if (estado == 0) { // Solo cambia si el estado es 0
      estado = 1;
      encendidoAPP=true;
      SerialBT.print("habilitado");
      Serial.println("habilitado");
      delay(100);
    }
  }
  if(valorbtn2==1 && ultimoValorBtn2 == 0){
    encendidoAPP=false;
    if (estado == 1) { // Solo cambia si el estado es 1
      estado = 0;
      SerialBT.print("desabilitado");
      Serial.println("desabilitado");
    }
    delay(100);
  }
    if (SerialBT.available()>0) {
      inputString = SerialBT.readString();
      
      if (inputString.length() > 2){
        int commaIndex = inputString.indexOf(','); // Encuentra la posición de la coma
        int secondCommaIndex = inputString.indexOf(',', commaIndex + 1);  // Encuentra la segunda coma
        String firstValue = inputString.substring(0, commaIndex);   //obtiene el primer valor antes de la coma
        String secondValue = inputString.substring(commaIndex + 1, secondCommaIndex);  // Segundo entre las comas
        String command = inputString.substring(secondCommaIndex + 1);  // Tercero después de la segunda coma (el comando 'i' o 'd')

        position1 = firstValue.toInt();   // Convierte la primera parte a int
        position2 = secondValue.toInt();  // Convierte la segunda parte a int

        if (command.indexOf('i') != -1) {
          if(encendidoAPP){                 //validar que si se habilitan los botones de la app pero aki en arduino aun no dio click en el
            Serial.println("inicio el proceso");   //boton1 no podra inicar el proceso
            cantvec1=0;
            posultm1=0;
            detener=false;
            actividadd=true;
            ledState = false;
            previousMillisLED = currentMillisl1;  // Reinicia el temporizador para el LED
            previousMillisLED2 = currentMillisl2;  // Reinicia el temporizador para el LED
          }

        } else if (command.indexOf('d') != -1) {
          if(encendidoAPP){                   //lo mismo para otro boton de la app validar que preciono el boton 1
            Serial.println("se detubo el proceso");
            cantvec1=0;
            posultm1=0;
            detener=true;
            actividadd=false;
          }
        }
      }
      inputString = "";
    }
    if (actividadd) {
      if(cantvec1<position1 && !detener){
        actividad1(); //repetir segun el numero de veces que se envio desde la APP validando que no este detenido
      }
      if(cantvec2<position2 && !detener){
        actividad2(); //repetir segun el numero de veces que se envio desde la APP validando que no este detenido
      }
    }
    if (currentMillisl1 - previousMillisLED <= ledDuration && !ledState) {
      if (actividadd) {
        digitalWrite(pinled_verde, HIGH);
        ledState = true;
      }
    } else if (currentMillisl1 - previousMillisLED > ledDuration && ledState) {
      if (actividadd) {
        digitalWrite(pinled_verde, LOW);
        ledState = false;
      }
    }
    if (currentMillisl2 - previousMillisLED2 <= ledDuration2 && !ledState2) {
      if (actividadd) {
        digitalWrite(pinled_rojo, HIGH);
        ledState2 = true;
      }
    } else if (currentMillisl2 - previousMillisLED2 > ledDuration2 && ledState2) {
      if (actividadd) {
        digitalWrite(pinled_rojo, LOW);
        ledState2 = false;
      }
    }
  ultimoValorBtn1 = valorbtn1;
  ultimoValorBtn2 = valorbtn2;
}

