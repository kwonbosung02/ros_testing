#include <chrono>
#include <memory>
#include <time.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/more_interfaces/defines.h"

#include "ros/ros.h"
#include "UdpPacket.h"

using namespace std::chrono_literals;
using namespace std;

volatile int dummy;
ofstream writeFile;
int frequency = 4000;

void udp_receive_packet(const udp2ros::UdpPacket::ConstPtr& msg) {
	int port_num = msg->port_num;
	std::string ip = msg->ip;
	std::string data = msg->send_data;

    //ROS_INFO("\n[Received]\ndata: " ,data , "\nip: " ,ip , "\nport: " , std::to_string(port_num));
	ROS_INFO("\n[Received packet]\n IP : %s, PORT : %d\n Data : %s",ip.c_str(), port_num, data.c_str());
	// compare the packet time and receive time
    ros::Time current_time = ros::Time::now();
    writeFile << data << " " << std::to_string(current_time.toSec()) << "\n";
	dummy++;
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "udp_send_receive_server");
	ros::NodeHandle n;

	dummy = 0;
    writeFile.open("log.txt");

    ros::Rate loop_rate(frequency);
  	ros::Publisher pub = n.advertise<udp2ros::UdpPacket>("udp_send", 1000);
 	ros::Subscriber sub = n.subscribe("udp_receive", 1000, udp_receive_packet);

	while (ros::ok()) {
        ros::Time current_time = ros::Time::now();

    	auto message = udp2ros::UdpPacket();
    	message.send_data = std::to_string(current_time.toSec());
		message.ip = UDP_SERVER_IP;
		message.port_num = UDP_SEND_PORT;

//    std::cout << "Udp send request '" + message.send_data + "' to " << message.ip << "\n";
		//ROS_INFO("\n[Publishing data]\nIP : %s, PORT : %d\nData : %s",message.ip.c_str(),message.port_num,message.send_data.c_str());

        pub.publish(message);
		dummy--;

		ros::spinOnce();
		loop_rate.sleep();

	}

}
