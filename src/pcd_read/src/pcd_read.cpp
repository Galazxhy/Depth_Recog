#include "ros/ros.h"
#include "pcl/point_types.h"
#include "pcl_ros/point_cloud.h"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl/io/pcd_io.h"
// #include "pcl/visualization/pcl_visualizer.h"
#include <iostream>
#include <sstream>

std::string num2str(int i){
    std::stringstream ss;
    ss<<i;
    return ss.str();
}


int main(int argc, char*argv[])
{
    setlocale(LC_ALL,"");
    //初始化ROS
    ros::init(argc,argv,"PCD_read_pub_node");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<sensor_msgs::PointCloud2>("PCDcloud",10);

    //参数指定，读入PCD文件
    // int i = atoi(argv[1]);
    // int i = 1;
    sensor_msgs::PointCloud2 cloud;
    // if(argc == 2){
    // std::string fname = "cloud" + num2str(i) +".pcd"; 
    std::string fname = "/home/galazxhy/Depth_Recog/cloud1.pcd"; 
    pcl::io::loadPCDFile(fname,cloud);
    // }
    // else{
        // ROS_INFO("参数个数不对");
        // return 1;
    // }

     //设header 以便能在rviz中打开,设is_dense
    cloud.header.frame_id = "cloud_read";
    cloud.is_dense = false;

    //设置发布频率
    ros::Rate rate(0.5);

    while(ros::ok()){
        //  i++;
    pub.publish(cloud);
    ROS_INFO("已发布...");

        rate.sleep();
     }

    ros::spinOnce();
    return 0;
}
