#include "glyk/indicator.h"


/***************** Indicator *****************/

Indicator::Indicator()
: mat(NULL)
, mesh(NULL)
{

}

Indicator::~Indicator()
{
    delete mat;
    delete mesh;
}


/***************** IconIndicator *****************/

IconIndicator::IconIndicator(std::string tex_path)
{
    mat = new BasicMaterial(
        "glyk/shaders/icon_indicator",
        Texture(tex_path)
    );
    mesh = new Mesh(mat, QuadGeometry());
}


/***************** AxesIndicator *****************/

std::vector<float> AxesIndicator::geom_vertex_data = std::vector<float>();

AxesIndicator::AxesIndicator()
{
    if (geom_vertex_data.size() == 0) {
        GenerateAxesGeometry();
    }

    mat = new BasicMaterial("glyk/shaders/axes_indicator");
    mesh = new Mesh(mat, Geometry(geom_vertex_data));
}

void AxesIndicator::GenerateAxesGeometry() {
    std::vector<float> vertex_data;

    BoxGeometry geom;
    std::vector<float> vert(geom.VertexData());

    Mat4x4f trans_x;
    trans_x.Translate(0.5, 0, 0.0);
    trans_x.Scale(0.5, 0.04, 0.04);
    Mat4x4f trans_y;
    trans_y.Translate(0, 0.5, 0.0);
    trans_y.Scale(0.04, 0.5, 0.04);
    Mat4x4f trans_z;
    trans_z.Translate(0, 0, 0.5);
    trans_z.Scale(0.04, 0.04, 0.5);

    Mat4x4f* trans_list[] = {&trans_x, &trans_y, &trans_z};

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < vert.size(); j += 3) {
            Vec3f p(vert[j], vert[j + 1], vert[j + 2]);
            Vec3f v = *trans_list[i] * p;

            geom_vertex_data.push_back(v.x);
            geom_vertex_data.push_back(v.y);
            geom_vertex_data.push_back(v.z);
        }
    }
}

