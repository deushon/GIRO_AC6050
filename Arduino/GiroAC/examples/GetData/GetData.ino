#include <SoftwareSerial.h>
#include "GIRO_AC6050.h"

//Полученние данных с датчика через SoftwareSeria
SoftwareSerial MyPORT(6,5); //RX, TX порты для подключения датчика. RX датчика к TX(5) на плате, TX Датчика к RX(6) на плате.
//Передаем созданый порт в конструктор класса датчика.
GiroAC AXER(MyPORT);

void setup() {
  //Инициализируем UART для вывода данных на ПК
  Serial.begin(115200);
  //Инициализируем датчик
  AXER.GiroIni();
}
void loop() {
  //Поочередно читаем и выводим данные с датчика
  //Ускорение 
  if(AXER.GetAc()){
    Serial.println(" данные ускорения:");
    Serial.print("X=");
    Serial.print(AXER.Xac);
    Serial.print("Y=");
    Serial.print(AXER.Yac);
    Serial.print("Z=");
    Serial.println(AXER.Zac);
  }
  else{
    Serial.println("Не удалось получить пакет данных ускорения");
  }

  //Угловая скорость
  if(AXER.GetAv()){
    Serial.println(" данные угловой скорости:");
    Serial.print("X=");
    Serial.print(AXER.Xav);
    Serial.print("Y=");
    Serial.print(AXER.Yav);
    Serial.print("Z=");
    Serial.println(AXER.Zav);
  }
  else{
    Serial.println("Не удалось получить пакет данных угловой скорости");
  }

  //Угол
  if(AXER.GetAn()){
    Serial.println(" данные угла:");
    Serial.print("X=");
    Serial.print(AXER.Xan);
    Serial.print("Y=");
    Serial.print(AXER.Yan);
    Serial.print("Z=");
    Serial.println(AXER.Zan);
  }
  else{
    Serial.println("Не удалось получить пакет данных угла");
  }

  //Температура
  if(AXER.GetT()){
    Serial.println(" данные температуры:");
    Serial.print("t=");
    Serial.println(AXER.TEMPER);
  }
  else{
    Serial.println("Не удалось получить пакет данных температуры");
  }
}
