#include "recog/cloudtype.h"
#include "recog/Position.h"

class talker_listener{
    private:
        ros::NodeHandle nh;
        ros::Publisher pub;
        ros::Subscriber sub;
    public:
        talker_listener(){
            sub = nh.subscribe("feature",10,&talker_listener::solve_cb,this);
            pub = nh.advertise<recog::Position>("position_msg",10,this);        
        }
        void solve_cb(const sensor_msgs::PointCloud2::ConstPtr &input_cloud);
};



void talker_listener::solve_cb(const sensor_msgs::PointCloud2::ConstPtr &input_cloud){
    TypeXYZ::Ptr cloud(new TypeXYZ);
    pcl::fromROSMsg(*input_cloud,*cloud);
    
    target target;
    target.solve(cloud);

    recog::Position position;
    position.theta = target.theta;
    position.alpha = target.alpha;
    position.distance = target.distance;
    
    pub.publish(position);
}

int main(int argc, char *argv[])
{
    ros::init(argc,argv,"solve");
    talker_listener tl;
    ros::spin();

    return 0;
}

target::target(){
    theta = 0.0f;
    alpha = 0.0f;
    distance = 0.0f;
}


void target::solve(const TypeXYZ::ConstPtr &input_cloud ){
    
    TypeXYZ::Ptr Keypoints(new TypeXYZ);
    *Keypoints = Keypoint_Extraction(input_cloud);
    pcl::PointXYZ center;
    ROS_INFO("size:%d\n",Keypoints->size());
    //容器保存关键点
    if(Keypoints->size() == 4| Keypoints->size()==5){
        std::vector<pcl::PointXYZ> keypoints_vec;
        for(const auto &Keypoint:*Keypoints)
        {
            keypoints_vec.push_back(Keypoint);
        }
        center = cal_center(keypoints_vec);
        ROS_INFO("center:%.2f,%.2f,%.2f",center.x,center.y,center.z);
    }
    else if(Keypoints->size() > 5){
        //多个目标
        center = {0, 0, 0};
    }
    else if(Keypoints->size() <4){
        ROS_INFO("None Detection!\n");
        center = {0,0,0};
    }
    angle_solve(center);
}

//转为俯仰、偏航与距离
void target::angle_solve(const pcl::PointXYZ & center){
    theta = atan(center.z/sqrt(pow(2,center.x)+pow(2,center.y)));
    alpha = atan(abs(center.x)/center.z);
    distance = center.z;
}

//关键点提取，返回关键点索引（角点）
TypeXYZ Keypoint_Extraction(const TypeXYZ::ConstPtr &input_cloud){
    //生成深度图像素
    //参数设置
    float angularResolution = (float) (  1.2f* (M_PI/180.0f));  //   角分辨率0.1度
    float maxAngleWidth     = (float) (360.0f * (M_PI/180.0f));  // 视宽360度
    float maxAngleHeight    = (float) (180.0f * (M_PI/180.0f));  // 视高180度
    Eigen::Affine3f sensorPose = (Eigen::Affine3f)Eigen::Translation3f(0.0f, 0.0f, 0.0f); //摄像头位姿
    pcl::RangeImage::CoordinateFrame coordinate_frame = pcl::RangeImage::CAMERA_FRAME; //坐标系
    float noiseLevel=0.00; //可将范围内点设为同一深度值
    float minRange = 0.0f; //可将近于该值的点排除
    int borderSize = 1; //边界宽度
    float support_size = 0.12f;

    
    //创建深度图
    pcl::RangeImage rangeImage;
    rangeImage.createFromPointCloud(*input_cloud, angularResolution, maxAngleWidth, maxAngleHeight, sensorPose, coordinate_frame, noiseLevel, minRange, borderSize);
    rangeImage.setUnseenToMaxRange();
    // ROS_INFO("rangeImagesize:%d",rangeImage.size());

    TypeXYZ::Ptr keypoints_ptr(new TypeXYZ);
    TypeXYZ& keypoints = *keypoints_ptr;

    pcl::RangeImageBorderExtractor range_image_border_extractor;

    pcl::NarfKeypoint narf_keypoint_detector(&range_image_border_extractor);
    narf_keypoint_detector.setRangeImage(&rangeImage);
    narf_keypoint_detector.getParameters().support_size = support_size;
    // narf_keypoint_detector.getParameters ().add_points_on_straight_edges = true;
    // narf_keypoint_detector.getParameters ().distance_for_additional_points = 0.1;

    pcl::PointCloud<int> Keypoint_indices;
    narf_keypoint_detector.compute(Keypoint_indices);

    keypoints.resize(Keypoint_indices.size());
    // ROS_INFO("size:%d",Keypoint_indices.size());
    for (std::size_t i = 0; i < Keypoint_indices.size(); ++i)
    {
        keypoints[i].getVector3fMap() = rangeImage[Keypoint_indices[i]].getVector3fMap();
    }

    return keypoints;
}

//计算中心
pcl::PointXYZ cal_center(std::vector<pcl::PointXYZ> & keypoints){
    pcl::PointXYZ center;

    std::vector<float> x_ls;
    std::vector<float> y_ls;
    std::vector<float> z_ls;

    for(const auto &keypoint:keypoints){
        x_ls.push_back(keypoint.x);
        y_ls.push_back(keypoint.y);
        z_ls.push_back(keypoint.z);
        ROS_INFO("%.4f, %.4f, %.4f",keypoint.x,keypoint.y,keypoint.z);
    }
    //找最值
    auto max_x_pos = std::max_element(x_ls.begin(),x_ls.end());
    auto min_x_pos = std::min_element(x_ls.begin(),x_ls.end());
    ROS_INFO("MAX_X:%.2f,MIN_X:%.2f",*max_x_pos,*min_x_pos);

    //排除误识别
    if((*max_x_pos - *min_x_pos) < 0.08 || (*max_x_pos - *min_x_pos) > 0.25){
        ROS_INFO("X error");
        center.x = 0;
        center.y = 0;
        center.z = 0;
        return center;
    }

    auto max_y_pos = max_element(y_ls.begin(),y_ls.end());
    auto min_y_pos = min_element(y_ls.begin(),y_ls.end());
    ROS_INFO("MAX_Y:%.2f,MIN_Y:%.2f",*max_y_pos,*min_y_pos);

    if((*max_y_pos - *min_y_pos) < 0.03 || (*max_y_pos - *min_y_pos) > 0.1){
        ROS_INFO("y error");
        center.x = 0;
        center.y = 0;
        center.z = 0;
        return center;
    }

    auto max_z_pos = max_element(z_ls.begin(),z_ls.end());
    auto min_z_pos = min_element(z_ls.begin(),z_ls.end());
     ROS_INFO("MAX_Z:%.2f,MIN_Z:%.2f",*max_z_pos,*min_z_pos);

    if((*max_z_pos - *min_z_pos) > 0.15){
        ROS_INFO("Z error");
        center.x = 0;
        center.y = 0;
        center.z = 0;
        return center;
    }

    center.x = accumulate(x_ls.begin(),x_ls.end(),0.0f)/keypoints.size();
    center.y = accumulate(y_ls.begin(),y_ls.end(),0.0f)/keypoints.size();
    center.z = accumulate(z_ls.begin(),z_ls.end(),0.0f)/keypoints.size();

    ROS_INFO("X_center:%.2f, Y_center:%.2f, Z_center:%.2f",center.x,center.y,center.z);

    return center;
}