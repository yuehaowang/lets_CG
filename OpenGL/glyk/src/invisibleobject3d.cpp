#include "glyk/invisibleobject3d.h"


InvisibleObject3D::InvisibleObject3D()
: Object3D()
{

}

InvisibleObject3D::~InvisibleObject3D()
{
    
}

void InvisibleObject3D::AddIndicator(Indicator* ind)
{
    if (ind) {
        indicators.push_back(ind);
    }
}

void InvisibleObject3D::BindIndicator(Indicator* ind)
{
    ind->mesh->SetTransform(Transform());
}
