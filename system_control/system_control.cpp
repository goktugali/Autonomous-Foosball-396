#include "system_control.hpp"

int start_game_thread()
{
    pthread_mutex_lock(&Global.game_state_mutex);
    if(Global.game_play_state != STATE_STOPPED)
    {
        pthread_mutex_unlock(&Global.game_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.game_state_mutex);

    pthread_mutex_lock(&Global.multicast_stream_state_mutex);
    if(STATE_STOPPED != Global.multicast_stream_state)
    {
        pthread_mutex_unlock(&Global.multicast_stream_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.multicast_stream_state_mutex);

    pthread_mutex_lock(&Global.servo_kicker_state_mutex);
    if(STATE_STOPPED != Global.servo_kicker_state)
    {
        pthread_mutex_unlock(&Global.servo_kicker_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.servo_kicker_state_mutex);

    pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
    if(STATE_STOPPED != Global.ball_warning_thread_state)
    {
        pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);

    start_multicast_stream();
    start_servo_kicker_thread();
    start_ball_tracker_warning_thread();
    pthread_create(&Global.game_thread, NULL, &game_thread_func, NULL);

    return 0;
}
int stop_game_thread()
{
    pthread_mutex_lock(&Global.game_state_mutex);
    if(STATE_PLAYING != Global.game_play_state)
    {
        pthread_mutex_unlock(&Global.game_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.game_state_mutex);

    pthread_mutex_lock(&Global.multicast_stream_state_mutex);
    if(STATE_PLAYING != Global.multicast_stream_state)
    {
        pthread_mutex_unlock(&Global.multicast_stream_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.multicast_stream_state_mutex);

    pthread_mutex_lock(&Global.servo_kicker_state_mutex);
    if(STATE_PLAYING != Global.servo_kicker_state)
    {
        pthread_mutex_unlock(&Global.servo_kicker_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.servo_kicker_state_mutex);

    pthread_mutex_lock(&Global.ball_warning_thread_state_mutex);
    if(STATE_PLAYING != Global.ball_warning_thread_state)
    {
        pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);
        return -1;
    }
    pthread_mutex_unlock(&Global.ball_warning_thread_state_mutex);

    stop_multicast_stream();
    stop_servo_kicker_thread();
    stop_ball_tracker_warning_thread();

    pthread_mutex_lock(&Global.game_state_mutex);
    Global.game_play_state = STATE_STOP_REQUESTED;
    pthread_mutex_unlock(&Global.game_state_mutex);
    return 0;
}

void init_main_server_connection()
{
    Global.main_server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == Global.main_server_socket_fd){
        perror("Main server socket acilamadi.");
        exit(EXIT_FAILURE);
    }

    // assign IP, PORT
    bzero(&Global.main_server_addr, sizeof(Global.main_server_addr));
    Global.main_server_addr.sin_family 		= AF_INET;
    Global.main_server_addr.sin_addr.s_addr 	= htonl(INADDR_ANY);
    Global.main_server_addr.sin_port 			= htons(MAIN_SERVER_PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(Global.main_server_socket_fd, (struct sockaddr*)&Global.main_server_addr, sizeof(Global.main_server_addr))) != 0) {
        perror("socket bind failed...");
        exit(EXIT_FAILURE);
    }

    if ((listen(Global.main_server_socket_fd, 5)) != 0) {
        perror("Listen failed...");
        exit(EXIT_FAILURE);
    }
}
void start_main_server_thread()
{
    init_main_server_connection();
    pthread_create(&Global.main_server_thread, NULL, &main_server_thread_func, NULL);
}

void* main_server_thread_func(void* arg)
{
    char message_buffer[64];
    struct sockaddr client;
    memset(&client, 0, sizeof(client));
    socklen_t len = sizeof(client);

    const int response_second = 3;

    while(true)
    {
        // accept desktop client
        int client_fd = accept(Global.main_server_socket_fd, (struct sockaddr*)&client, &len);

        // read request message
        memset(message_buffer, 0, sizeof(message_buffer));
        int readed_byte = read(client_fd, &message_buffer, 64);

        if(-1 != readed_byte)
        {
            // null terminate the request string
            message_buffer[readed_byte] = '\0';

            if(0 == strcmp(message_buffer, CMD_START_GAME)) // START GAME COMMAND
            {
                printf("Game start requested\n");
                start_game_thread();
                sleep(response_second);

                char response_msg[64];
                strcpy(response_msg, RESPONSE_OK);
                write(client_fd, response_msg, strlen(response_msg));
            }

            else if(0 == strcmp(message_buffer, CMD_STOP_GAME)) // STOP GAME COMMAND
            {
                printf("Game stop requested\n");
                stop_game_thread();
                sleep(response_second);

                char response_msg[64];
                strcpy(response_msg, RESPONSE_OK);
                write(client_fd, response_msg, strlen(response_msg));
            }

            else if(0 == strcmp(message_buffer, CMD_GET_OLD_SCORES)) // GET OLD SCORES COMMAND
            {
                printf("Old scores requested\n");

                /* Get json file content, send it to client */
                char json_db_content[4096];
                get_json_file_content(json_db_content);

                sleep(response_second);

                write(client_fd, json_db_content, strlen(json_db_content));
            }

            else
            {
                // unknown command
            }
        }
        close(client_fd);
    }
}