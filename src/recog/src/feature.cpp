#include "recog/cloudtype.h"
#include "recog/clouds.h"

#define normal_Kmean 50 //计算法向量K近邻搜索阈值
#define outlier_mover_Kmean 60 //去离群点搜索阈值
#define outlier_mover_Point_distance 1.5//去离群点点间距阈值
#define region_growing_mincluster 50 //区域生长最小聚类阈值
#define region_growing_maxcluster 10000 //区域生长最大聚类阈值
#define region_growing_numofNeighbors 30 //区域生长近邻点数
#define region_growing_smoothness_Threshold 3.0/180.0 * M_PI //区域生长平滑度函数
#define region_growing_curvature_Threshold 1.0 //区域生长曲率阈值

//同时订阅发布
//ROS类定义
class talker_listener_feature{
    private:
        // TypePC2 cloud;

        ros::NodeHandle nh;
        ros::Subscriber sub;
        ros::Publisher pub;
    public:
        talker_listener_feature(){
            sub = nh.subscribe("filted",10,&talker_listener_feature::feature_descrip_cb,this);
            pub = nh.advertise<recog::clouds>("seg_feature",10);
            // pub = nh.advertise<recog::Position>("position",10);
        };
        void feature_descrip_cb(const sensor_msgs::PointCloud2::ConstPtr &raw_cloud);
};


//回调函数
void talker_listener_feature::feature_descrip_cb(const sensor_msgs::PointCloud2::ConstPtr &raw_cloud){

    TypeXYZ::Ptr cloud(new TypeXYZ);
    pcl::fromROSMsg(*raw_cloud,*cloud);

    // pcl::visualization::CloudViewer viewer("Plane Model");
    // viewer.showCloud(cloud);

    //区域生长
    std::vector<TypeXYZ> seg_clouds;
    seg_clouds = region_growing(cloud);
    ROS_INFO("seg_size : %d",seg_clouds.size());

    //平面分割判断
    std::vector<TypeXYZ> plane_clouds;
    int i = 0;
    while (i < seg_clouds.size())
    {
        TypeXYZ::Ptr seg_cloud(new TypeXYZ);
        *seg_cloud = seg_clouds[i];
        if(Plane_segmentation(seg_cloud))
            plane_clouds.push_back(*seg_cloud);
        // ROS_INFO("here");
        i++;
    }
    // seg_cloud = *cloud;

    // //去离群点
    // TypeXYZ::Ptr outlier_passthough(new TypeXYZ);
    // *outlier_passthough = outlier_fileter();

    //Transform
    //转为sensor_msgs格式进行发布
    
    // ROS_INFO("plane_clouds size : %d",plane_clouds.size());
    i = 0;

    sensor_msgs::PointCloud2 dst_clouds;
    recog::clouds clouds_msgs;
    if(plane_clouds.size() != 0){
        while (i < plane_clouds.size())
        {
            pcl::toROSMsg(plane_clouds[i],dst_clouds);
            clouds_msgs.clouds.push_back(dst_clouds);
            i++;
        }
        // ROS_INFO("send : %d",clouds_msgs.clouds.size());
        pub.publish(clouds_msgs);
    }
    else
        {
            // ROS_INFO("send : 0");
            std::vector<sensor_msgs::PointCloud2> empty_cloud;
            clouds_msgs.clouds = empty_cloud;
            pub.publish(clouds_msgs);
        }
        
}


//主函数
int main(int argc,char * argv[]){
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"feature_node");
    talker_listener_feature tl;
    ros::spin();
}


// ********************************处理函数


//平面分割
bool Plane_segmentation(const TypeXYZ::ConstPtr &input_cloud){
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

    if(output_cloud->size() == 0)
        return 0;
    else 
        return 1;
}



//区域生长
std::vector<TypeXYZ> region_growing(const TypeXYZ::ConstPtr &input_cloud){
    //搜索树初始化
    pcl::search::Search<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);

    //计算法向量
    pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
    pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> normal_estimator;
    normal_estimator.setSearchMethod(tree);//Method
    normal_estimator.setInputCloud(input_cloud);//input
    normal_estimator.setKSearch(normal_Kmean);
    normal_estimator.compute(*normals);//

    //初始化索引
    pcl::IndicesPtr indices(new std::vector <int>);
    //区域生长
    pcl::RegionGrowing<pcl::PointXYZ, pcl::Normal> reg;
    reg.setMinClusterSize(region_growing_mincluster);
    reg.setMaxClusterSize(region_growing_maxcluster);
    reg.setSearchMethod(tree);
    reg.setNumberOfNeighbours(region_growing_numofNeighbors);
    reg.setInputCloud(input_cloud);
    reg.setInputNormals(normals);
    reg.setSmoothnessThreshold(region_growing_smoothness_Threshold);
    reg.setCurvatureThreshold(region_growing_curvature_Threshold);

    std::vector<pcl::PointIndices> clusters;
    std::vector<TypeXYZ> cloud_clusters;
    reg.extract(clusters);
    // ROS_INFO("cluster_size : %d",clusters.size());

    int i = 0;
    while (i < clusters.size())
    {
        TypeXYZ::Ptr temp_cloud(new TypeXYZ);
        pcl::copyPointCloud(*input_cloud, clusters[i],*temp_cloud);
        cloud_clusters.push_back(*temp_cloud);
        i++;
    }

    // pcl::PointCloud<pcl::PointXYZRGB>::Ptr colored_cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
    // colored_cloud = reg.getColoredCloud();
    // pcl::visualization::CloudViewer viewer("Clusters viewer");
    // viewer.showCloud(colored_cloud);
    
    return cloud_clusters;
}




// //去离群点
// TypeXYZ outlier_fileter(const TypeXYZ::ConstPtr &input_cloud){
//     TypeXYZ::Ptr filted(new TypeXYZ);
//     TypePC2::Ptr output_cloud(new TypePC2);
//     pcl::toPCLPointCloud2(*input_cloud,*output_cloud);
//     //滤波
//     pcl::StatisticalOutlierRemoval<TypePC2> outlier_remover;
//     outlier_remover.setInputCloud(output_cloud);
//     outlier_remover.setMeanK(outlier_mover_Kmean);
//     outlier_remover.setStddevMulThresh(outlier_mover_Point_distance);
//     outlier_remover.filter(*output_cloud);
//     pcl::fromPCLPointCloud2(*output_cloud,*filted);
//     return *filted;
// }

// //Transform
// TypeXYZ transform(const TypeXYZ::ConstPtr &input_cloud){

// }