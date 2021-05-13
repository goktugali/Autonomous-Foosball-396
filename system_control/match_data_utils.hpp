#ifndef MATCH_DATA_UTILS_HPP
#define MATCH_DATA_UTILS_HPP
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../defines.hpp"
#include "../global.hpp"

#define MAX_STORED_MATCH_DATA_SIZE 10

void parse_json_data(const char* json_data, match_data_t* parsed_json_data, int* entry_size);
void get_json_file_content(char* json_content);
void get_current_date_time(char* str_time);
void delete_first_match_data();
void add_new_match_data(const match_data_t* match_data);
void match_data_to_string(const match_data_t* match_data, char* match_data_str);
void set_current_match_start_date(const char* str_time);
void update_current_match_data(uint16_t human_score, uint16_t robot_score);

#endif