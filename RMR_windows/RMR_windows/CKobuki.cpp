#include "CKobuki.h"
#include"pch.h"

int CKobuki::checkChecksum(unsigned char * data)
{//najprv hlavicku
    unsigned char chckSum = 0;
    for (int i = 0; i < data[0]+2; i++)
    {
        chckSum ^= data[i];
    }
    return chckSum;
}

std::vector<unsigned char> CKobuki::setLed(int led1, int led2)
{

    unsigned char message[8] = {0xaa,0x55,0x04,0x0c,0x02,0x00,(unsigned char)((led1+led2*4)%256),0x00};
    message[7] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6];

    std::vector<unsigned char> vystup(message,message+sizeof(message)/sizeof(message[0]));
        return vystup;
}



std::vector<unsigned char> CKobuki::setTranslationSpeed(int mmpersec)
{
    unsigned char message[14] = { 0xaa,0x55,0x0A,0x0c,0x02,0xf0,0x00,0x01,0x04,(unsigned char)(mmpersec%256),(unsigned char)(mmpersec>>8),0x00,0x00,  0x00 };
    message[13] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11] ^ message[12];


    std::vector<unsigned char> vystup(message,message+sizeof(message)/sizeof(message[0]));
        return vystup;

}

std::vector<unsigned char> CKobuki::setRotationSpeed(double radpersec)
{
    int speedvalue = radpersec * 230.0f / 2.0f;
    unsigned char message[14] = { 0xaa,0x55,0x0A,0x0c,0x02,0xf0,0x00,0x01,0x04,(unsigned char)(speedvalue % 256),(unsigned char)(speedvalue >>8),0x01,0x00,  0x00 };
    message[13] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11] ^ message[12];


    std::vector<unsigned char> vystup(message,message+sizeof(message)/sizeof(message[0]));
        return vystup;
}

std::vector<unsigned char> CKobuki::setArcSpeed(int mmpersec, int radius)
{
    if (radius == 0) {
        return setTranslationSpeed(mmpersec);

    }
 
    int speedvalue = mmpersec * ((radius + (radius>0? 230:-230) )/ 2 ) / radius;

    unsigned char message[14] = { 0xaa,0x55,0x0A,0x0c,0x02,0xf0,0x00,0x01,0x04,(unsigned char)(speedvalue % 256),(unsigned char)(speedvalue >>8),(unsigned char)(radius % 256),(unsigned char)(radius >>8),  0x00 };
    message[13] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7] ^ message[8] ^ message[9] ^ message[10] ^ message[11] ^ message[12];

    std::vector<unsigned char> vystup(message,message+sizeof(message)/sizeof(message[0]));
        return vystup;
}


std::vector<unsigned char> CKobuki::setSound(int noteinHz, int duration)
{
    int notevalue = floor((double)1.0 / ((double)noteinHz*0.00000275) + 0.5);
    unsigned char message[13] = { 0xaa,0x55,0x09,0x0c,0x02,0xf0,0x00,0x03,0x03,(unsigned char)(notevalue%256),(unsigned char)(notevalue>>8),(unsigned char)(duration%256),0x00 };
    message[12] = message[2] ^ message[3] ^ message[4] ^ message[5] ^ message[6] ^ message[7]^ message[8]^ message[9]^ message[10]^ message[11];


std::vector<unsigned char> vystup(message,message+sizeof(message)/sizeof(message[0]));
    return vystup;
}



std::vector<unsigned char> CKobuki::setDefaultPID()
{
    unsigned char message[23] = { 0xaa,0x55,0x13,0x0c,0x02,0xf0,0x00,0x0D,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,  0x00 };
    message[22]=0;
    for(int i=0;i<23-3;i++)
    {
        message[22]=message[22]^message[i+2];
    }


    std::vector<unsigned char> vystup(message,message+sizeof(message)/sizeof(message[0]));
        return vystup;
}





int CKobuki::parseKobukiMessage(TKobukiData &output, unsigned char * data)
{
    int rtrnvalue = checkChecksum(data);
    //ak je zly checksum,tak kaslat na to
    if (rtrnvalue != 0)
        return -2;

    int checkedValue = 1;
    //kym neprejdeme celu dlzku
    while (checkedValue < data[0])
    {
        //basic data subload
        if (data[checkedValue] == 0x01)
        {
            checkedValue++;
            if (data[checkedValue ] != 0x0F)
                return -1;
            checkedValue++;
            output.timestamp = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.BumperCenter = data[checkedValue ] && 0x02;
            output.BumperLeft = data[checkedValue] && 0x04;
            output.BumperRight = data[checkedValue] && 0x01;
            checkedValue++;
            output.WheelDropLeft= data[checkedValue] && 0x02;
            output.WheelDropRight = data[checkedValue] && 0x01;
            checkedValue++;
            output.CliffCenter = data[checkedValue] && 0x02;
            output.CliffLeft = data[checkedValue] && 0x04;
            output.CliffRight = data[checkedValue] && 0x01;
            checkedValue++;
            output.EncoderLeft = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.EncoderRight = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.PWMleft = data[checkedValue] ;
            checkedValue++;
            output.PWMright = data[checkedValue] ;
            checkedValue++;
            output.ButtonPress = data[checkedValue];
            checkedValue++;
            output.Charger = data[checkedValue];
            checkedValue++;
            output.Battery = data[checkedValue];
            checkedValue++;
            output.overCurrent = data[checkedValue];
            checkedValue++;
        }
        else if (data[checkedValue] == 0x03)
        {
            checkedValue++;
            if (data[checkedValue] != 0x03)
                return -3;
            checkedValue++;
            output.IRSensorRight = data[checkedValue];
            checkedValue++;
            output.IRSensorCenter = data[checkedValue];
            checkedValue++;
            output.IRSensorLeft = data[checkedValue];
            checkedValue++;
        }
        else if (data[checkedValue] == 0x04)
        {
            checkedValue++;
            if (data[checkedValue] != 0x07)
                return -4;
            checkedValue++;
            output.GyroAngle = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.GyroAngleRate = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 5;//3 unsued
        }
        else if (data[checkedValue] == 0x05)
        {
            checkedValue++;
            if (data[checkedValue] != 0x06)
                return -5;
            checkedValue++;
            output.CliffSensorRight = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.CliffSensorCenter = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.CliffSensorLeft = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
        }
        else if (data[checkedValue] == 0x06)
        {
            checkedValue++;
            if (data[checkedValue] != 0x02)
                return -6;
            checkedValue++;
            output.wheelCurrentLeft =  data[checkedValue];
            checkedValue ++;
            output.wheelCurrentRight =data[checkedValue];
            checkedValue ++;

        }
        else if (data[checkedValue] == 0x0A)
        {
            checkedValue++;
            if (data[checkedValue] != 0x04)
                return -7;
            checkedValue++;
            output.extraInfo.HardwareVersionPatch = data[checkedValue];
            checkedValue++;
            output.extraInfo.HardwareVersionMinor = data[checkedValue];
            checkedValue++;
            output.extraInfo.HardwareVersionMajor = data[checkedValue];
            checkedValue += 2;

        }
        else if (data[checkedValue] == 0x0B)
        {
            checkedValue++;
            if (data[checkedValue] != 0x04)
                return -8;
            checkedValue++;
            output.extraInfo.FirmwareVersionPatch = data[checkedValue];
            checkedValue++;
            output.extraInfo.FirmwareVersionMinor = data[checkedValue];
            checkedValue++;
            output.extraInfo.FirmwareVersionMajor = data[checkedValue];
            checkedValue += 2;

        }
        else if (data[checkedValue] == 0x0D)
        {
            checkedValue++;
            if (data[checkedValue]%2 !=0)
                return -9;
            checkedValue++;
            output.frameId = data[checkedValue];
            checkedValue++;
            int howmanyFrames = data[checkedValue]/3;
            checkedValue++;
            output.gyroData.reserve(howmanyFrames);
            output.gyroData.clear();
            for (int hk = 0; hk < howmanyFrames; hk++)
            {
                TRawGyroData temp;
                temp.x = data[checkedValue + 1] * 256 + data[checkedValue];
                checkedValue += 2;
                temp.y = data[checkedValue + 1] * 256 + data[checkedValue];
                checkedValue += 2;
                temp.z = data[checkedValue + 1] * 256 + data[checkedValue];
                checkedValue += 2;
                output.gyroData.push_back(temp);
            }
        }
        else if (data[checkedValue] == 0x10)
        {
            checkedValue++;
            if (data[checkedValue] != 0x10)
                return -10;
            checkedValue++;
            output.digitalInput = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.analogInputCh0 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.analogInputCh1 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.analogInputCh2 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 2;
            output.analogInputCh3 = data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 8;//2+6


        }
        else if (data[checkedValue] == 0x13)
        {
            checkedValue++;
            if (data[checkedValue] != 0x0C)
                return -11;
            checkedValue++;
            output.extraInfo.UDID0 = data[checkedValue + 3] * 256*256*256+ data[checkedValue + 2] * 256*256+ data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 4;
            output.extraInfo.UDID1 = data[checkedValue + 3] * 256 * 256 * 256 + data[checkedValue + 2] * 256 * 256 +data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 4;
            output.extraInfo.UDID2 = data[checkedValue + 3] * 256 * 256 * 256 + data[checkedValue + 2] * 256 * 256 +data[checkedValue + 1] * 256 + data[checkedValue];
            checkedValue += 4;
        }
        else
        {
            checkedValue++;
            checkedValue += data[checkedValue] + 1;
        }
    }
    return 0;
}





