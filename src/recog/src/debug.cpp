#include "recog/cloudtype.h"
#include "recog/Position.h"

void debug_cb(const recog::Position::ConstPtr &input){
    ROS_INFO("Theta = %.2f, Alpha = %.2f, Distance = %.2f",input->alpha,input->theta,input->distance);
}

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"Debug_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("position_msg",10,debug_cb);
    ros::spin();
    return 0;
}

