#include "ros/ros.h"
#include "sensor_msgs/PointCloud2.h"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl_ros/point_cloud.h"
#include "pcl/point_types.h"
#include "pcl/io/pcd_io.h"

#include "pcl/filters/voxel_grid.h"
#include "pcl/filters/passthrough.h"
#include "pcl/filters/statistical_outlier_removal.h"

class talker_listener{
    private:
    // pcl::PCLPointCloud2 cloud;

    ros::NodeHandle nh;
    ros::Subscriber sub;
    ros::Publisher pub;

    public:
        talker_listener(){
            //从相机读
            sub = nh.subscribe("/camera/depth/points",10,&talker_listener::raw_cloud_cb,this);
            //从PCD文件读
            //sub = nh.subscribe("PCDcloud",10,&talker_listener::raw_cloud_cb,this);

            pub = nh.advertise<sensor_msgs::PointCloud2>("filted",10);
        };
        void raw_cloud_cb(const sensor_msgs::PointCloud2::ConstPtr &camera_input);
};

void talker_listener::raw_cloud_cb(const sensor_msgs::PointCloud2::ConstPtr &camera_input){
    //定义对象实现订阅同时发布
        sensor_msgs::PointCloud2 filted_cloud;
    
    //转为PCLPointCloud2格式
        pcl::PCLPointCloud2 cloud;
        pcl::PointCloud<pcl::PointXYZ> temp_cloud;
        pcl::fromROSMsg(*camera_input,temp_cloud);
        pcl::toPCLPointCloud2(temp_cloud,cloud);

        //创建滤波后pointcloud2对象
        pcl::PCLPointCloud2::Ptr cloud_ptr(new pcl::PCLPointCloud2());
        pcl::PCLPointCloud2::Ptr cloud_passfilted(new pcl::PCLPointCloud2());
        pcl::PCLPointCloud2::Ptr cloud_voxelfilted(new pcl::PCLPointCloud2());
        pcl::PCLPointCloud2::Ptr cloud_outlierfilted(new pcl::PCLPointCloud2());
        *cloud_ptr = cloud;
    
    //进行直通滤波
        pcl::PassThrough<pcl::PCLPointCloud2> pass;
        pass.setInputCloud(cloud_ptr);
        pass.setFilterFieldName("z");
        pass.setFilterLimits(0.0,1.0);
        pass.setFilterLimitsNegative(false);
        pass.filter(*cloud_passfilted);

    //进行体素滤波
        //滤波器
        pcl::VoxelGrid<pcl::PCLPointCloud2> sor;
        sor.setInputCloud(cloud_passfilted);//输入
        sor.setLeafSize(0.01f, 0.01f, 0.01f);//0.01f表示体素为1cm立方体
        sor.filter(*cloud_voxelfilted);

    //去离群点
        pcl::StatisticalOutlierRemoval<pcl::PCLPointCloud2> outlier_remover;
        outlier_remover.setInputCloud(cloud_voxelfilted);
        outlier_remover.setMeanK(100);
        outlier_remover.setStddevMulThresh(1);
        outlier_remover.filter(*cloud_outlierfilted);

    // //保存为PCD文件
    // pcl::io::savePCDFile("cloud_filted.pcd",*cloud_outlierfilted);

    // ROS_INFO("Saved");
    
    //转为sensor_msgs格式发布
    // ros::Rate rate(1);
    pcl::fromPCLPointCloud2(*cloud_outlierfilted,temp_cloud);
    // temp_cloud.header.frame_id = "filtered";
    // temp_cloud.is_dense = false;
    pcl::toROSMsg(temp_cloud,filted_cloud);
    filted_cloud.header.frame_id = "filted";
    filted_cloud.is_dense = false;
    pub.publish(filted_cloud);
    // ROS_INFO("正在发布...");
    // rate.sleep();
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"read_in_filter_node");
    talker_listener tl;
    //ros::Subscriber sub = nh.subscribe("/camera/depth/points",10,raw_cloud_cb);
    //ros::Subscriber sub = nh.subscribe("PCDcloud",10,raw_cloud_cb);
    ros::spin();

    return 0;
}



