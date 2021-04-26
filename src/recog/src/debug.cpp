#include "recog/cloudtype.h"
#include "recog/Position.h"

void debug_cb(const recog::Position::ConstPtr &input){
    ROS_INFO("Pitch = %.2f, Yaw = %.2f, Distance = %.2f",input->pitch,input->yaw,input->distance);
    ROS_INFO("\n"); 
}

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"Debug_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("position_msg",10,debug_cb);
    ros::spin();
    return 0;
}