#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd (0x27, 16, 2);
const int PULSADOR_MODO = 2;
const int PULSADOR_UP = 3;
const int PULSADOR_DOWN = 4;
const int PULSADOR_LUZ = 5;
const int BUZZER = 6;

int horas=0;
int minutos=0;
volatile int segundos=0;
volatile boolean actualizar=true;

int horas_alarma=0;
int minutos_alarma=0;
boolean alarmaOn=false;
int contador_alarma=0;

boolean luzOn=false;
int contador_luz=0;

char texto[10];
int modo = 0;
const int N=6;


void setup(){
  Wire.begin();  
  lcd.begin(16,2);
  lcd.clear();
  lcd.noBacklight();

  pinMode(PULSADOR_MODO, INPUT);
  pinMode(PULSADOR_UP, INPUT);
  pinMode(PULSADOR_DOWN, INPUT);
  pinMode(PULSADOR_LUZ, INPUT);
  pinMode(BUZZER, OUTPUT);
  
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(manejadoraTimer);
}

void manejadoraTimer(){
    segundos++;
    if(luzOn==true){
      contador_luz++;
    }
    if(digitalRead(BUZZER)==HIGH){
      contador_alarma++;
    }
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

void incrementarHorasAlarma(){
  horas_alarma++;
  horas_alarma = horas_alarma % 24; 
  actualizar=true; 
}

void decrementarHorasAlarma(){
  horas_alarma--;
  if(horas_alarma<0){
    horas_alarma = 23;  
  }  
  actualizar=true;
}


void incrementarMinutosAlarma(){
  minutos_alarma ++;
  minutos_alarma = minutos_alarma % 60; 
  actualizar=true; 
}

void decrementarMinutosAlarma(){
  minutos_alarma --;
  if(minutos_alarma<0){
    minutos_alarma = 59;  
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
    case 3: 
      lcd.setCursor(0, 1);
      lcd.blink();
    break;  
    case 4: 
      lcd.setCursor(3, 1);
      lcd.blink();
    break;
    case 5:
      lcd.setCursor(6, 1);
      lcd.blink();
    break;
  }
}

void loop(){

  if(digitalRead(PULSADOR_MODO)==HIGH){
    contador_luz=0;
    modo++;
    modo = modo % N;
    fijarCursorModo();
    while(digitalRead(PULSADOR_MODO)==HIGH);
  }

  if(digitalRead(PULSADOR_LUZ)==HIGH){
    if(digitalRead(BUZZER)==HIGH){
      digitalWrite(BUZZER, LOW);
      contador_alarma=0;  
    }
    contador_luz=0;
    if(luzOn==false){
      lcd.backlight();  
    }else{
      lcd.noBacklight();
    }  
    luzOn=!luzOn;
    while(digitalRead(PULSADOR_LUZ)==HIGH);
  }

  switch(modo){
    //RELOJ
    case 1: 
      if(digitalRead(PULSADOR_UP)==HIGH){
        contador_luz=0;
        incrementarHoras();
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        contador_luz=0;
        decrementarHoras();
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
    break;
    case 2: 
    if(digitalRead(PULSADOR_UP)==HIGH){
      contador_luz=0;
        incrementarMinutos();
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        contador_luz=0;
        decrementarMinutos();
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
    break;
    //ALARMA
    case 3: 
      if(digitalRead(PULSADOR_UP)==HIGH){
        contador_luz=0;
        incrementarHorasAlarma();
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        contador_luz=0;
        decrementarHorasAlarma();
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
    break;
    case 4: 
      if(digitalRead(PULSADOR_UP)==HIGH){
        contador_luz=0;
        incrementarMinutosAlarma();
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        contador_luz=0;
        decrementarMinutosAlarma();
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
    break;
    case 5:
    
      if(digitalRead(PULSADOR_UP)==HIGH){
        contador_luz=0;
        alarmaOn=!alarmaOn;
        actualizar=true;
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
     
      /*
      if(digitalRead(PULSADOR_UP)==HIGH){
        alarmaOn=true;
        actualizar=true;
        while(digitalRead(PULSADOR_UP)==HIGH);
      }
      if(digitalRead(PULSADOR_DOWN)==HIGH){
        alarmaOn=false;
        actualizar=true;
        while(digitalRead(PULSADOR_DOWN)==HIGH);
      }
      */
    break;
  }

  if(actualizar == true){
    actualizarReloj();
    lcd.clear();
    lcd.noBlink();
    //RELOJ
    lcd.setCursor(0,0);
    sprintf(texto, "%02d:%02d:%02d",horas,minutos, segundos); 
    lcd.print(texto);
    //ALARMA
    lcd.setCursor(0,1);
    sprintf(texto, "%02d:%02d",horas_alarma,minutos_alarma); 
    lcd.print(texto);

    if(alarmaOn ==true){
      if(segundos==0 && horas == horas_alarma && minutos == minutos_alarma){
        digitalWrite(BUZZER, HIGH);
      }
      lcd.print(" ON");
    }else{
      lcd.print(" OFF");  
    }

    if(contador_alarma==60){
      digitalWrite(BUZZER, LOW);
      contador_alarma=0;
    }
  
    //LUZ
    if(contador_luz==30){
      lcd.noBacklight();
      luzOn=false;
      contador_luz=0;
    }
    actualizar = false;
    fijarCursorModo();
  }
  
}
