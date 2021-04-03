ROBOMASTER   工程基于深度相机+ROS+PCL矿石识别
================================
相机型号
--------
    奥比中光astra mini
使用说明
--------
>1.安装ros(kinetic/melodic)
>>安装完整版的ros自带了PCL库
>2.下载项目
>>下载并解压至任意项目中，将.sh文件及.service文件中/home/galazxhy替换为运行用户名（开机自启需用）
>终端中catkin_make
>3.配置头文件及相机驱动
>>(1)将recog文件夹复制至/devel/include文件夹下<br>
>>(2)配置相机驱动（详见ros::astra_camera官网说明http://wiki.ros.org/astra_camera）
>>>终端安装相机环境：
>>>sudo apt install ros-*-rgbd-launch ros-*-libuvc ros-*-libuvc-camera ros-*-libuvc-ros
>>>由于本项目中已包括了astra_camera功能包，故只需在工作空间中source bash文件，接着roscd进入功能包运行scripts/create_udev_rules即可
>>>命令：
>>>* cd */Depth_Recog
>>>* source ./devel/setup.bash
>>>* roscd astra_camera
>>>* ./scripts/create_udev_rules 
              
