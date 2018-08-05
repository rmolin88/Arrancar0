// File:           config.hpp
// Description:    Used to load configuration from json file
// Author:		    Reinaldo Molina
// Email:          rmolin88 at gmail dot com
// Revision:	    0.0.0
// Created:        Fri Jun 01 2018 03:55
// Last Modified:  Fri Jun 01 2018 03:55

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <string_view>
#include <rapidjson/document.h>

class config
{
	const std::string_view CMD_OPTION = "-c";
	const std::string_view CMD_DEFAULT = "config.json";

	rapidjson::Document config_file;
	std::string config_file_location;

	// Search for CMD_OPTION inside arguments. Called fromm parse_config_file
	void parse_arguments(int num_options, const char *options[]);

	// If CMD_OPTION found in parse_arguments attempt to load json file using rapidjson
	int load_config_file(void);

	// Helping function used by get_value functions
	int is_there_object_with_member(const char *object, const char *member) const;
public:
	config() : config_file_location(CMD_DEFAULT) {}

	static config& get_config(void)
	{
		static config rc;
		return rc;
	}

	config(config const&) = delete;
	void operator=(config const&) = delete;

	int parse_config_file(int num_options, const char **arguments);

	int get_value(const char *object, const char *member, std::string &value) const;
	int get_value(const char *object, const char *member, double &value) const;
	int get_value(const char *object, const char *member, int &value) const;
	int get_value(const char *object, const char *member, bool &value) const;
};

#endif
