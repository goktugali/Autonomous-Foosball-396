#include "desktop_comm.hpp"

void init_multicast_connection()
{
    Global.multicast_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(Global.multicast_socket_fd < 0){
        perror("Multicast socket acilamadi :");
        exit(EXIT_FAILURE);
    }

    bzero(&Global.multicast_addr, sizeof(Global.multicast_addr));
    Global.multicast_addr.sin_family = AF_INET;
    Global.multicast_addr.sin_addr.s_addr = inet_addr(MULTICAST_GROUP_IP);
    Global.multicast_addr.sin_port = htons(MULTICAST_PORT);
}

void send_game_data()
{
    /* fill raw data */
    uint16_t raw_data[COMM_DATA_PACKET_SIZE];
    raw_data[0] = Global.ball_position_x;
    raw_data[1] = Global.ball_position_y;
    raw_data[2] = Global.arm_robot_snt_position;
    raw_data[3] = Global.arm_robot_gk_position;
    raw_data[4] = Global.arm_human_snt_position;
    raw_data[5] = Global.arm_human_gk_position;
    raw_data[6] = Global.human_score;
    raw_data[7] = Global.robot_score;

    int nbytes = sendto(Global.multicast_socket_fd, &raw_data, sizeof(raw_data), 0, (struct sockaddr*)(&(Global.multicast_addr)), sizeof(Global.multicast_addr));
    if(nbytes < 0){
        perror("Multicast paket hata : ");
    }
}

int stop_multicast_stream()
{
    close(Global.multicast_socket_fd);
    Global.multicast_socket_fd = -1;
    return 0;
}