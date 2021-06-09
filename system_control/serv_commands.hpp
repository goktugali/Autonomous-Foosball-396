#ifndef SERV_COMMANDS_HPP
#define SERV_COMMANDS_HPP

/**
 * THIS FILE CONTAINS THE REQUEST & RESPONSE COMMANDS AS STRING TO
 * DEFINE THE COMMUNICATION PROTOCOL BETWEEN DESKTOP AND EMBEDDED DEVICE.
 *
 * THIS FILE ALSO USED BY DESKTOP QT UI APPLICATION.
 *
 */

#define CMD_START_GAME_EASY 			"cmd_start_game_easy"
#define CMD_START_GAME_MEDIUM			"cmd_start_game_medium"
#define CMD_START_GAME_HARD			    "cmd_start_game_hard"
#define CMD_STOP_GAME  			        "cmd_stop_game"
#define CMD_GET_OLD_SCORES 		        "cmd_get_old_scores"
#define RESPONSE_OK                     "OK"

#endif