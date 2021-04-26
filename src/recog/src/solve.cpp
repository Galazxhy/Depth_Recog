#include "recog/cloudtype.h"
#include "recog/Position.h"
#include "recog/clouds.h"

#define pitch_score 10000
#define yaw_score 5000
#define distance_score 8000

class talker_listener{
    private:
        ros::NodeHandle nh;
        ros::Publisher pub;
        ros::Subscriber sub;
    public:
        talker_listener(){
            sub = nh.subscribe("seg_feature",10,&talker_listener::solve_cb,this);
            pub = nh.advertise<recog::Position>("position_msg",10,this);        
        }
        void solve_cb(const recog::clouds::ConstPtr &input_clouds);
};


//回调函数
void talker_listener::solve_cb(const recog::clouds::ConstPtr &input_clouds){
    //初始化，读取点云数组
    // ROS_INFO("received : %d",input_clouds->clouds.size());
    TypeXYZ::Ptr cloud(new TypeXYZ);
    std::vector<target> targets;
    std::vector<int> score_ls;
    int cloud_num = input_clouds->clouds.size();
    if(cloud_num != 0){
        std::vector<sensor_msgs::PointCloud2> clouds = input_clouds->clouds;

        for(int i = 0; i < cloud_num; i++){
            target temp_target;
            pcl::fromROSMsg(clouds[i],*cloud);
            temp_target.solve(cloud);
            targets.push_back(temp_target);
            score_ls.push_back(temp_target.score);
        }

        //求分数最高者索引
        std::vector<int>::iterator biggest = std::max_element(std::begin(score_ls),std::end(score_ls));
        int index = std::distance(std::begin(score_ls), biggest);
        std::printf("index = %d",index);

        recog::Position position;
        if(cloud_num == 0){
            position.pitch = 0.0f;
            position.yaw = 0.0f;
            position.distance = -1.0f;
            ROS_INFO("None Detection!");
        }
        else{
            position.pitch = targets[index].pitch;
            position.yaw = targets[index].yaw;
            position.distance = targets[index].distance;
        }
        pub.publish(position);
    }
    else
        {
            recog::Position position;
            position.pitch = 0.0f;
            position.yaw = 0.0f;
            position.distance = -1.0f;

            pub.publish(position);
        }
}


//主函数
int main(int argc, char *argv[])
{
    ros::init(argc,argv,"solve");
    talker_listener tl;
    ros::spin();

    return 0;
}

//目标
target::target(){
    pitch = 0.0f;
    yaw = 0.0f;
    distance = -1.0f;
    score = 0;
}


//解算
void target::solve(const TypeXYZ::ConstPtr &input_cloud ){
    
    TypeXYZ::Ptr Keypoints(new TypeXYZ);
    *Keypoints = Keypoint_Extraction(input_cloud);
    pcl::PointXYZ center;
    // ROS_INFO("size:%d\n",Keypoints->size());
    //容器保存关键点
    if(Keypoints->size() == 4| Keypoints->size()==5){
        std::vector<pcl::PointXYZ> keypoints_vec;
        for(const auto &Keypoint:*Keypoints)
        {
            keypoints_vec.push_back(Keypoint);
        }
        center = cal_center(keypoints_vec);
        // ROS_INFO("center:%.2f,%.2f,%.2f",center.x,center.y,center.z);
    }
    else if(Keypoints->size() > 5){
        score = 0;
        center = {0.0f, 0.0f, -1.0f};
    }
    else if(Keypoints->size() <4){
        score = 0;
        // ROS_INFO("None Detection!\n");
        center = {0.0f,0.0f,-1.0f};
    }

    angle_solve(center);
    cal_score(center);
}

//转为俯仰、偏航与距离
void target::angle_solve(const pcl::PointXYZ & center){
    if(center.z != -1.0) 
        pitch = atan(center.y/sqrt(pow(center.x,2)+pow(center.z,2)));
    else
        pitch = 0.0f;
    std::printf("solve_pitch = %f",pitch);
    if(center.z != -1.0f)
        yaw = atan(center.x/center.z);
    std::printf("solve_yaw = %f",yaw);
    distance = center.z;
    std::printf("solve_distance = %f",distance);
    std::printf("\n");
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


//计算得分
void target::cal_score(const pcl::PointXYZ &center){
    score += fabs(pitch) * pitch_score;
    score += fabs(yaw) * yaw_score;
    if(center.z != -1.0f)
        score += fabs(distance) * distance_score;
    ROS_INFO("score : %d ",score);
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
        // ROS_INFO("%.4f, %.4f, %.4f",keypoint.x,keypoint.y,keypoint.z);
    }
    //找最值
    auto max_x_pos = std::max_element(x_ls.begin(),x_ls.end());
    auto min_x_pos = std::min_element(x_ls.begin(),x_ls.end());
    // ROS_INFO("MAX_X:%.2f,MIN_X:%.2f",*max_x_pos,*min_x_pos);

    //排除误识别
    if((*max_x_pos - *min_x_pos) < 0.06 || (*max_x_pos - *min_x_pos) > 0.25){
        // ROS_INFO("X error");
        center.x = 0;
        center.y = 0;
        center.z = -1;
        return center;
    }

    auto max_y_pos = max_element(y_ls.begin(),y_ls.end());
    auto min_y_pos = min_element(y_ls.begin(),y_ls.end());
    // ROS_INFO("MAX_Y:%.2f,MIN_Y:%.2f",*max_y_pos,*min_y_pos);

    if((*max_y_pos - *min_y_pos) < 0.03 || (*max_y_pos - *min_y_pos) > 0.12){
        // ROS_INFO("y error");
        center.x = 0;
        center.y = 0;
        center.z = -1;
        return center;
    }

    auto max_z_pos = max_element(z_ls.begin(),z_ls.end());
    auto min_z_pos = min_element(z_ls.begin(),z_ls.end());
    //  ROS_INFO("MAX_Z:%.2f,MIN_Z:%.2f",*max_z_pos,*min_z_pos);

    if((*max_z_pos - *min_z_pos) > 0.15){
        // ROS_INFO("Z error");
        center.x = 0;
        center.y = 0;
        center.z = -1;
        return center;
    }

    center.x = accumulate(x_ls.begin(),x_ls.end(),0.0f)/keypoints.size();
    center.y = accumulate(y_ls.begin(),y_ls.end(),0.0f)/keypoints.size();
    center.z = accumulate(z_ls.begin(),z_ls.end(),0.0f)/keypoints.size();

    // ROS_INFO("X_center:%.2f, Y_center:%.2f, Z_center:%.2f",center.x,center.y,center.z);

    return center;
}