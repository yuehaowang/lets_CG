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
std::vector<float> AxesIndicator::geom_normal_data = std::vector<float>();

AxesIndicator::AxesIndicator()
{
    if (geom_vertex_data.size() == 0 || geom_normal_data.size() == 0) {
        GenerateAxesGeometry();
    }

    mat = new BasicMaterial("glyk/shaders/axes_indicator");
    mesh = new Mesh(mat, Geometry(geom_vertex_data, geom_normal_data));
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

    Vec3f normal_list[] = {
        Vec3f(235.0f, 64.0f, 52.0f) / 255.0f,
        Vec3f(50.0f, 168.0f, 82.0f) / 255.0f,
        Vec3f(66.0f, 135.0f, 245.0f) / 255.0f
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < vert.size(); j += 3) {
            Vec3f p(vert[j], vert[j + 1], vert[j + 2]);
            Vec3f v = *trans_list[i] * p;

            geom_vertex_data.push_back(v.x);
            geom_vertex_data.push_back(v.y);
            geom_vertex_data.push_back(v.z);

            Vec3f n = normal_list[i];
            geom_normal_data.push_back(n.x);
            geom_normal_data.push_back(n.y);
            geom_normal_data.push_back(n.z);
        }
    }
}

