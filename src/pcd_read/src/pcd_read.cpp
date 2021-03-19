#include "ros/ros.h"
#include "sensor_msgs/PointCloud2.h"
#include "pcl/point_types.h"
#include "pcl/io/pcd_io.h"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl_ros/point_cloud.h"
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
    int i = 1;
    sensor_msgs::PointCloud2 cloud;
    // if(argc == 2){
    std::string fname = "cloud" + num2str(i) +".pcd"; 
    pcl::io::loadPCDFile(fname,cloud);
    // }
    // else{
        // ROS_INFO("参数个数不对");
        // return 1;
    // }

     //设header 以便能在rviz中打开,设is_dense
    cloud.header.frame_id = "cloud_read";
    cloud.is_dense = false;

    //相机读入以及可视化
    // pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_view(new pcl::PointCloud<pcl::PointXYZ>);
    // pcl::fromROSMsg(cloud,*cloud_view);
    // boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("Viewer"));

    // viewer->setBackgroundColor(0,0,0);
    // viewer->addPointCloud(cloud_view,"sample");

    // viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2 ,"sample cloud");
    
    // viewer->addCoordinateSystem(1.0);

    // viewer->initCameraParameters();

    //设置发布频率
    ros::Rate rate(0.5);

    while(ros::ok()){
         i++;
    pub.publish(cloud);
    ROS_INFO("已发布...");

        rate.sleep();
     }

    ros::spin();
    return 0;
}
