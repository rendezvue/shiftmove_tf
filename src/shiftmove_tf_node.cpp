#include <ros/ros.h>
#include <tf2/LinearMath/Vector3.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <tf2_ros/transform_listener.h>
#include <geometry_msgs/TransformStamped.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Transform.h>
#include <iostream>
#include <string>

tf2_ros::Buffer tfBuffer;

geometry_msgs::Transform send_shiftmove_tf(float x, float y, std::string str_shiftmove_tf)
{
    while(ros::ok()){
        static tf::TransformBroadcaster odom_broadcaster;        
        static tf2_ros::TransformListener odomListener(tfBuffer);

        tf::Transform new_tf;
        new_tf.setOrigin( tf::Vector3(x,y,0) );
        tf2::Quaternion tf2_q;
        tf2_q.setRPY(0,0,0);
        new_tf.setRotation( tf::Quaternion( tf2_q.x(), tf2_q.y(), tf2_q.z(), tf2_q.w()) );
        odom_broadcaster.sendTransform(tf::StampedTransform(new_tf, ros::Time::now(), "base_footprint", str_shiftmove_tf));

        geometry_msgs::TransformStamped transform;
        try{
            transform = tfBuffer.lookupTransform("map", str_shiftmove_tf, ros::Time(0));
        }
        catch (tf2::TransformException &ex){
            ROS_ERROR("%s", ex.what());
            ros::Duration(0.1).sleep();
            continue;
        }
        // std::cout << transform << std::endl;
        return transform.transform;
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "shiftmove_tf");
    ros::NodeHandle node;
    ros::NodeHandle nhp("~");

    float param_front_dist;
    float param_back_dist;
    float param_left_dist;
    float param_right_dist;
    nhp.getParam("front_dist", param_front_dist);
    nhp.getParam("back_dist",  param_back_dist );
    nhp.getParam("left_dist",  param_left_dist );
    nhp.getParam("right_dist", param_right_dist);

    // fprintf(stderr,"param_front_dist = %f\n",param_front_dist);

    ros::Publisher shiftmove_tf_front = node.advertise<geometry_msgs::Transform>("shiftmove_tf/front", 10);
    ros::Publisher shiftmove_tf_back = node.advertise<geometry_msgs::Transform>("shiftmove_tf/back", 10);
    ros::Publisher shiftmove_tf_left = node.advertise<geometry_msgs::Transform>("shiftmove_tf/left", 10);
    ros::Publisher shiftmove_tf_right = node.advertise<geometry_msgs::Transform>("shiftmove_tf/right", 10);

    ros::Rate r(10);

    while(ros::ok()){
        // shiftmove_tf_front.publish(send_shiftmove_tf(2,0,"tfpose_front"));
        // shiftmove_tf_back.publish(send_shiftmove_tf(-2,0,"tfpose_back"));
        // shiftmove_tf_left.publish(send_shiftmove_tf(0,2,"tfpose_left"));
        // shiftmove_tf_right.publish(send_shiftmove_tf(0,-2,"tfpose_right"));
        shiftmove_tf_front.publish(send_shiftmove_tf(param_front_dist,0,"tfpose_front"));
        shiftmove_tf_back.publish(send_shiftmove_tf(param_back_dist,0,"tfpose_back"));
        shiftmove_tf_left.publish(send_shiftmove_tf(0,param_left_dist,"tfpose_left"));
        shiftmove_tf_right.publish(send_shiftmove_tf(0,param_right_dist,"tfpose_right"));
        r.sleep();
    }
    ros::spin();
}