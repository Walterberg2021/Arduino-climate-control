#include <Wire.h>                 // Підключення бібліотек
#include <iarduino_RTC.h>         //
#include<LiquidCrystal_I2C.h>     //
#include <stDHT.h>                //
#include <GyverEncoder.h>         //

#define tempHumSens 5     // Встановлення визначень
#define vent_rele 6       //
#define tempup_rele 7     //
#define humup_rele 8      //
#define light_rele 9      //

iarduino_RTC watch(RTC_DS1307);     // Налаштування моделі модуля реального часу
LiquidCrystal_I2C lcd(0X27,16,2);   // Налаштування типу LCD дисплею
DHT sens (DHT11);                   // Налаштування моделі датчика вологості і температури
Encoder enc (3, 4, 2);              // Налаштування пінів енкодера (DT, CLK, SW)

byte menu, maxtemp, maxhum, temp, hum, mintemp, minhum, hourset, minset, secset;      // Створення змінних типу byte
byte ontimeh, ontimem, offtimeh, offtimem, dayset, monthset, yearset, dayofweekset;   //
unsigned long sec1, sec2;                                                             // Створення змінних типу unsigned long
boolean light;
void setup(){
  
  watch.begin();
  lcd.begin();
  lcd.setBacklight(HIGH);
  enc.setType(TYPE2);
  enc.setTickMode(AUTO);
  pinMode(tempHumSens,INPUT);
  pinMode(vent_rele,OUTPUT);
  pinMode(tempup_rele,OUTPUT);
  pinMode(humup_rele,OUTPUT);
  pinMode(light_rele,OUTPUT);
  menu = 1;
  light = 1;
  updateMenu();
  
}

void loop(){
  Serial.println(watch.gettimeUnix());

  if(watch.gettimeUnix() - sec1 >= 1){
      sec1 = watch.gettimeUnix();
      temp = sens.readTemperature(5);
      hum = sens.readHumidity(5);    
  }
        
  if(enc.isRightH() && menu < 17){  
    menu++;
    updateMenu();
  }
  
  if(enc.isLeftH() && menu > 1){
    menu--;
    updateMenu();
  }
  
  if(temp > maxtemp || hum > maxhum){
    digitalWrite(vent_rele,HIGH);   
  }else{
    digitalWrite(vent_rele,LOW);
  }
  
  if(temp < mintemp){
    digitalWrite(tempup_rele,HIGH);   
  }else{
    digitalWrite(tempup_rele,LOW);
  }
  
  if(hum < minhum){
    digitalWrite(humup_rele,HIGH);   
  }else{
    digitalWrite(humup_rele,LOW);
  }
  
  if(ontimeh == watch.Hours && ontimem == watch.minutes){
    digitalWrite(light_rele,HIGH);
  }
  
  if(offtimeh == watch.Hours && offtimem == watch.minutes){
    digitalWrite(light_rele,LOW);
  }

  switch (menu){

    case 1:
    if(sec2 != sec1){
      sec2 = sec1;
      updateMenu();
    }
    if (light == 1 && enc.isDouble()){
      light = 0;
      lcd.setBacklight(LOW);
    }
    if (light == 0 && enc.isDouble()){
      light = 1;
      lcd.setBacklight(HIGH);
    }
    break;
    
    case 2:
    if(maxtemp < 40 && enc.isRight()){
      maxtemp++;
      updateMenu();
    }
    if(maxtemp > 0 && enc.isLeft()){
      maxtemp--;
      updateMenu();
    }
    break;
    
    case 3:
    if(maxhum < 100 && enc.isRight()){
       maxhum++;
       updateMenu();
    } 
    if(maxhum > 0 && enc.isLeft()){
      maxhum--;
      updateMenu();
    }
    break;

    case 4:
    if(mintemp < 40 && enc.isRight()){
      mintemp++;
      updateMenu();
    }
    if(mintemp > 0 &&enc.isLeft()){
    mintemp--;
    updateMenu();
    }
    break;

    case 5:
    if(minhum < 100 &&enc.isRight()){
      minhum++;
      updateMenu();
    }
    if(minhum > 0 && enc.isLeft()){
        minhum--;
        updateMenu();
    }
    break;

    case 6:
    if(ontimeh < 23 && enc.isRight()){
      ontimeh++;
      updateMenu();
    }
    if(ontimeh > 0 && enc.isLeft()){
      ontimeh--;
      updateMenu();
    }
    break;

    case 7:
    if(ontimem < 59 && enc.isRight()){
      ontimem++;
      updateMenu();
    }
    if(ontimem > 0 && enc.isLeft()){
      ontimem--;
      updateMenu();
    }
    break;

    case 8:
    if(offtimeh < 23 && enc.isRight()){
      offtimeh++;
      updateMenu();
    }
    if(offtimeh > 0 && enc.isLeft()){
      offtimeh--;
      updateMenu();
    }
    case 9:
    if(offtimem < 59 && enc.isRight()){
      offtimem++;
      updateMenu();
    }
    if(offtimem > 0 && enc.isLeft()){
      offtimem--;
      updateMenu();
    }
    break;

    case 10:
    if(hourset < 23 && enc.isRight()){
      hourset++;
      updateMenu();
    }
    if(hourset > 0 && enc.isLeft()){
      hourset--;
      updateMenu();
    }
    break;

    case 11:
    if(minset < 59 && enc.isRight()){
      minset++;
      updateMenu();
    }
    if(minset > 0 && enc.isLeft()){
      minset--;
      updateMenu();
    }
    break;

    case 12:
    if(secset < 59 && enc.isRight()){
      secset++;
      updateMenu();
    }
    if(secset > 0 && enc.isLeft()){
      secset--;
      updateMenu();
    }
    break;

    case 13:
    if(dayset < 31 && enc.isRight()){
      dayset++;
      updateMenu();
    }
    if(dayset > 1 && enc.isLeft()){
      dayset--;
      updateMenu();
    }
    break;

    case 14:
    if(monthset < 12 && enc.isRight()){
      monthset++;
      updateMenu();
    }
    if(monthset > 1 && enc.isLeft()){
      monthset--;
      updateMenu();
    }
    break;

    case 15:
    if(yearset < 99 && enc.isRight()){
      yearset++;
      updateMenu();
    }
    if(yearset > 0 && enc.isLeft()){
      yearset--;
      updateMenu();
    }
    break;

    case 16:
    if(dayofweekset < 6 && enc.isRight()){
      dayofweekset++;
      updateMenu();
    }
    if(dayofweekset > 0 && enc.isLeft()){
      dayofweekset--;
      updateMenu();
    }
    break;

    case 17:
    if(enc.isDouble()){
      watch.settime(secset, minset, hourset, dayset, monthset, yearset, dayofweekset);
      menu = 18;
      sec1 = 0;
      updateMenu();
    }
    break;
  }
  
}

void updateMenu(){
  
  switch (menu){
    case 1:
    lcd.setCursor(0,0);
    lcd.print("t:");
    lcd.print(temp);
    lcd.print(" ");
    lcd.print(watch.gettime("H:i:sD"));
    lcd.setCursor(0,1);
    lcd.print("h:");
    lcd.print(hum);
    lcd.print(" ");
    lcd.print(watch.gettime("d-m-Y"));
    break;
    
    case 2:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set max TEMP/hum");
    lcd.setCursor(0,1);
    lcd.print(maxtemp);
    lcd.print("/");
    lcd.print(maxhum);
    break;
    
    case 3:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set max temp/HUM");
    lcd.setCursor(0,1);
    lcd.print(maxtemp);
    lcd.print("/");
    lcd.print(maxhum);
    break;
    
    case 4:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set min TEMP/hum");
    lcd.setCursor(0,1);
    lcd.print(mintemp);
    lcd.print("/");
    lcd.print(minhum);
    break;
    
    case 5:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set min temp/HUM");
    lcd.setCursor(0,1);
    lcd.print(mintemp);
    lcd.print("/");
    lcd.print(minhum);
    break;
    
    case 6:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set ON HOUR:min");
    lcd.setCursor(0,1);
    lcd.print(ontimeh);
    lcd.print(":");
    lcd.print(ontimem);
    break;
    
    case 7:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set ON hour:MIN");
    lcd.setCursor(0,1);
    lcd.print(ontimeh);
    lcd.print(":");
    lcd.print(ontimem);
    break;
    
    case 8:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set OFF HOUR:min");
    lcd.setCursor(0,1);
    lcd.print(offtimeh);
    lcd.print(":");
    lcd.print(offtimem);
    break;
    
    case 9:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set OFF hour:MIN");
    lcd.setCursor(0,1);
    lcd.print(offtimeh);
    lcd.print(":");
    lcd.print(offtimem);
    break;

    case 10:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set HOUR:min:sec");
    lcd.setCursor(0,1);
    lcd.print(hourset);
    lcd.print(":");
    lcd.print(minset);
    lcd.print(":");
    lcd.print(secset);
    break;

    case 11:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set hour:MIN:sec");
    lcd.setCursor(0,1);
    lcd.print(hourset);
    lcd.print(":");
    lcd.print(minset);
    lcd.print(":");
    lcd.print(secset);
    break;

    case 12:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set hour:min:SEC");
    lcd.setCursor(0,1);
    lcd.print(hourset);
    lcd.print(":");
    lcd.print(minset);
    lcd.print(":");
    lcd.print(secset);
    break;

    case 13:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set date D-m-y");
    lcd.setCursor(0,1);
    lcd.print(dayset);
    lcd.print("-");
    lcd.print(monthset);
    lcd.print("-");
    lcd.print(yearset);
    break;

    case 14:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set date d-M-y");
    lcd.setCursor(0,1);
    lcd.print(dayset);
    lcd.print("-");
    lcd.print(monthset);
    lcd.print("-");
    lcd.print(yearset);
    break;

    case 15:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set date d-m-Y");
    lcd.setCursor(0,1);
    lcd.print(dayset);
    lcd.print("-");
    lcd.print(monthset);
    lcd.print("-");
    lcd.print(yearset);
    break;

    case 16:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set day of week");
    lcd.setCursor(0,1);
    switch (dayofweekset){
      case 0:
      lcd.print("Sun");
      break;
      
      case 1:
      lcd.print("Mon");
      break;
      
      case 2:
      lcd.print("Tue");
      break;
      
      case 3:
      lcd.print("Wed");
      break;
      
      case 4:
      lcd.print("Thu");
      break;
      
      case 5:
      lcd.print("Fri");
      break;
      
      case 6:
      lcd.print("Sat");
      break;
    }
    break;

    case 17:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("set RTC ");
    lcd.print(hourset);
    lcd.print(":");
    lcd.print(minset);
    lcd.print(":");
    lcd.print(secset);
    lcd.setCursor(0,1);
    lcd.print(dayset);
    lcd.print("-");
    lcd.print(monthset);
    lcd.print("-");
    lcd.print(yearset);
    lcd.print(" ");
    switch (dayofweekset){
      case 0:
      lcd.print("Sun");
      break;
      
      case 1:
      lcd.print("Mon");
      break;
      
      case 2:
      lcd.print("Tue");
      break;
      
      case 3:
      lcd.print("Wed");
      break;
      
      case 4:
      lcd.print("Thu");
      break;
      
      case 5:
      lcd.print("Fri");
      break;
      
      case 6:
      lcd.print("Sat");
      break;
    }
    break;

    case 18:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("RTC SETTED");
    break;
  }
  
}
