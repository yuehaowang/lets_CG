#pragma once
#include <string>
#include <map>
#include <omp.h>

#define CONF_DEFAULT_NUM_SAMPLES 30
#define CONF_DEFAULT_MAX_DEPTH 5
#define CONF_DEFAULT_LIGHT_POWER 48.0f
#define CONF_DEFUALT_NUM_THREADS omp_get_max_threads()
#define CONF_DEFAULT_INTEGRAL_METHOD "mis"
#define CONF_DEFAULT_OUTPUT_DIR "./"


struct Config
{
    int num_samples;
    int max_depth;
    float light_power;
    int num_threads;
    std::string integral_method;
    std::string output_dir;

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
        /* Set default configure values */
        std::map<std::string, std::string> args_map;
        args_map["-num_samples"] = std::to_string(CONF_DEFAULT_NUM_SAMPLES);
        args_map["-max_depth"] = std::to_string(CONF_DEFAULT_MAX_DEPTH);
        args_map["-light_pow"] = std::to_string(CONF_DEFAULT_LIGHT_POWER);
        args_map["-num_threads"] = std::to_string(CONF_DEFUALT_NUM_THREADS);
        args_map["-int_mth"] = CONF_DEFAULT_INTEGRAL_METHOD;
        args_map["-out_dir"] = CONF_DEFAULT_OUTPUT_DIR;
        /* Parse given arguments */
        for (int i = 0; i < argc; ++i) {
            std::string arg = argv[i];
            size_t splitter_pos = arg.find("=");
            if (splitter_pos != std::string::npos) {
                std::string arg_key = arg.substr(0, splitter_pos);
                std::string arg_val = arg.substr(splitter_pos + 1);
                if (args_map.find(arg_key) != args_map.end()) {
                    args_map[arg_key] = arg_val;
                } else {
                    std::cout << "Unknown option: " << arg << std::endl;
                }
            } else {
                args_map[arg] = "1";
            }
        }

        num_samples = std::stoi(args_map["-num_samples"]);
        max_depth = std::stoi(args_map["-max_depth"]);
        light_power = std::stof(args_map["-light_pow"]);
        num_threads = std::stoi(args_map["-num_threads"]);
        integral_method = args_map["-int_mth"];
        output_dir = args_map["-out_dir"];
        if (output_dir.back() != '/') {
            output_dir += "/";
        }

        std::cout << "=====================" << std::endl;
        std::cout << "max depth: " << max_depth << std::endl;
        std::cout << "number of samples: " << num_samples << std::endl;
        std::cout << "light power: " << light_power << std::endl;
        std::cout << "integral method: " << integral_method << std::endl;
        std::cout << "max number of threads: " << omp_get_max_threads() << std::endl;
        omp_set_num_threads(num_threads);
        #pragma omp parallel
        {
            #pragma omp single
            {
                std::cout << "number of using threads: " << omp_get_num_threads() << std::endl;
            }
        }
        std::cout << "output directory: " << output_dir << std::endl;
        std::cout << "=====================" << std::endl << std::endl;
    }
};