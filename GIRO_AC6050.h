#pragma once
#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial GIROSerial(0, 0); //BADCODE
class GiroAC {
public:

    //RAW DATA
    //ac - ���������; av - ������� ��������; an - ����. �������������� ������ �� ���� X, Y, Z.
    //ac - Acceleration; av - Angular velocity; an - Angle. Data from X, Y, Z Axises.
    double rawXac, rawYac, rawZac, rawXav, rawYav, rawZav, rawXan, rawYan, rawZan, rawTEMPER;

    //CALIBRATE DATA
    double g; //��������� ���������� �������, �� ��������� 9,8. G-Force;
    double tempShift; //���������� �������� �����������. Temp. Corrector;

    //PROFIT DATA
    double Xac, Yac, Zac, Xav, Yav, Zav, Xan, Yan, Zan, TEMPER;

    //����� �� ���������. ����������� UART
    
    GiroAC() {

        UARTdefault = true;
        TargetSearchLimit = 16;
        LinkTimeOut = 1000;
        tempShift = 36.53;
        g = 9.8;
        BaudRate = 115200;
    }

    //����������� UART
    //Config mode, use software UART.

    GiroAC(SoftwareSerial Port){

        UARTdefault = false;
        GIROSerial = Port;  //BADCODE �������� �� ������?
        TargetSearchLimit = 16;
        LinkTimeOut = 1000;
        tempShift = 36.53;
        g = 9.8;
        BaudRate = 115200; //�������� � �������������� ����� ����� ��� ����������?
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
        //����� �� ������������� �������.
        delay(2000);
        return(true);
    }

    //��������� ������; Geting DATA;
    //������� �������� ����� ������ ������ �� UART �� ��������� TimeOut � ������� ��������� �� 16 ���. ���� ����� �� ����������, ���������� ������ false; � ������ ������ true.
    //If no DATA on UART, or Data pack marker not found before TimeOut exp, or not valid data pack name >16 packs turn false, else, if all okay - true;

    //RAW
    //���������; Acceleration.
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
                //��������� ��������.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //������ �� ������ ���������� �������.
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

    //������� ��������; Angular velocity.
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
                //��������� ��������.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //������ �� ������ ���������� �������.
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

    //����; Angle.
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
                //��������� ��������.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //������ �� ������ ���������� �������.
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

    //�����������; Temperature.
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
                //��������� ��������.
                if (timeout < (millis() - LinkTimeOut))
                    return(dataGETED);
                //������ �� ������ ���������� �������.
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
    //RAW DATA ��� �� ����� ���������; RAW DATA will update to.
    //���������; Acceleration.
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

    //������� ��������; Angular velocity.
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

    //����; Angle.
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

    //�����������; Temperature.
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
    //����������� ������ ������ c �������� �� UART; true - ����������� ������� RX TX �����; false - ������������� ������������ UART SoftwareSerial.
    //UART settings, true - stock RX-TX Arduino ports; false - progframm RX-TX ports by SoftwareSerial.
    bool UARTdefault;

    //BADCODE ��� ��������� ������ �� ������������, ���������� �������� ����������� SOFTWAREserial � �����.
    int SoftUARTrx, SoftUARTtx; //����� RX-TX ��� ������������ UART; SoftwareSerial RX-TX ports.
    int BaudRate; //������� �������������� � �������� 115200 (100Hz ����� ���� �������) �� ���������, ��� 9600 (20Hz) ��� ������������� �������. Baud rate, can use 2 mode 115200 (100Hz frame rate of sensor) def mode, or 9600 (20Hz) if change sensor Setings.

    //���� ��� � ���-�� ������� ����� ������ �����.
    int TargetSearchLimit;
    int LinkTimeOut;

    //��������� ���� ��������� ������, ��� �������� �����������.
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

//��������� ������� ������ � �������. ���� ������� �������� ����� ���������� true, ����� false.
//Get data pack from sensor. If pack geted return true, else false.
bool getPack() {

    bool HAVEDATA;

    //��� ������ ����������
    if (UARTdefault) {
        HAVEDATA = Serial.available();
    }
    else {
        HAVEDATA = GIROSerial.available();
    }

    while (HAVEDATA) {

        //��� ������ ����������
        if (UARTdefault) {
            BBUF = Serial.read();
        }
        else {
            BBUF = GIROSerial.read();
        }
        
        //��� ������ ����� ������ ������ - �������� ���           
        if (BBUF == 0x55) {
            buffer[0] = 0x55;
            bcounter = 0;
            for (int i = 1; i <= PACSIZE; i++) {
                
                //��� ������ ����������
                if (UARTdefault) {
                    BBUF = Serial.read();
                }
                else {
                    BBUF = GIROSerial.read();
                }
                buffer[i] = BBUF;
            }
            hasher = getHash(buffer, PACSIZE); //�������� ��� ������ ��� �������� �����������
            if (hasher == buffer[10])
                return(true);
            else
                return(false);
        }

        //��� ������ ����������
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