#include "recog/cloudtype.h"

//回调函数写入
void serial_write_cb(const recog::Position::ConstPtr &position_msg){
    size_t n = sp.available();
    Protocol(buffer, position_msg);
    sp.write(buffer, n);
}

int main(int argc,char* argv[]){
    ros::init(argc,argv,"serial_port");
    ros::NodeHandle nh;

    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    sp.setPort("/dev/ttyS0");
    sp.setBaudrate(115200);
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


unsigned char* Protocol(unsigned char* buffer,const recog::Position::ConstPtr &position_msg){
    //帧头
    buffer[0] = 0x03;
    buffer[1] = 0xFC;

    //数据部分
        //预处理
    serial_data position_theta;
    position_theta.position = position_msg->theta;
    serial_data position_alpha;
    position_alpha.position = position_msg->alpha;
    serial_data position_distance;
    position_distance.position = position_msg->distance;
    memcpy(buffer+2*sizeof(u_char),&position_theta.data_char,sizeof(position_theta.data_char));
    memcpy(buffer+2*sizeof(u_char)+sizeof(position_theta.data_char),&position_alpha.data_char,sizeof(position_alpha.data_char));
    memcpy(buffer+2*sizeof(u_char)+2*sizeof(position_theta.data_char),&position_distance,sizeof(position_distance.data_char));

    //帧尾
    buffer[14] = 0xFC;
    buffer[15] = 0x03;
}

