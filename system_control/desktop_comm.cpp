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
    pthread_mutex_lock(&Global.ball_info_mutex);

    /* fill raw data */
    uint16_t raw_data[COMM_DATA_PACKET_SIZE];
    raw_data[0] = Global.ball_position_x;
    raw_data[1] = Global.ball_position_y;
    raw_data[2] = Global.arm_snt_position;
    raw_data[3] = Global.arm_gk_position;
    raw_data[4] = Global.human_score;
    raw_data[5] = Global.robot_score;

    pthread_mutex_unlock(&Global.ball_info_mutex);

    int nbytes = sendto(Global.multicast_socket_fd, &raw_data, sizeof(raw_data), 0, (struct sockaddr*)(&(Global.multicast_addr)), sizeof(Global.multicast_addr));
    if(nbytes < 0){
        perror("Multicast paket hata : ");
    }
}

int start_multicast_stream()
{
    pthread_mutex_lock(&Global.multicast_stream_state_mutex);
    if(STATE_STOPPED != Global.multicast_stream_state){
        pthread_mutex_unlock(&Global.multicast_stream_state_mutex);
        return -1;
    }

    init_multicast_connection();
    pthread_create(&Global.multicast_stream_thread, NULL, &multicast_stream_thread_func, NULL);
    pthread_mutex_unlock(&Global.multicast_stream_state_mutex);
    return 0;
}

int stop_multicast_stream()
{
    pthread_mutex_lock(&Global.multicast_stream_state_mutex);
    if(STATE_PLAYING == Global.multicast_stream_state)
        Global.multicast_stream_state = STATE_STOP_REQUESTED;
    pthread_mutex_unlock(&Global.multicast_stream_state_mutex);

    return 0;
}

void* multicast_stream_thread_func(void* arg)
{
    pthread_mutex_lock(&Global.multicast_stream_state_mutex);
    if(STATE_STOPPED == Global.multicast_stream_state)
        Global.multicast_stream_state = STATE_PLAYING;
    pthread_mutex_unlock(&Global.multicast_stream_state_mutex);

    printf("Multicast stream thread started\n");
    while(true)
    {
        /* send game data */
        send_game_data();
        usleep(DESKTOP_COMM_FREQUENCY);

        pthread_mutex_lock(&Global.multicast_stream_state_mutex);
        int state = Global.multicast_stream_state;
        pthread_mutex_unlock(&Global.multicast_stream_state_mutex);

        /* Check stream state */
        if(STATE_STOP_REQUESTED == state){
            break;
        }
    }

    pthread_mutex_lock(&Global.multicast_stream_state_mutex);
    Global.multicast_stream_state = STATE_STOPPED;

    // Close socket
    close(Global.multicast_socket_fd);
    Global.multicast_socket_fd = -1;
    pthread_mutex_unlock(&Global.multicast_stream_state_mutex);

    printf("Multicast stream thread stopped\n");
}
