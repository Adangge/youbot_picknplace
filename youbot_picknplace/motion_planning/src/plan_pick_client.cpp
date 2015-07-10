#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <actionlib/client/terminal_state.h>
#include <motion_planning_msgs/PlanPickAction.h>
#include <motion_msgs/GripperPose.h>

//tf
#include <tf/transform_datatypes.h>

int main (int argc, char **argv) {
  ros::init(argc, argv, "test_planpick");
  ros::NodeHandle n;

  if ( argc != 4)
    return 0;

  // create the action client
  // true causes the client to spin its own thread
  actionlib::SimpleActionClient<motion_planning_msgs::PlanPickAction> ac("motion_planning/plan_pick", true);
  ros::ServiceClient pose_c = n.serviceClient<motion_msgs::GripperPose>("motion/gripper_pose");

  ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  ac.waitForServer(); //will wait for infinite time

  ROS_INFO("Action server started, sending goal.");
  // send a goal to the action
  motion_planning_msgs::PlanPickGoal goal;
  geometry_msgs::PoseStamped target_pose;
  target_pose.header.frame_id = "base_footprint";
  // geometry_msgs::Quaternion quat;
  // quat = tf::createQuaternionMsgFromRollPitchYaw(-3.129, 0.0549, 1.686);
  // target_pose.pose.orientation.x = quat.x;
  // target_pose.pose.orientation.y = quat.y;
  // target_pose.pose.orientation.z = quat.z;
  // target_pose.pose.orientation.w = quat.w;
  // ROS_INFO("Quaternion info- x: %f  y: %f  z: %f  w: %f", quat.x, quat.y, quat.z, quat.w);
  target_pose.pose.position.x = atof(argv[1]);
  target_pose.pose.position.y = atof(argv[2]);
  target_pose.pose.position.z = atof(argv[3]);

  motion_msgs::GripperPose srv;
  srv.request.position = target_pose.pose.position;
  if (pose_c.call(srv)) {
    ROS_INFO("Received orientation from GripperPose");
    target_pose.pose.orientation = srv.response.orientation;
  } else {
    ROS_ERROR("Failed to call service GripperPose");
    return 1;
  }


  goal.object_pose = target_pose;
  ac.sendGoal(goal);

  //exit
  return 0;
}
