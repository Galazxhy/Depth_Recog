ROBOMASTER   工程基于深度相机+ROS+PCL矿石识别
================================
相机型号
--------
    奥比中光astra mini
使用说明
--------
>1.安装ros(kinetic/melodic)
>>安装完整版的ros自带PCL库

>2.下载项目
>>下载并解压至任意项目中，将.sh文件及.service文件中/home/galazxhy替换为运行用户名（开机自启需用）<br>
>>终端中catkin_make

>3.配置头文件及相机驱动
>>(1)将recog文件夹复制至/devel/include文件夹下<br>
>>(2)配置相机驱动（详见ros::astra_camera官网说明http://wiki.ros.org/astra_camera）<br>
>>>终端安装相机环境：<br>
>>>sudo apt install ros-*-rgbd-launch ros-*-libuvc ros-*-libuvc-camera ros-*-libuvc-ros
>>>由于本项目中已包括了astra_camera功能包，故只需在工作空间中source bash文件，接着roscd进入功能包运行scripts/create_udev_rules即可<br>
>>>命令：
>>>* cd */Depth_Recog
>>>* source ./devel/setup.bash
>>>* roscd astra_camera
>>>* ./scripts/create_udev_rules 
              
>4.链接库问题
>>对于可能会出现的找不到链接库的问题(妙算上大概率出现),可以通过新建软链接的方式解决<br>
>>>示例(编译提示缺少libx.so，但设备上有libx.so.1)：<br>
>>>* sudo ln -s lib.so.1 libx.so<br>
>>新建了一个名为libx.so的指向libx.so.1的链接，即可解决问题
