
#include <iostream>
#include <algorithm>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "camera.hpp"
#include "mathext.hpp"


inline float clamp(float x) { return x < 0 ? 0 : x > 1 ? 1 : x; }
inline unsigned char toInt(float x) { unsigned char c(pow(clamp(x), 1 / 2.2) * 255 + .5);return c; }


int main()
{

    Eigen::Vector3f cameraPosition(0, 0, 10);
	Eigen::Vector3f cameraLookAt(0, 0, 0);
	Eigen::Vector3f cameraUp(0, 1, 0);
	float verticalFov = 45;
	Eigen::Vector2i filmRes(500, 500);

	Camera camera(cameraPosition, cameraLookAt, cameraUp, verticalFov, filmRes);

    for (int i = 0; i < 10000; i++) {
        Eigen::Vector2f sample = mathext::disk(250);

        float x = sample.x() * cos(sample.y()) + 250.0f;
        float y = sample.x() * sin(sample.y()) + 250.0f;
        camera.setPixel(x, y, Eigen::Vector3f(1.0f, 0.0f, 0.0f));
    }
	
	std::string outputPath = "./disk_sample.png";
	std::vector<unsigned char> outputData;
	outputData.reserve(int(filmRes.x() * filmRes.y() * 3));
	for (Eigen::Vector3f v : camera.m_Film.pixelSamples)
	{
		for (int i = 0; i < 3; i++)
		{
			outputData.push_back(toInt(v[i]));
		}
	}
	stbi_flip_vertically_on_write(true);
	stbi_write_png(outputPath.c_str(), filmRes.x(), filmRes.y(), 3, outputData.data(), 0);

	return 0;
}