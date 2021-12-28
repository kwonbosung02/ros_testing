#include <chrono>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "../include/more_interfaces/defines.h"

#include "ros/ros.h"
#include "UdpPacket.h"

#define BUFSIZE         (1024)

using namespace std::chrono_literals;

volatile int cnt = 0;
int frequency = 4000;

int main(int argc, char **argv) {

    ros::init(argc, argv, "udp_receive_server");
    ros::NodeHandle nh;

    size_t count_;
    int serverFd, clientFd;

    struct sockaddr_in serverAddr, clientAddr;
    char recvBuffer[BUFSIZE];
    int clientAddrSize;
    
    ssize_t receivedBytes;

    ros::Publisher pub = nh.advertise<udp2ros::UdpPacket>("udp_receive", 1000);

    // set to zero
    memset(&serverAddr, 0, sizeof(serverAddr));

    // set address and port num
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;//inet_aton(ip.c_str(), (struct in_addr*)&serverAddr.sin_addr.s_addr); // servAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(UDP_RECEIVE_PORT);

    // open socket
    if((serverFd=socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        ROS_INFO("Fail to open socket");
        return -1;
    }

    // bind info to socket
    int option = 1;
    setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    if(bind(serverFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        ROS_INFO("Fail to bind server address " , UDP_MY_IP);
        return -1;
    }

    ROS_INFO("Loop started");
    ros::Rate loop_rate(frequency);

    cnt = 0;
    while (ros::ok()) {
        // receive packet
        receivedBytes = recvfrom(serverFd, recvBuffer, BUFSIZE, 0, (struct sockaddr*)&clientAddr, (unsigned int *)&clientAddrSize);
        recvBuffer[receivedBytes] = '\0';
        //td::cout << "[UDP/" << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "] : " << recvBuffer << std::endl;
        if(receivedBytes == 0)  continue;       // if receive empty packet, discarded it.
        else if(receivedBytes < 0){
            ROS_INFO( "Comm error. return.");
            std::cout << cnt << "\n";
            return -1;
        }

        // make topic
        auto message = udp2ros::UdpPacket();
        message.send_data = std::string(recvBuffer);
        message.ip = std::string(inet_ntoa(clientAddr.sin_addr));
        message.port_num = UDP_RECEIVE_PORT;

        // comment it for realtime issue
        //ROS_INFO("[packet]\ndata: " , message.send_data , "\nc_str: " , message.send_data.c_str() ,\
        ///         "\nip: " , message.ip , "\nport: " , std::to_string(message.port_num) , "\nstrlen: " ,\
        //         std::to_string(strlen(message.send_data.c_str())), "\n");
        ROS_INFO("\n[Sending Packet]\nIP : %s, PORT : %d\nData : %s",message.ip.c_str(), message.port_num,message.send_data.c_str());

        // publish packet
        pub.publish(message);
        ros::spinOnce();
        // comment it for realtime issue
//        ROS_INFO( "Receive a packet and published it");

        loop_rate.sleep();
        cnt++;
    }
    std::cout << cnt << "\n";
    return 0;
}

