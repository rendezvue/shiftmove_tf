#!/usr/bin/env python
# license removed for brevity
import time
import rospy
import actionlib
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal
from geometry_msgs.msg import Transform

global pos_data
pos_data = Transform
def pos_callback(data):
    global pos_data
    pos_data = data

def movebase_client():
    time.sleep(0.5)
    client = actionlib.SimpleActionClient('move_base',MoveBaseAction)
    client.wait_for_server()

    goal = MoveBaseGoal()
    goal.target_pose.header.frame_id = "map"
    goal.target_pose.header.stamp = rospy.Time.now()
    
    
    global pos_data
    print(pos_data)
    goal.target_pose.pose.position.x = pos_data.translation.x
    goal.target_pose.pose.position.y = pos_data.translation.y
    goal.target_pose.pose.position.z = pos_data.translation.z
    
    goal.target_pose.pose.orientation.x = pos_data.rotation.x
    goal.target_pose.pose.orientation.y = pos_data.rotation.y
    goal.target_pose.pose.orientation.z = pos_data.rotation.z
    goal.target_pose.pose.orientation.w = pos_data.rotation.w

    client.send_goal(goal)
    wait = client.wait_for_result()
    if not wait:
        rospy.logerr("Action server not available!")
        rospy.signal_shutdown("Action server not available!")
    else:
        return client.get_result()

if __name__ == '__main__':
    try:
        rospy.init_node('movebase_client_py')
        pos_sub = rospy.Subscriber('/shiftmove_tf/right', Transform, pos_callback)
        result = movebase_client()
        if result:
            rospy.loginfo("Goal execution done!")
    except rospy.ROSInterruptException:
        rospy.loginfo("Navigation test finished.")
