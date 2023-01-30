#pragma once
#define TINYCOLORMAP_WITH_EIGEN
#include "tinycolormap.hpp"
#include <iostream>
#include <map>
#include <omp.h>
#include <string>

#define CONF_DEFUALT_NUM_THREADS omp_get_max_threads()
#define CONF_DEFAULT_BACKWARD "0"
#define CONF_DEFAULT_FORWARD "0"
#define CONF_DEFAULT_OUTPUT_DIR "./"
#define CONF_DEFAULT_INPUT_DIR "data/"
#define CONF_DEFAULT_FRAME ""
#define CONF_DEFAULT_OUTPUT_RES "1024"
#define CONF_DEFAULT_SAMPLE_STEP "0.1"
#define CONF_DEFAULT_INTERPOLATE_METHOD "tri"
#define CONF_DEFAULT_COLORMAP "heat"
#define CONF_DEFAULT_DISABLE_INSCATTERING "0"

struct Config
{
    int num_threads;
    bool backward_render;
    bool forward_render;
    std::string output_dir;
    std::string input_dir;
    std::string single_frame;
    int output_resolution;
    float sample_step;
    std::string interpolate_method;
    tinycolormap::ColormapType colormap;
    std::string colormap_name;
    bool disable_inscattering;

    static std::string get_cmd_option(int argc, char *argv[], const std::string &option)
    {
        std::string cmd = "";
        for (int i = 0; i < argc; ++i)
        {
            std::string arg = argv[i];
            if (arg.find(option) == 0)
            {
                std::size_t found = arg.find_last_of(option);
                cmd = arg.substr(found + 1);
                return cmd;
            }
        }
        return cmd;
    }

    void parse_args(int argc, char *argv[])
    {
        /* Set default configure values */
        std::map<std::string, std::string> args_map;
        args_map["-num_threads"] = std::to_string(CONF_DEFUALT_NUM_THREADS);
        args_map["-backward"] = CONF_DEFAULT_BACKWARD;
        args_map["-forward"] = CONF_DEFAULT_FORWARD;
        args_map["-out_dir"] = CONF_DEFAULT_OUTPUT_DIR;
        args_map["-in_dir"] = CONF_DEFAULT_INPUT_DIR;
        args_map["-frame"] = CONF_DEFAULT_FRAME;
        args_map["-out_res"] = CONF_DEFAULT_OUTPUT_RES;
        args_map["-sample_step"] = CONF_DEFAULT_SAMPLE_STEP;
        args_map["-i_mth"] = CONF_DEFAULT_INTERPOLATE_METHOD;
        args_map["-cm"] = CONF_DEFAULT_COLORMAP;
        args_map["-no-inscattering"] = CONF_DEFAULT_DISABLE_INSCATTERING;
        /* Parse given arguments */
        for (int i = 0; i < argc; ++i)
        {
            std::string arg = argv[i];
            size_t splitter_pos = arg.find("=");
            if (splitter_pos != std::string::npos)
            {
                std::string arg_key = arg.substr(0, splitter_pos);
                std::string arg_val = arg.substr(splitter_pos + 1);
                if (args_map.find(arg_key) != args_map.end())
                {
                    args_map[arg_key] = arg_val;
                }
                else
                {
                    std::cout << "Unknown option: " << arg << std::endl;
                }
            }
            else
            {
                args_map[arg] = "1";
            }
        }

        num_threads = std::stoi(args_map["-num_threads"]);
        backward_render = (bool)std::stoi(args_map["-backward"]);
        forward_render = (bool)std::stoi(args_map["-forward"]);
        if (!(backward_render || forward_render))
            backward_render = true;
        output_dir = args_map["-out_dir"];
        if (output_dir.back() != '/')
            output_dir += "/";
        input_dir = args_map["-in_dir"];
        if (input_dir.back() != '/')
            input_dir += "/";
        single_frame = args_map["-frame"];
        output_resolution = std::stoi(args_map["-out_res"]);
        if (output_resolution <= 0)
            output_resolution = std::stoi(CONF_DEFAULT_OUTPUT_RES);
        sample_step = std::stof(args_map["-sample_step"]);
        if (sample_step <= 0)
            sample_step = std::stoi(CONF_DEFAULT_SAMPLE_STEP);
        interpolate_method = args_map["-i_mth"];
        colormap_name = args_map["-cm"];
        if (interpolate_method != "tri" && interpolate_method != "nn")
            std::cout << "Unsupported option value of -i_mth: " << interpolate_method << std::endl;
        if (args_map["-cm"] == "heat")
            colormap = tinycolormap::ColormapType::Heat;
        else if (args_map["-cm"] == "hot")
            colormap = tinycolormap::ColormapType::Hot;
        else if (args_map["-cm"] == "jet")
            colormap = tinycolormap::ColormapType::Jet;
        else if (args_map["-cm"] == "magma")
            colormap = tinycolormap::ColormapType::Magma;
        else if (args_map["-cm"] == "plasma")
            colormap = tinycolormap::ColormapType::Plasma;
        else if (args_map["-cm"] == "viridis")
            colormap = tinycolormap::ColormapType::Viridis;
        else if (args_map["-cm"] == "cividis")
            colormap = tinycolormap::ColormapType::Cividis;
        else if (args_map["-cm"] == "github")
            colormap = tinycolormap::ColormapType::Github;
        else
        {
            colormap = tinycolormap::ColormapType::Heat;
            colormap_name = "heat";
        }
        disable_inscattering = (bool)std::stoi(args_map["-no-inscattering"]);

        std::cout << "=====================" << std::endl;
        std::cout << "composition scheme: " << (backward_render ? "backward" : "") << " " << (forward_render ? "forward" : "") << std::endl;
        std::cout << "max number of threads: " << omp_get_max_threads() << std::endl;
        omp_set_num_threads(num_threads);
#pragma omp parallel
        {
#pragma omp single
            {
                std::cout << "number of using threads: " << omp_get_num_threads() << std::endl;
            }
        }

        std::cout << "input directory: " << input_dir << std::endl;
        std::cout << "output directory: " << output_dir << std::endl;
        std::cout << "output resolution: " << output_resolution << " x " << output_resolution << std::endl;
        std::cout << "samples per voxel: " << sample_step << std::endl;
        std::cout << "interpolation method: " << interpolate_method << std::endl;
        std::cout << "colormap: " << colormap_name << std::endl;
        std::cout << "inscattering: " << (disable_inscattering ? "FALSE" : "TRUE") << std::endl;
        std::cout << "=====================" << std::endl;
        std::cout << std::endl;
    }
};