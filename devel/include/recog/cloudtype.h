//basic
#include "ros/ros.h"
#include "pcl/point_types.h"
#include "pcl_ros/point_cloud.h"
#include "pcl_conversions/pcl_conversions.h"
#include "pcl/io/pcd_io.h"
#include "recog/Position.h"
#include "pcl/console/parse.h"
#include "pcl/common/transforms.h"

//segmentation
#include "pcl/ModelCoefficients.h"
#include "pcl/sample_consensus/method_types.h"
#include "pcl/sample_consensus/model_types.h"
#include "pcl/segmentation/sac_segmentation.h"

//filters
#include "pcl/filters/voxel_grid.h"
#include "pcl/filters/passthrough.h"
#include "pcl/filters/statistical_outlier_removal.h"
#include "pcl/filters/conditional_removal.h"

//solve
#include "recog/solve.h"
#include "pcl/range_image/range_image.h"
#include "pcl/features/range_image_border_extractor.h"
#include "pcl/keypoints/narf_keypoint.h"
#include "pcl/filters/extract_indices.h"

//Serial
#include "recog/serial_port.h"

//recognition
// #include "pcl/features/shot_omp.h"
// #include "pcl/correspondence.h"
// #include "pcl/features/normal_3d.h"
// #include "pcl/features/board.h"
// #include "pcl/filters/uniform_sampling.h"
// #include "pcl/recognition/cg/hough_3d.h"
// #include "pcl/recognition/cg/geometric_consistency.h"
// #include "pcl/kdtree/kdtree_flann.h"
// #include "pcl/kdtree/impl/kdtree_flann.hpp"
// #include "pcl/common/transforms.h"
// #include "pcl/console/parse.h"
// #include "pcl/visualization/pcl_visualizer.h"
// #include "pcl/visualization/cloud_viewer.h"

typedef pcl::PointCloud<pcl::PointXYZ> TypeXYZ;
typedef pcl::PCLPointCloud2 TypePC2;

//平面聚类分割
TypeXYZ Plane_segmentation(const TypeXYZ::ConstPtr &input_cloud);

//去离群点
TypeXYZ outlier_fileter(const TypeXYZ::ConstPtr &input_cloud);

//Transform
TypeXYZ transform(const TypeXYZ::ConstPtr &input_cloud);

TypeXYZ Keypoint_Extraction(const TypeXYZ::ConstPtr &input_cloud);

pcl::PointXYZ cal_center(std::vector<pcl::PointXYZ> & keypoints);

//目标类
class target{
    public:
        float theta;
        float alpha;
        float distance;
        target();
        void solve(const TypeXYZ::ConstPtr &input_cloud);
        void angle_solve(const pcl::PointXYZ & center);
};