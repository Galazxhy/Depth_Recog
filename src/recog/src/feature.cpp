#include "ros/ros.h"
#include "pcl/point_types.h"
#include "pcl_ros/point_cloud.h"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl/io/pcd_io.h"
#include "recog/Position.h"

#include "pcl/features/shot_omp.h"
#include "pcl/correspondence.h"
#include "pcl/features/normal_3d.h"
#include "pcl/features/board.h"
#include "pcl/filters/uniform_sampling.h"
#include "pcl/recognition/cg/hough_3d.h"
#include "pcl/recognition/cg/geometric_consistency.h"
#include "pcl/kdtree/kdtree_flann.h"
#include "pcl/kdtree/impl/kdtree_flann.hpp"
#include "pcl/common/transforms.h"
#include "pcl/console/parse.h"
#include "pcl/visualization/pcl_visualizer.h"
#include "pcl/visualization/cloud_viewer.h"

//同时订阅发布
class talker_listener_feature{
    private:
        // pcl::PCLPointCloud2 cloud;

        ros::NodeHandle nh;
        ros::Subscriber sub;
        ros::Publisher pub;
    public:
        talker_listener_feature(){
            sub = nh.subscribe("filted",10,&talker_listener_feature::feature_descrip_cb,this);
            pub = nh.advertise<sensor_msgs::PointCloud2>("feature",10);
            // pub = nh.advertise<recog::Position>("position",10);
        };
        void feature_descrip_cb(const sensor_msgs::PointCloud2::ConstPtr &raw_cloud);
};

pcl::PointCloud<pcl::Normal> normal_estimation(const pcl::PointCloud<pcl::PointXYZ>::Ptr &cloud){
    //法线估计重建
    //创建法线估计的对象并输入
        pcl::NormalEstimation<pcl::PointXYZ,pcl::Normal> ne;
        ne.setInputCloud(cloud); 
    
    //创建空kd树，通过kd树重建
        pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>());
        ne.setSearchMethod(tree);

    // 创建输出对象
    pcl::PointCloud<pcl::Normal>::Ptr cloud_normal(new pcl::PointCloud<pcl::Normal>());
    
    //设置判定为邻点的半径阈值
    ne.setRadiusSearch(0.03);
    
    //估计
    ne.compute(*cloud_normal);

    return(*cloud_normal)
}



void talker_listener_feature::feature_descrip_cb(const sensor_msgs::PointCloud2::ConstPtr &raw_cloud){
    sensor_msgs::PointCloud2 dst_cloud;

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
    pcl::fromROSMsg(*raw_cloud,*cloud);

    //法线估计
    pcl::PointCloud<pcl::Normal> normal;
    normal = normal_estimation(cloud);
    

    //

    //可视化
    // pcl::visualization::CloudViewer viewer("CloudViewer");
    // viewer.showCloud(cloud);

    //转为sensor_msgs格式进行发布
    pcl::toROSMsg(normal,dst_cloud);
    dst_cloud.header.frame_id = "feature";
    dst_cloud.is_dense = false;

    pub.publish(dst_cloud);

}

int main(int argc,char * argv[]){
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"feature_node");
    talker_listener_feature tl;
    ros::spin();
}