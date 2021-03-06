#include "ros/ros.h"
#include "MDPmanager.h"
#include "std_msgs/String.h"
#include "std_msgs/Int8.h"
#include "geometry_msgs/Pose2D.h"
#include "geometry_msgs/PointStamped.h"
#include <nav_msgs/OccupancyGrid.h>
#include <nav_msgs/Odometry.h>
#include <sstream>
#include <boost/thread/thread.hpp>
#include "srBSpline.h"

// using namespace Eigen;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "Gobal_planner");
  
  MDPManager problemmanager; 
  MapParam   mapParam;
  problemmanager.setPMapParam(&mapParam);

  // ros::Rate r(5);
  ros::Subscriber Point_sub;          //subscriber clicked point
  ros::Subscriber staticmap_sub;
  ros::Subscriber global_pos_sub;

  ros::NodeHandle n;
  
  Point_sub     = n.subscribe<geometry_msgs::PointStamped>("/clicked_point", 10, &MDPManager::ClikedpointCallback,&problemmanager);
  //staticmap_sub = n.subscribe<nav_msgs::OccupancyGrid>("/static_obstacle_map_ref", 30, &MDPManager::static_mapCallback,&problemmanager); 
  staticmap_sub = n.subscribe<nav_msgs::OccupancyGrid>("/spot/map", 30, &MDPManager::static_mapCallback,&problemmanager); 
  //global_pos_sub= n.subscribe<geometry_msgs::PoseStamped>("/global_pose", 10, &MDPManager::global_pose_callback,&problemmanager);

  ros::Rate loop_rate(20);

  while (ros::ok())
  {
     // problemmanager.MDPsolPublish();  //check,,, why tis line is required
      if(problemmanager.m_boolSolve)
      {
         printf("Begin to solve\n");
         problemmanager.pathPublish();
         problemmanager.MDPsolve(); 
         // problemmanager.printPath(); 
         problemmanager.generatePath();
        
         printf("End solve\n");
         problemmanager.m_boolSolve=false;
      }

  	 ros::spinOnce();
     loop_rate.sleep();  
  }

  ros::spin();

  return 0;
}




