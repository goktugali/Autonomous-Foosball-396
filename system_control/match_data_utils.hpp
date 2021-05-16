#ifndef MATCH_DATA_UTILS_HPP
#define MATCH_DATA_UTILS_HPP
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../defines.hpp"
#include "../global.hpp"

#define MAX_STORED_MATCH_DATA_SIZE 10

/**
 * Parses json data into struct array.
 * @param json_data Json raw string data to be parsed.
 * @param parsed_json_data Struct array to be filled.
 * @param entry_size Total json entry size in the raw data.
 */
void parse_json_data(const char* json_data, match_data_t* parsed_json_data, int* entry_size);

/**
 * Gets the content of the json database file.
 * @param json_content Content of the json file to be filled.
 */
void get_json_file_content(char* json_content);

/**
 * Gets the current date & time as string.
 * @param str_time current date & time as string to be filled.
 */
void get_current_date_time(char* str_time);

/**
 * Deletes first match entry from json database file.
 * Uses MAX_STORED_MATCH_DATA_SIZE to check the limit.
 */
void delete_first_match_data();

/**
 * Adds new match data to json database file.
 * @param match_data Match data to be added.
 */
void add_new_match_data(const match_data_t* match_data);

/**
 * Converts match data (struct) to string.
 * @param match_data Match data to be converted.
 * @param match_data_str Match data as string.
 */
void match_data_to_string(const match_data_t* match_data, char* match_data_str);

/**
 * Sets the current match start date.
 * When game thread started, it uses this method.
 * @param str_time Start date of the current match.
 */
void set_current_match_start_date(const char* str_time);

/**
 * Updates the current match score.
 * @param human_score Human score.
 * @param robot_score Robot score.
 */
void update_current_match_data(uint16_t human_score, uint16_t robot_score);

#endif