#include "recog/cloudtype.h"

#define Baudrate 115200 //波特率
#define port_name "/dev/ttyUSB0" //串口名

//回调函数写入
void serial_write_cb(const recog::Position::ConstPtr &position_msg){
    size_t n = 16;
    Protocol(buffer, position_msg);
    sp.write(buffer, n);
}

int main(int argc,char* argv[]){
    ros::init(argc,argv,"serial_port");
    ros::NodeHandle nh;

    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    sp.setPort(port_name);
    sp.setBaudrate(Baudrate);
    sp.setTimeout(to);

    try{
        sp.open();
    }
    catch(serial::IOException &e){
        ROS_ERROR("unable to open port");
        return -1;
    }

    if(sp.isOpen()){
        ROS_INFO("port is opened");
    }
    else{
        return -1;
    }

     ros::Subscriber sub = nh.subscribe("position_msg",10,serial_write_cb);

    ros::spin();
    return 0;
}


void Protocol(unsigned char buffer[],const recog::Position::ConstPtr &position_msg){
    //帧头
    buffer[0] = 0x03;
    buffer[1] = 0xFC;

    //数据部分
        //预处理
    unsigned char pitch[4];
    unsigned char yaw[4];
    unsigned char distance[4];
    float_to_byte(position_msg->pitch,pitch);
    float_to_byte(position_msg->yaw,yaw);
    float_to_byte(position_msg->distance,distance);
    
    memcpy(buffer+2*sizeof(u_char),pitch,sizeof(pitch));
    memcpy(buffer+2*sizeof(u_char)+sizeof(pitch),yaw,sizeof(yaw));
    memcpy(buffer+2*sizeof(u_char)+2*sizeof(pitch),distance,sizeof(distance));

    //帧尾
    buffer[14] = 0xFC;
    buffer[15] = 0x03;
    int i = 0;
    while (i<16)
    {
        std::printf("%x ",buffer[i]);
        i++;
    }
    std::printf("\n");
}

