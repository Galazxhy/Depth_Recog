#include "recog/cloudtype.h"

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
            // sub = nh.subscribe("PCDcloud",10,&talker_listener::raw_cloud_cb,this);

            pub = nh.advertise<sensor_msgs::PointCloud2>("filted",10);
        };
        void raw_cloud_cb(const sensor_msgs::PointCloud2::ConstPtr &camera_input);
};

void talker_listener::raw_cloud_cb(const sensor_msgs::PointCloud2::ConstPtr &camera_input){
    //定义对象实现订阅同时发布
        sensor_msgs::PointCloud2 filted_cloud;
    
    //转为PCLPointCloud2格式
        TypePC2::Ptr cloud(new TypePC2);
        TypeXYZ::Ptr temp_cloud(new TypeXYZ);
        pcl::fromROSMsg(*camera_input,*temp_cloud);

        //创建滤波后对象
        TypeXYZ::Ptr cloud_condditionfilted(new TypeXYZ);
        TypePC2::Ptr cloud_condditionfilted_PC2(new TypePC2);
        TypePC2::Ptr cloud_voxelfilted(new TypePC2);
        TypePC2::Ptr cloud_outlierfilted(new TypePC2);

    
    //条件滤波
        pcl::ConditionAnd<pcl::PointXYZ>::Ptr range_cond(new pcl::ConditionAnd<pcl::PointXYZ>());
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("z",pcl::ComparisonOps::GT, 0.45)));
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("z",pcl::ComparisonOps::LT, 0.70)));
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("x",pcl::ComparisonOps::GT,-0.25)));
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("x",pcl::ComparisonOps::LT,0.25)));
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("y",pcl::ComparisonOps::GT,-0.1)));
        range_cond->addComparison(pcl::FieldComparison<pcl::PointXYZ>::ConstPtr(new pcl::FieldComparison<pcl::PointXYZ>("y",pcl::ComparisonOps::LT,0.2)));
        pcl::ConditionalRemoval<pcl::PointXYZ> removal;
        removal.setCondition(range_cond);
        removal.setInputCloud(temp_cloud);
        removal.setKeepOrganized(true);
        removal.filter(*cloud_condditionfilted);

        pcl::toPCLPointCloud2(*cloud_condditionfilted,*cloud_condditionfilted_PC2);

    //进行体素滤波
        //滤波器
        pcl::VoxelGrid<TypePC2> sor;
        sor.setInputCloud(cloud_condditionfilted_PC2);//输入
        sor.setLeafSize(0.01f, 0.01f, 0.01f);//0.01f表示体素为1cm立方体
        sor.filter(*cloud_voxelfilted);

    //去离群点
        pcl::StatisticalOutlierRemoval<TypePC2> outlier_remover;
        outlier_remover.setInputCloud(cloud_voxelfilted);
        outlier_remover.setMeanK(60);
        outlier_remover.setStddevMulThresh(1.5);
        outlier_remover.filter(*cloud_outlierfilted);


    // //保存为PCD文件
    // pcl::io::savePCDFile("cloud_filted.pcd",*cloud_outlierfilted);

    // ROS_INFO("Saved");
    
    //转为sensor_msgs格式发布
    ros::Rate rate(1);
    pcl::fromPCLPointCloud2(*cloud_outlierfilted,*temp_cloud);
    // temp_cloud.header.frame_id = "filtered";
    // temp_cloud.is_dense = false;
    pcl::toROSMsg(*temp_cloud,filted_cloud);
    filted_cloud.header.frame_id = "filted";
    filted_cloud.is_dense = false;
    pub.publish(filted_cloud);
    // ROS_INFO("正在发布...");
    rate.sleep();
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc,argv,"read_in_filter_node");
    talker_listener tl;
    ros::spin();

    return 0;
}



