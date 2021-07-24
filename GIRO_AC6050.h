#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial GIROSerial(0, 0); //BADCODE
class GiroAC {
public:

    //RAW DATA
    //ac - ускорение; av - угловая скорость; an - угол. Соответственно данные по осям X, Y, Z.
    //ac - Acceleration; av - Angular velocity; an - Angle. Data from X, Y, Z Axises.
    double rawXac, rawYac, rawZac, rawXav, rawYav, rawZav, rawXan, rawYan, rawZan, rawTEMPER;

    //CALIBRATE DATA
    double g; //Ускорение свободного падения, по умолчанию 9,8. G-Force;
    double tempShift; //Постоянное смещение температуры. Temp. Corrector;

    //PROFIT DATA
    double Xac, Yac, Zac, Xav, Yav, Zav, Xan, Yan, Zan, TEMPER;

    //Режим по умолчанию. Стандартный UART
    
    GiroAC() {

        UARTdefault = true;
        TargetSearchLimit = 16;
        LinkTimeOut = 1000;
        tempShift = 36.53;
        g = 9.8;
        BaudRate = 115200;
    }

    //Программный UART
    //Config mode, use software UART.

    GiroAC(SoftwareSerial Port){

        UARTdefault = false;
        GIROSerial = Port;  //BADCODE Заменить на ссылки?
        TargetSearchLimit = 16;
        LinkTimeOut = 1000;
        tempShift = 36.53;
        g = 9.8;
        BaudRate = 115200; //Пробелма с инициализацией порта через эту переменную?
    }

    //Initialization
    bool GiroIni() {
        if (!UARTdefault) {
            GIROSerial.begin(115200);
            GIROSerial.listen();
        }
        else
        {
            Serial.begin(115200);
        }
        //Время на инициализацию датчика.
        delay(2000);
        return(true);
    }

    //Получение данных; Geting DATA;
    //Функции пытаются найти начало пакета по UART до истечения TimeOut и целевой заголовок до 16 раз. Если этого не происходит, возвращает ошибку false; в случае успеха true.
    //If no DATA on UART, or Data pack marker not found before TimeOut exp, or not valid data pack name >16 packs turn false, else, if all okay - true;

    //RAW
    //Ускорение; Acceleration.
    bool GetAcRAW() {
        bool dataGETED=false;
        int ERETIK=0;
        unsigned long timeout = 0;
        while (!dataGETED)
        {
            ERETIK++;
            timeout = millis();
            while (!getPack())
            {
                //Отработка таймаута.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //Защита от сброса системного таймера.
                if (timeout > millis())
                    break;
            }
            if (buffer[1] == 0x51) {
                rawXac = ((buffer[3] << 8) | buffer[2]);
                rawYac = ((buffer[5] << 8) | buffer[4]);
                rawZac = ((buffer[7] << 8) | buffer[6]);
                dataGETED=true;
            }
            if (ERETIK > TargetSearchLimit) {
                break;
            }
        }
        return(dataGETED);
    }

    //Угловая скорость; Angular velocity.
    bool GetAvRAW() {
        bool dataGETED = false;
        int ERETIK = 0;
        unsigned long timeout = 0;
        while (!dataGETED)
        {
            ERETIK++;
            timeout = millis();
            while (!getPack())
            {
                //Отработка таймаута.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //Защита от сброса системного таймера.
                if (timeout > millis())
                    break;
            }
            if (buffer[1] == 0x52) {
                rawXav = ((buffer[3] << 8) | buffer[2]);
                rawYav = ((buffer[5] << 8) | buffer[4]);
                rawZav = ((buffer[7] << 8) | buffer[6]);
                dataGETED = true;
            }
            if (ERETIK > TargetSearchLimit) {
                break;
            }
        }
        return(dataGETED);
    }

    //Угол; Angle.
    bool GetAnRAW() {
        bool dataGETED = false;
        int ERETIK = 0;
        unsigned long timeout = 0;
        while (!dataGETED)
        {
            ERETIK++;
            timeout = millis();
            while (!getPack())
            {
                //Отработка таймаута.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //Защита от сброса системного таймера.
                if (timeout > millis())
                    break;
            }
            if (buffer[1] == 0x53) {
                rawXan = ((buffer[3] << 8) | buffer[2]);
                rawYan = ((buffer[5] << 8) | buffer[4]);
                rawZan = ((buffer[7] << 8) | buffer[6]);
                dataGETED = true;
            }
            if (ERETIK > TargetSearchLimit) {
                break;
            }
        }
        return(dataGETED);
    }

    //Температура; Temperature.
    bool GetTRAW() {
        bool dataGETED = false;
        int ERETIK = 0;
        unsigned long timeout = 0;
        while (!dataGETED)
        {
            ERETIK++;
            timeout = millis();
            while (!getPack())
            {
                //Отработка таймаута.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //Защита от сброса системного таймера.
                if (timeout > millis())
                    break;
            }
            if (buffer[1] == 0x51 || buffer[1] == 0x52 || buffer[1] == 0x53) {
                rawTEMPER = ((buffer[9] << 8) | buffer[8]);
                dataGETED = true;
            }
            if (ERETIK > TargetSearchLimit) {
                break;
            }
        }
        return(dataGETED);
    }


    //Profit DATA;
    //RAW DATA так же будет обновлена; RAW DATA will update to.
    //Ускорение; Acceleration.
    bool GetAc() {
        if (GetAcRAW()) {
            Xac = rawXac / 32768 * 16 * g;
            Yac = rawYac / 32768 * 16 * g;
            Zac = rawZac / 32768 * 16 * g;
            return(true);
        }
        else
            return(false);
    }

    //Угловая скорость; Angular velocity.
    bool GetAv() {
        if (GetAvRAW()) {
            Xav = rawXav  / 32768 * 2000;
            Yav = rawYav  / 32768 * 2000;
            Zav = rawZav  / 32768 * 2000;
            return(true);
        }
        else
            return(false);
    }

    //Угол; Angle.
    bool GetAn() {
        if (GetAnRAW()) {
            Xan = rawXan / 32768 * 180;
            Yan = rawYan / 32768 * 180;
            Zan = rawZan / 32768 * 180;
            return(true);
        }
        else
            return(false);
    }

    //Температура; Temperature.
    bool GetT() {
        if (GetTRAW()) {
            TEMPER = rawTEMPER / 340 + tempShift;
            return(true);
        }
        else
            return(false);
    }

private:

    //UART DATA
    byte buffer[10];
    byte BBUF;
    int bcounter = 0;
    byte hasher;
    const int PACSIZE = 10;

    //SETTINGS
    //Определение режима работы c датчиком по UART; true - Стандартные разьемы RX TX платы; false - Использование программного UART SoftwareSerial.
    //UART settings, true - stock RX-TX Arduino ports; false - progframm RX-TX ports by SoftwareSerial.
    bool UARTdefault;

    //BADCODE Эти параметры сейчас не используется, необходимо починить подключение SOFTWAREserial к библе.
    int SoftUARTrx, SoftUARTtx; //Порты RX-TX для виртуального UART; SoftwareSerial RX-TX ports.
    int BaudRate; //Ботрейт взаимодействия с датчиком 115200 (100Hz Фрейм рейт датчика) по умолчанию, или 9600 (20Hz) при перенастройке датчика. Baud rate, can use 2 mode 115200 (100Hz frame rate of sensor) def mode, or 9600 (20Hz) if change sensor Setings.

    //Тайм аут и кол-во попыток найти нужный пакет.
    int TargetSearchLimit;
    int LinkTimeOut;

    //Получение хэша принятого пакета, для првоерки целостности.
    //Get Hash from geted Data Pack, for check it.
    byte getHash(byte* data, int length) {
        byte hash = 0;
        int i = 0;
        while (length--) {
            hash += *(data + i);
            i++;
        }
        return hash;
    }

//Получение пакетов данных с сенсора. Если получен валидный пакет возвращает true, иначе false.
//Get data pack from sensor. If pack geted return true, else false.
bool getPack() {

    bool HAVEDATA;

    //Для выбора интерфейса
    if (UARTdefault) {
        HAVEDATA = Serial.available();
    }
    else {
        HAVEDATA = GIROSerial.available();
    }

    while (HAVEDATA) {

        //Для выбора интерфейса
        if (UARTdefault) {
            BBUF = Serial.read();
        }
        else {
            BBUF = GIROSerial.read();
        }
        
        //Как только нашли начало пакета - собираем его           
        if (BBUF == 0x55) {
            buffer[0] = 0x55;
            bcounter = 0;
            for (int i = 1; i <= PACSIZE; i++) {
                
                //Для выбора интерфейса
                if (UARTdefault) {
                    BBUF = Serial.read();
                }
                else {
                    BBUF = GIROSerial.read();
                }
                buffer[i] = BBUF;
            }
            hasher = getHash(buffer, PACSIZE); //Получаем хэш пакета для проверки целостности
            if (hasher == buffer[10])
                return(true);
            else
                return(false);
        }

        //Для выбора интерфейса
        if (UARTdefault) {
            HAVEDATA = Serial.available();
        }
        else {
            HAVEDATA = GIROSerial.available();
        }
    }
    return(false);
}  
};