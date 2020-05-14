#pragma once
#include <string>
#include <omp.h>

#define DEFAULT_NUM_SAMPLES 30
#define DEFAULT_MAX_DEPTH 5
#define DEFAULT_LIGHT_POWER 24.0f
#define DEFUALT_NUM_THREADS omp_get_max_threads()


struct Config
{
	int num_samples;
	int max_depth;
	float light_power;
	int num_threads;

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
		std::string light_power_str = get_cmd_option(argc, argv, "-light_pow=");
		std::string num_threads_str = get_cmd_option(argc, argv, "-num_threads=");

		if (num_samples_str.empty()) {
			num_samples = DEFAULT_NUM_SAMPLES;
		} else {
			num_samples = std::stoi(num_samples_str);
		}

		if (max_depth_str.empty()) {
			max_depth = DEFAULT_MAX_DEPTH;
		} else {
			max_depth = std::stoi(max_depth_str);
		}

		if (light_power_str.empty()) {
			light_power = DEFAULT_LIGHT_POWER;
		} else {
			light_power = std::stof(light_power_str);
		}

		if (num_threads_str.empty()) {
			num_threads = DEFUALT_NUM_THREADS;
		} else {
			num_threads = std::stof(num_threads_str);
		}
	}
};