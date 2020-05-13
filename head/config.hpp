#pragma once
#include <string>

struct Config
{
	int num_samples;
	int max_depth;

	static std::string get_cmd_option(int argc, char* argv[], const std::string& option)
	{
		std::string cmd = "";
		for (int i = 0; i < argc; ++i) {
			std::string arg = argv[i];
			if (arg.find(option) == 0) {
				std::size_t found = arg.find_last_of(option);
				cmd = arg.substr(found + 1);
				return cmd;
			}
		}
		return cmd;
	}

	void parse_args(int argc, char* argv[])
	{
		std::string num_samples_str = get_cmd_option(argc, argv, "-num_samples=");
		std::string max_depth_str = get_cmd_option(argc, argv, "-max_depth=");

		if (num_samples_str.empty()) {
			num_samples = 30;
		} else {
			num_samples = std::stoi(num_samples_str);
		}

		if (max_depth_str.empty()) {
			max_depth = 5;
		} else {
			max_depth = std::stoi(max_depth_str);
		}
	}
};