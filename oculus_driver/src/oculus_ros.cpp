/**
 * Copyright (c) 2013, Takashi Ogura
 * All rights reserved.
 *
 * Modified by Lars Harmsen
 */
 
#include <oculus_driver/oculus_ros.h>
#include <oculus_driver/util.h>
#include <oculus_msgs/HMDInfo.h>
#include <iostream>

namespace oculus_driver
{

    OculusRos::OculusRos(ros::NodeHandle& node)
    : is_info_loaded(false)
    , parent_frame("parent")
    , oculus_frame("oculus")
    , node(node)
    {
        ROS_INFO("Oculus Rift Object Created");
    }

    bool OculusRos::init()
    {
        OVR::System::Init();

        ROS_INFO("Oculus Rift System Starting");

        ros::NodeHandle private_node("~");
        private_node.getParam("parent_frame", parent_frame);
        private_node.getParam("oculus_frame", oculus_frame);

        manager = *OVR::DeviceManager::Create();
        hmd = *manager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
        fusion_result = new OVR::SensorFusion();
        ROS_INFO("Oculus Rift Device Manager Running");

        if (hmd)
        {
            is_info_loaded = hmd->GetDeviceInfo(&info);
            sensor = *hmd->GetSensor();
            hmd_pub = node.advertise<oculus_msgs::HMDInfo>("/oculus/hmdinfo", 10);
        }
        else
        {
            sensor = *manager->EnumerateDevices<OVR::SensorDevice>().CreateDevice();
        }

        if (sensor)
        {
            fusion_result -> AttachToSensor(sensor);
            pub = node.advertise<geometry_msgs::PoseStamped>("/oculus/orientation_stamped", 1);
        }
        return is_info_loaded || sensor;
    }

    OculusRos::~OculusRos()
    {
        sensor.Clear();
        hmd.Clear();
        manager.Clear();
        OVR::System::Destroy();
    }

    void OculusRos::publish()
    {
        ros::Time now = ros::Time::now();
        if (is_info_loaded)
        {
            oculus_msgs::HMDInfo hmd_msg;
            convertHMDInfoToMsg(info, hmd_msg);
            hmd_msg.header.stamp = now;
            hmd_pub.publish(hmd_msg);
        }
        if (sensor)
        {
            // topic
            geometry_msgs::PoseStamped q_msg;
            convertQuaternionToMsg(fusion_result->GetOrientation(), q_msg.pose.orientation);
            q_msg.header.stamp = now;
            pub.publish(q_msg);

            // tf
            tf::Transform transform;
            transform.setRotation(tf::Quaternion(q_msg.pose.orientation.x,
                         q_msg.pose.orientation.y,
                         q_msg.pose.orientation.z,
                         q_msg.pose.orientation.w));
            br.sendTransform(tf::StampedTransform(transform,
                           now,
                           parent_frame,
                           oculus_frame));
        }
    }

} 	// namespace oculus_driver
