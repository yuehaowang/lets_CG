#include <algorithm>
#include <iostream>
#include <string>

#include "config.hpp"
#include "renderer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define FN_MODIFIER (conf.interpolate_method + "_" + to_string_float(conf.sample_step) + "_" + std::to_string(conf.output_resolution) + "_" + conf.colormap_name + (conf.disable_inscattering ? "_nois" : ""))

#define START_FRAME 55
#define END_FRAME 124

Config conf;

std::string to_string_float(float v, int n = 3)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << v;
    return out.str();
}

void renderSingle(std::string data_name)
{
    /*
     * 1. Volume Setting
     */
    std::string filepath = conf.input_dir + data_name + ".bin";
    Volume vol(filepath);
    if (vol.is_null)
        return;

    /*
     * 2. Camera Setting
     */
    Eigen::Vector3f cameraPosition = vol.bbox.getCenter() - 2.5 * Eigen::Vector3f(0, 0, vol.size_physics.z());
    Eigen::Vector3f cameraLookAt = vol.bbox.getCenter();
    Eigen::Vector3f cameraUp(0, 1, 0);
    float verticalFov = 45;
    Eigen::Vector2i filmRes(conf.output_resolution, conf.output_resolution);

    Camera camera(cameraPosition, cameraLookAt, cameraUp, verticalFov, filmRes);
    PointLight light(cameraPosition + Eigen::Vector3f(0, 5, 0), Eigen::Vector3f(1, 1, 1));

    /*
     * 3. Renderer Setting
     */
    Renderer renderer;
    renderer.setCamera(&camera);
    renderer.setVolume(&vol);
    renderer.addLight((Light *)&light);

    myClassifier classifer;
    renderer.setClassifier((Classifier *)&classifer);

    TrilinearInterpolator tri_interpolator;
    NearestNeighbourInterpolator nn_interpolator;

    if (conf.interpolate_method == "tri")
    {
        renderer.setInterpolator((Interpolator *)&tri_interpolator);
    }
    else if (conf.interpolate_method == "nn")
    {
        renderer.setInterpolator((Interpolator *)&nn_interpolator);
    }
    else
    {
        return;
    }

    /*
     * 4. Render
     */
    if (conf.forward_render)
    {
        std::string output_path = conf.output_dir + data_name + "_fw_" + FN_MODIFIER + ".png";
        renderer.renderFrontToBack(output_path);
    }
    if (conf.backward_render)
    {
        std::string output_path = conf.output_dir + data_name + "_bw_" + FN_MODIFIER + ".png";
        renderer.renderBackToFront(output_path);
    }
}

void multipleRender()
{
    /*
     * 1. Camera Setting
     */
    std::cout << "-- Preparing camera setting..." << std::endl;
    char filein[256];
    sprintf(filein, "%sframe%05d.bin", conf.input_dir.c_str(), START_FRAME);
    Volume test_vol(filein);
    if (test_vol.is_null)
        return;
    Eigen::Vector3f cameraPosition = test_vol.bbox.getCenter() - 2.5 * Eigen::Vector3f(0, 0, test_vol.size_physics.z());
    Eigen::Vector3f cameraLookAt = test_vol.bbox.getCenter();
    Eigen::Vector3f cameraUp(0, 1, 0);
    float verticalFov = 45;
    Eigen::Vector2i filmRes(conf.output_resolution, conf.output_resolution);

    Camera camera(cameraPosition, cameraLookAt, cameraUp, verticalFov, filmRes);
    PointLight light(cameraPosition + Eigen::Vector3f(0, 5, 0), Eigen::Vector3f(1, 1, 1));

    /*
     * 2. Renderer Setting
     */
    Renderer renderer;
    renderer.setCamera(&camera);
    renderer.addLight((Light *)&light);

    myClassifier classifer;
    renderer.setClassifier((Classifier *)&classifer);

    TrilinearInterpolator tri_interpolator;
    NearestNeighbourInterpolator nn_interpolator;

    if (conf.interpolate_method == "tri")
    {
        renderer.setInterpolator((Interpolator *)&tri_interpolator);
    }
    else if (conf.interpolate_method == "nn")
    {
        renderer.setInterpolator((Interpolator *)&nn_interpolator);
    }
    else
    {
        return;
    }

    /*
     * 3. Render multiple volumes
     */
    std::cout << std::endl;
    std::cout << "-- Rendering multiple volumes... Total: " << (END_FRAME - START_FRAME + 1) << " frames" << std::endl;
    for (int frame = START_FRAME; frame <= END_FRAME; frame++)
    {
        char filein[256];
        sprintf(filein, "%sframe%05d.bin", conf.input_dir.c_str(), frame);
        Volume vol(filein);
        if (vol.is_null)
            continue;

        renderer.setVolume(&vol);

        char frame_name[256];
        sprintf(frame_name, "frame%05d", frame);
        if (conf.forward_render)
        {
            std::string output_path = conf.output_dir + frame_name + "_fw_" + FN_MODIFIER + ".png";
            renderer.renderFrontToBack(output_path);
        }
        if (conf.backward_render)
        {
            std::string output_path = conf.output_dir + frame_name + "_bw_" + FN_MODIFIER + ".png";
            renderer.renderBackToFront(output_path);
        }
    }
}

int main(int argc, char *argv[])
{
    conf.parse_args(argc, argv);

    if (!conf.single_frame.empty())
    {
        renderSingle(conf.single_frame);
    }
    else
    {
        multipleRender();
    }

    return 0;
}