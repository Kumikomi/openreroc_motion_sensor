#include "ros/ros.h"
#include "openreroc_motion_sensor.h"
#include <stdio.h>

void chatterCallback(const openreroc_motion_sensor::openreroc_motion_sensor msg)
{
    printf("%d\n",msg.sensor_data);
}

int main(int argc, char  **argv)
{
	ros::init(argc, argv, "sample_output");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("openreroc_motion_sensor", 1000, chatterCallback);
	ros::spin();

  	return 0;
}