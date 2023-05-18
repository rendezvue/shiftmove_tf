#include <ros/ros.h>
#include <tf2/LinearMath/Vector3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
int main(int argc, char** argv)
{
    ros::init(argc, argv, "tfpose_navi");
    ros::NodeHandle n;
    ros::NodeHandle nhp("~");
    ros::Rate r(50);
    tf2_ros::Buffer tfBuffer;
    tf2_ros::TransformListener odomListener(tfBuffer);

    while(ros::ok()){
        static tf::TransformBroadcaster odom_broadcaster;
        tf::Transform new_tf;
        new_tf.setOrigin( tf::Vector3(1,0,0) );
        new_tf.setRotation( tf::Quaternion(0,0,0,1) );
        odom_broadcaster.sendTransform(tf::StampedTransform(new_tf, ros::Time::now(), "base_footprint", "tfpose_navi"));

        geometry_msgs::TransformStamped transform;
        try{
            transform = tfBuffer.lookupTransform("map", "tfpose_navi", ros::Time(0));
        }
        catch (tf2::TransformException &ex){
            ROS_ERROR("%s", ex.what());
            ros::Duration(0.1).sleep();
            continue;
        }
        std::cout << transform << std::endl;

        break;

    }
    return 0;
}