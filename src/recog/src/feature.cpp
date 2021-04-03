#include "recog/cloudtype.h"

//同时订阅发布
class talker_listener_feature{
    private:
        // TypePC2 cloud;

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

void talker_listener_feature::feature_descrip_cb(const sensor_msgs::PointCloud2::ConstPtr &raw_cloud){
    sensor_msgs::PointCloud2 dst_cloud;

    TypeXYZ::Ptr cloud(new TypeXYZ);
    pcl::fromROSMsg(*raw_cloud,*cloud);
    
    //平面分割
    TypeXYZ::Ptr seg_cloud(new TypeXYZ);
    *seg_cloud = Plane_segmentation(cloud);
    // seg_cloud = *cloud;

    //去离群点
    TypeXYZ::Ptr outlier_passthough(new TypeXYZ);
    *outlier_passthough = outlier_fileter(seg_cloud);

    //Transform


    //转为sensor_msgs格式进行发布
    pcl::toROSMsg(*outlier_passthough,dst_cloud);
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


TypeXYZ Plane_segmentation(const TypeXYZ::ConstPtr &input_cloud){
    //初始化系数和点云索引
    pcl::ModelCoefficients::Ptr coefficients(new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers(new pcl::PointIndices);
    
    //创建分割器
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    seg.setOptimizeCoefficients (true);
    //设置分割方式
    seg.setModelType(pcl::SACMODEL_PLANE);
    seg.setMethodType(pcl::SAC_RANSAC);
    seg.setDistanceThreshold(0.01);
    //输入输出
    seg.setInputCloud(input_cloud);
    seg.segment(*inliers, *coefficients);
    //根据索引提取对应点云子集
    TypeXYZ::Ptr output_cloud(new TypeXYZ);
    pcl::copyPointCloud(*input_cloud, *inliers, *output_cloud );

    return *output_cloud;
}

//去离群点
TypeXYZ outlier_fileter(const TypeXYZ::ConstPtr &input_cloud){
    TypeXYZ::Ptr filted(new TypeXYZ);
    TypePC2::Ptr output_cloud(new TypePC2);
    pcl::toPCLPointCloud2(*input_cloud,*output_cloud);
    //滤波
    pcl::StatisticalOutlierRemoval<TypePC2> outlier_remover;
    outlier_remover.setInputCloud(output_cloud);
    outlier_remover.setMeanK(60);
    outlier_remover.setStddevMulThresh(1.5);
    outlier_remover.filter(*output_cloud);
    pcl::fromPCLPointCloud2(*output_cloud,*filted);
    return *filted;
}

//Transform
TypeXYZ transform(const TypeXYZ::ConstPtr &input_cloud){

}