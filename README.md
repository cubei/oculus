ROS node for Oculus Rift
=========================
This is [ROS](http://ros.org) driver for [Oculus Rift](http://www.oculusvr.com).
see http://developer.oculusvr.com for Oculus Rift SDK.

Fork of the oculus repository of Takashi Ogura

Copyright (c) 2013, Takashi Ogura

Packages
------------------
* oculus_driver: oculus rift HMD ROS driver.
* oculus_msgs: oculus HMD message definitions.

Nodes
=============

oculus_node (in oculus_driver package)
------------------
publishes sensor data and HMD information of Oculus Rift.

### publish

* /oculus/orientation (geometry_msgs/PoseStamped) orientation of sensor.
* /oculus/hmd_info (oculus_ros/HMDInfo) HMD device info.

### param
* ~frequency (double: default 120.0) [Hz] rate of publish

License
-----------
BSD
