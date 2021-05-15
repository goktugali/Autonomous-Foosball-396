#include "match_data_utils.hpp"

void get_json_file_content(char* json_content)
{
    int json_file_fd = open(JSON_DATABASE_FILE_PATH, O_RDONLY);
    size_t file_size = lseek(json_file_fd,0,SEEK_END);
    lseek(json_file_fd, 0, SEEK_SET);

    read(json_file_fd, json_content, file_size);
    json_content[file_size] = '\0';
    close(json_file_fd);
}

void parse_json_data(const char* json_data, match_data_t* parsed_json_data, int* entry_size)
{
    // Format --> {date, human_score, robot_score}; {date, human_score, robot_score}
    char copy_str[1024];
    int total_entry_size  = 0;

    // eliminate whitespace / tab
    size_t str_len = strlen(json_data);
    int new_str_size = 0;
    for (int i = 0; i < str_len; ++i)
        if(json_data[i] != ' ' && json_data[i] != '\t')
            copy_str[new_str_size++] = json_data[i];
    copy_str[new_str_size] = '\0';

    char* entry_token = strtok(copy_str, ";");
    while(entry_token != NULL)
    {
        entry_token[strlen(entry_token) - 1] = '\0';
        entry_token = &entry_token[1];

        // get json entry & values
        char* date          = strtok_r(entry_token, ",", &entry_token);
        char* human_score   = strtok_r(entry_token, ",", &entry_token);
        char* robot_score   = strtok_r(entry_token, ",", &entry_token);

        // store parsed values into struct
        strcpy(parsed_json_data[total_entry_size].date, date);
        strcpy(parsed_json_data[total_entry_size].human_score, human_score);
        strcpy(parsed_json_data[total_entry_size].robot_score, robot_score);

        entry_token = strtok(NULL, ";");
        ++total_entry_size;
    }

    // store total entry size
    *entry_size = total_entry_size;
}

void get_current_date_time(char* str_time)
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[24];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y-%H:%M",timeinfo);
    strcpy(str_time, buffer);
}

void add_new_match_data(const match_data_t* match_data)
{
    // create json entry
    char match_data_str[64];
    match_data_to_string(match_data, match_data_str);

    // if file contains more than 10 entries, delete oldest (first) one.
    delete_first_match_data();

    // add new match data to database file
    int database_file_fd = open(JSON_DATABASE_FILE_PATH, O_RDWR | O_FSYNC | O_APPEND | O_CREAT);
    write(database_file_fd, match_data_str, strlen(match_data_str));
    close(database_file_fd);
}

void match_data_to_string(const match_data_t* match_data, char* match_data_str)
{
    sprintf(match_data_str, R"({"%s", "%s", "%s"};)", match_data->date, match_data->human_score, match_data->robot_score);
}

void delete_first_match_data()
{
    char database_file_content[1024];
    get_json_file_content(database_file_content);
    char* content_except_first_match = database_file_content;

    match_data_t match_entries[MAX_STORED_MATCH_DATA_SIZE];
    int total_entry_size = 0;
    parse_json_data(database_file_content, match_entries, &total_entry_size);

    if(total_entry_size >= MAX_STORED_MATCH_DATA_SIZE)
    {
        strtok_r(content_except_first_match, ";", &content_except_first_match);
        // truncate file and write again.
        int database_file_fd = open(JSON_DATABASE_FILE_PATH, O_RDWR | O_FSYNC | O_TRUNC);
        write(database_file_fd, content_except_first_match, strlen(content_except_first_match));
        close(database_file_fd);
    }
}

void update_current_match_data(uint16_t human_score, uint16_t robot_score)
{
    pthread_mutex_lock(&Global.current_match_data_mutex);
    memset(Global.current_match_data.human_score, 0, sizeof(Global.current_match_data.human_score));
    memset(Global.current_match_data.robot_score, 0, sizeof(Global.current_match_data.robot_score));
    sprintf(Global.current_match_data.human_score, "%d", human_score);
    sprintf(Global.current_match_data.robot_score, "%d", robot_score);
    pthread_mutex_unlock(&Global.current_match_data_mutex);
}

void set_current_match_start_date(const char* str_time)
{
    pthread_mutex_lock(&Global.current_match_data_mutex);
    memset(Global.current_match_data.date, 0 , sizeof(Global.current_match_data));
    sprintf(Global.current_match_data.date, "%s", str_time);
    pthread_mutex_unlock(&Global.current_match_data_mutex);
}

