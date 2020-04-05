#ifndef _INDICATOR_H_
#define _INDICATOR_H_

#include <string>
#include "glyk/mesh.h"
#include "glyk/material.h"
#include "glyk/texture.h"
#include "glyk/geometry.h"


/***************** Indicator *****************/

struct Indicator
{

    Material* mat;
    Mesh* mesh;

    Indicator();
    virtual ~Indicator();

};


/***************** IconIndicator *****************/

struct IconIndicator : public Indicator
{

    IconIndicator(std::string tex_path);

};


/***************** AxesIndicator *****************/

struct AxesIndicator : public Indicator
{

    static std::vector<float> geom_vertex_data;
    static std::vector<float> geom_normal_data;

    AxesIndicator();
    void GenerateAxesGeometry();

};

#endif
