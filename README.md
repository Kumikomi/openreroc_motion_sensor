openreroc_motion_sensor
=======
[OpenReroc (Open Reconfigurable Robot Component)](https://github.com/Kumikomi/OpenReroc) is a project to build an open source platform of reconfigurable (i.e. FPGA) devices for robot components.  
This package supports ultra sonic sensor an FPGA board ([ZedBoard](http://zedboard.org/) Xilinx). 
*openreroc_motion_sensor* publishes sensor data to the topic.  
 
**Git**:         https://github.com/Kumikomi/openreroc_motion_sensor   
**Author**:      Kazushi Yamashina (Utsunomiya University)  
**Copyright**:   2015, Kazushi Yamashina, Utsunomiya University    
**License**:      new BSD License   
**Latest Version**: 0.1.0  

Directry tree
=======
```
openreroc_motion_sensor 
|-include/ 
|-msg/ 
|-hardware
	|-src/
    |-image/
|-src/ 
|-CMakeLists.txt 
|-package.xml  
|-LICENSE.txt
```

Requirements
======

##Platform for ROS system

- [ZedBoard](http://zedboard.org/)
- [xillinux-1.3c](http://xillybus.com/xillinux)
 - Xillinux is used to communicate between FPGA logic and ARM processor. Xillinux is a platform for Zynq that is released by Xillybus Ltd. Linux (Ubuntu) OS runs on the ARM processor. Xillinux can access to FPGA logic through a specific device file.
- ROS (hydro or groovy) please install on xillinux!
- ssh server

##Software

- ISE 14.7 (for hardware synthesis)

##Sensor

- [PING))) Ultrasonic Distance Sensor | 28015 | Parallax Inc](https://www.parallax.com/product/28015)

<img src="http://aquila.is.utsunomiya-u.ac.jp/~kazushi/sonic_sensor_img.jpeg" alt="" height="150" />
![sonic_sensor](http://aquila.is.utsunomiya-u.ac.jp/~kazushi/sonic_sensor_img.jpeg)
How to build software
=======
Please replace **catkin_ws** to your work space name.

```
cd ~/catkin_ws/src
git clone https://github.com/Kumikomi/openreroc_motion_sensor
cd ..
catkin_make 
```

Test Run
======= 
1. [Xillinux installation](http://xillybus.com/downloads/doc/xillybus_getting_started_zynq.pdf)
 - http://xillybus.com/xillinux

2. Hardware bitstream installation
Please replace **xillydemo.bit** on the SD card with `openreroc_pwm/hardware/image/openreroc_motion_sensor.bit`

3. Insert SD card & boot system

4. Run sample nodes 

**terminal 1**
```
cd ~/catkin_ws/
source devel/setup.bash
roscore &
rosrun openreroc_motion_sensor sample_output
```

**terminal 2**
```
cd ~/catkin_ws/
source devel/setup.bash
rosrun openreroc_motion_sensor openreroc_motion_sensor
```

How to build hardware
====== 
It's too complex to describe all the necessary procedure to build hardware, so some hints are shown below.
1 : Place of Soruce code `hardware/src`  
2 : Pin assignment: add the code below to **xillydemo.ucf**  

```verilog
NET  led[0] 	LOC = V22 | IOSTANDARD = LVCMOS33;	# LD4
NET  led[1] 	LOC = W22 | IOSTANDARD = LVCMOS33;	# LD5
NET  led[2] 	LOC = U19 | IOSTANDARD = LVCMOS33;	# LD6
NET  led[3] 	LOC = U14 | IOSTANDARD = LVCMOS33;	# LD7

NET PS_GPIO[7] LOC = K18 | IOSTANDARD = LVCMOS33;  # "FMC-LA05_N"
NET PS_GPIO[8] LOC = J18 | IOSTANDARD = LVCMOS33;  # "FMC-LA05_P"
NET PS_GPIO[9] LOC = L22 | IOSTANDARD = LVCMOS33;  # "FMC-LA06_N"
NET PS_GPIO[10] LOC = L21 | IOSTANDARD = LVCMOS33;  # "FMC-LA06_P"
```

3 : Add the ports to Top module **xillydemo.v**

```verilog
inout sig,
output [3:0] led
```

4 : Add the FIFO connection to Top module **xillydemo.v**

```verilog
// 32-bit loopback
//   fifo_32x512 fifo_32
//     (
//      .clk(bus_clk),
//      .srst(!user_w_write_32_open && !user_r_read_32_open),
//      .din(user_w_write_32_data),
//      .wr_en(user_w_write_32_wren),
//      .rd_en(user_r_read_32_rden),
//      .dout(user_r_read_32_data),
//      .full(user_w_write_32_full),
//      .empty(user_r_read_32_empty)
//      );

sensor_ctl sensor_ctl
(
	.clk(bus_clk),
	.rst_32(!user_w_write_32_open && !user_r_read_32_open),
	.din_32(user_w_write_32_data),
	.wr_en_32(user_w_write_32_wren),
	.rd_en_32(user_r_read_32_rden),
	.dout_32(user_r_read_32_data),
	.full_32(user_w_write_32_full),
	.empty_32(user_r_read_32_empty),
	.sig(sig),
	.led(led)
);
```

