#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd (0x27, 16, 2);

int horas=0;
int minutos=0;
volatile int segundos=0;
volatile boolean actualizar=true;

void setup(){
  Wire.begin();  
  lcd.begin(16,2);
  lcd.clear();
  lcd.backlight();

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

void loop(){
  if(actualizar == true){
    actualizarReloj();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(horas);
    lcd.print(":");
    lcd.print(minutos);
    lcd.print(":");
    lcd.print(segundos);
    actualizar = false;
  }
  
}


