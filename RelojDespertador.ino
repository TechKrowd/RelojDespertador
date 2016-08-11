#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd (0x27, 16, 2);
const int PULSADOR_MODO = 2;
const int PULSADOR_UP = 3;
const int PULSADOR_DOWN = 4;

int horas=0;
int minutos=0;
volatile int segundos=0;
volatile boolean actualizar=true;

char texto[10];
int modo = 0;
const int N=3;


void setup(){
  Wire.begin();  
  lcd.begin(16,2);
  lcd.clear();
  lcd.backlight();

  pinMode(PULSADOR_MODO, INPUT);
  pinMode(PULSADOR_UP, INPUT);
  pinMode(PULSADOR_DOWN, INPUT);
  
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(manejadoraTimer);
}

void manejadoraTimer(){
    segundos++;
    actualizar=true;
}

void actualizarReloj(){
  minutos += segundos / 60;
  segundos = segundos % 60;

  horas += minutos / 60;
  minutos = minutos % 60;

  horas = horas % 24;
}


void incrementarHoras(){
  horas ++;
  horas = horas % 24; 
  actualizar=true; 
}

void decrementarHoras(){
  horas --;
  if(horas<0){
    horas = 23;  
  }  
  actualizar=true;
}

void incrementarMinutos(){
  minutos ++;
  minutos = minutos % 60; 
  actualizar=true; 
}

void decrementarMinutos(){
  minutos --;
  if(minutos<0){
    minutos = 59;  
  }  
  actualizar=true;
}

void fijarCursorModo(){
  switch(modo){
    case 0: 
      lcd.noBlink();
    break;
    case 1: 
      lcd.setCursor(0, 0);
      lcd.blink();
    break;
    case 2: 
      lcd.setCursor(3, 0);
      lcd.blink();
    break;  
  }
}

void loop(){

  if(digitalRead(PULSADOR_MODO)==HIGH){
    modo++;
    modo = modo % N;
    fijarCursorModo();
    while(digitalRead(PULSADOR_MODO)==HIGH);
  }

  switch(modo){
    case 1: 
      if(digitalRead(PULSADOR_UP)==HIGH){
        incrementarHoras();
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        decrementarHoras();
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
    break;
    case 2: 
    if(digitalRead(PULSADOR_UP)==HIGH){
        incrementarMinutos();
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        decrementarMinutos();
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
    break;
  }

  if(actualizar == true){
    actualizarReloj();
    lcd.clear();
    lcd.noBlink();
    lcd.setCursor(0,0);
    sprintf(texto, "%02d:%02d:%02d",horas,minutos, segundos); 
    lcd.print(texto);
    /*lcd.print(horas);
    lcd.print(":");
    lcd.print(minutos);
    lcd.print(":");
    lcd.print(segundos);*/
    actualizar = false;
    fijarCursorModo();
  }
  
}
