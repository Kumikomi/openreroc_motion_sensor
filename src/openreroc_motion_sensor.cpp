#include "ros/ros.h"
#include "openreroc_motion_sensor.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
  int fd_32;
  int rc;
  unsigned int buf;
  fd_32 = open("/dev/xillybus_read_32", O_RDONLY);
  ros::init(argc, argv, "openreroc_motion_sensor");
  ros::NodeHandle n;
  ros::Publisher pub_openreroc_motion_sensor = n.advertise<openreroc_motion_sensor::openreroc_motion_sensor>("openreroc_motion_sensor", 1000);
  ros::Rate loop_rate(10);
  openreroc_motion_sensor::openreroc_motion_sensor msg;
  int cur=0;
  while (ros::ok())
  {
    rc = read(fd_32, &buf, sizeof(buf));
    if(cur != buf && buf != 1){
      msg.sensor_data = buf;
      // printf("%d\n",msg.sensor_data);
    }
    cur = buf;
    pub_openreroc_motion_sensor.publish(msg);
    ros::spinOnce();
    // loop_rate.sleep();
  }

  close(fd_32);
  return 0;
}
