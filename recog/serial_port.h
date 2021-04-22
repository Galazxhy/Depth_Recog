#include "serial/serial.h"
#include "recog/Position.h"

serial::Serial sp;

//缓冲区
unsigned char buffer[16];

//浮点数和十六进制字符串转换
typedef union{
    float data;
    unsigned long data_hex;
}serial_data;

void float_to_byte(float data,unsigned char byte[]){
    serial_data sd;
    sd.data = data;
    byte[0] = (unsigned char)sd.data_hex;
    byte[1] = (unsigned char)(sd.data_hex>>8);
    byte[2] = (unsigned char)(sd.data_hex>>16);
    byte[3] = (unsigned char)(sd.data_hex>>24);
}

void Protocol(unsigned char buffer[],const recog::Position::ConstPtr &position_msg);
