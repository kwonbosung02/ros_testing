#include <chrono>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/more_interfaces/defines.h"
#include "UdpPacket.h"

#include "ros/ros.h"

volatile int cnt = 0;
int sockfd;
int frequency = 4000;
std::ofstream writeFile;
std::string data;

void send_udp_packet(const udp2ros::UdpPacket::ConstPtr& msg) {
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
//    data = msg->send_data;
//    ros::Time current_time = ros::Time::now();
//    writeFile << data << " " << std::to_string(current_time.toSec()) << "\n";
    // set address and port num
    servAddr.sin_family = AF_INET;
    inet_aton(msg->ip.c_str(), (struct in_addr*)&servAddr.sin_addr.s_addr);
    servAddr.sin_port = htons(msg->port_num);

    // send msg
    if (sendto(sockfd, msg->send_data.c_str(), strlen(msg->send_data.c_str()), 0, (struct sockaddr *) &servAddr, sizeof(servAddr)) !=
        strlen(msg->send_data.c_str())) {
        ROS_INFO("Fail to send socket");
        return;
    }
    cnt++;
}

int main(int argc, char **argv) {
    ros::init(argc, argv, "udp_send_server");
    ros::NodeHandle nh;
//    writeFile.open("send.txt");

    ROS_INFO("Wait for packet...");

    // open socket
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        ROS_INFO("Fail to open socket");
        return -1;
    }

    ros::Subscriber sub = nh.subscribe("udp_send", 1000, send_udp_packet);
    ros::spin();
    return 0;
}

