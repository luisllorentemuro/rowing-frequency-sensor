
#include <LiquidCrystal.h> 
#include <TimeLib.h> 
#include <Wire.h> 
#include <math.h> 

float ponderacion_paladas[3]={0.20,0.30,0.50}; 
float tiempos[3]={1.0,1.0,1.0}; 
bool detectoPulso; 
float sensor_sor; 
float SPM = 0.00; 
float tiempo; 
float valor_anterior; 
int cnt = 0; 
const int pin = 6; 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; 
LiquidCrystal lcd(rs, en, d4, d5, d6, d7); 
const int MPU = 0x68; 
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ; 
float angulo = 0.0; 
float umbral[3] = {10, 15, 20}; 

void setup() { 
  lcd.begin(16,2); 
  lcd.setCursor(0,0); 
  lcd.print("Paladas por"); 
  lcd.setCursor(0,1); 
  lcd.print("minuto: "); 
  lcd.setCursor(8,1); 
  lcd.print(SPM); 
  pinMode(pin, INPUT); 
  pinMode(22, OUTPUT); 
  pinMode(24, OUTPUT); 
  pinMode(26, OUTPUT); 
  pinMode(28, OUTPUT); 
  pinMode(30, OUTPUT); 
  pinMode(32, OUTPUT); 
  pinMode(34, OUTPUT); 
  pinMode(36, OUTPUT); 
  pinMode(38, OUTPUT); 
  pinMode(40, OUTPUT); 
  
  Wire.begin(); 
  Wire.beginTransmission(MPU); 
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true); 
} 
void loop() { 
  sensor_sor = digitalRead(pin); 
  if(sensor_sor == LOW && valor_anterior == HIGH){ 
    Serial.print("Se detecta"); 
    detectoPulso = true; 
  }else{ 
    detectoPulso = false; 
  } 
  valor_anterior = sensor_sor; 
  if (detectoPulso) { 
    if (cnt == 0) { 
      tiempos[0] = tiempos[1]; 
      tiempos[1] = tiempos[2]; 
      tiempos[2] = (millis()-tiempo)/1000.0; 
      tiempo = millis(); 
      SPM = 60.0*(ponderacion_paladas[0]/tiempos[0]+ 
      +ponderacion_paladas[1]/tiempos[1]+ 
      +ponderacion_paladas[2]/tiempos[2]); 
      lcd.setCursor(8,1); 
      lcd.print(SPM,1); 
      cnt = -2; 
    } 
    cnt++; 
  } 
  Wire.beginTransmission(MPU); 
  Wire.write(0x3B); 
  Wire.endTransmission(false); 
  Wire.requestFrom(MPU,12,true); 
  AcX=Wire.read()<<8|Wire.read(); 
  AcY=Wire.read()<<8|Wire.read(); 
  AcZ=Wire.read()<<8|Wire.read(); 
  GyX=Wire.read()<<8|Wire.read(); 
  GyY=Wire.read()<<8|Wire.read(); 
  GyZ=Wire.read()<<8|Wire.read(); 
  angulo = 180*(atan((float)AcY/AcZ))/PI; 
  delay(100); 
  if (angulo < umbral[0] && angulo > -umbral[0]) { 
    digitalWrite(22, LOW); 
    digitalWrite(24, LOW); 
    digitalWrite(26, LOW); 
    digitalWrite(28, HIGH); 
    digitalWrite(30, HIGH); 
    digitalWrite(32, HIGH); 
    digitalWrite(34, LOW); 
    digitalWrite(36, LOW); 
    digitalWrite(38, LOW); 
    digitalWrite(40, LOW); 
  } else if (angulo > -umbral[1] && angulo <= -umbral[0]) { 
    digitalWrite(22, LOW); 
    digitalWrite(24, LOW); 
    digitalWrite(26, HIGH); 
    digitalWrite(28, HIGH); 
    digitalWrite(30, HIGH); 
    digitalWrite(32, LOW); 
    digitalWrite(34, LOW); 
    digitalWrite(36, LOW); 
    digitalWrite(38, LOW); 
    digitalWrite(40, LOW); 
  } else if (angulo > -umbral[2] && angulo <= -umbral[1]) { 
    digitalWrite(22, LOW); 
    digitalWrite(24, HIGH); 
    digitalWrite(26, HIGH); 
    digitalWrite(28, HIGH); 
    digitalWrite(30, LOW); 
    digitalWrite(32, LOW); 
    digitalWrite(34, LOW); 
    digitalWrite(36, LOW); 
    digitalWrite(38, LOW); 
    digitalWrite(40, LOW); 
  } else if (angulo <= -umbral[2]) { 
    digitalWrite(22, HIGH); 
    digitalWrite(24, HIGH); 
    digitalWrite(26, HIGH); 
    digitalWrite(28, LOW); 
    digitalWrite(30, LOW); 
    digitalWrite(32, LOW); 
    digitalWrite(34, LOW); 
    digitalWrite(36, LOW); 
    digitalWrite(38, LOW); 
    digitalWrite(40, HIGH); 
  }else if (angulo < umbral[1] && angulo >= umbral[0]) { 
    digitalWrite(22, LOW); 
    digitalWrite(24, LOW); 
    digitalWrite(26, LOW); 
    digitalWrite(28, LOW); 
    digitalWrite(30, HIGH); 
    digitalWrite(32, HIGH); 
    digitalWrite(34, HIGH); 
    digitalWrite(36, LOW); 
    digitalWrite(38, LOW); 
    digitalWrite(40, LOW); 
  } else if (angulo < umbral[2] && angulo >= umbral[1]) { 
    digitalWrite(22, LOW); 
    digitalWrite(24, LOW); 
    digitalWrite(26, LOW); 
    digitalWrite(28, LOW); 
    digitalWrite(30, LOW); 
    digitalWrite(32, HIGH); 
    digitalWrite(34, HIGH); 
    digitalWrite(36, HIGH); 
    digitalWrite(38, LOW); 
    digitalWrite(40, LOW); 
  } else if (angulo >= umbral[2]) { 
    digitalWrite(22, LOW); 
    digitalWrite(24, LOW); 
    digitalWrite(26, LOW); 
    digitalWrite(28, LOW); 
    digitalWrite(30, LOW); 
    digitalWrite(32, LOW); 
    digitalWrite(34, HIGH); 
    digitalWrite(36, HIGH); 
    digitalWrite(38, HIGH); 
    digitalWrite(40, HIGH); 
  } 
} 


 
