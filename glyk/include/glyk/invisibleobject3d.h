#ifndef _INVISIBLE_OBJECT3D_H_
#define _INVISIBLE_OBJECT3D_H_

#include <string>
#include "glyk/object3d.h"
#include "glyk/indicator.h"


class Scene;


class InvisibleObject3D : public Object3D
{

protected:

    std::vector<Indicator*> indicators;
    void BindIndicator(Indicator* ind);

    friend class Scene;

public:

    InvisibleObject3D();
    virtual ~InvisibleObject3D();
    virtual void AddIndicator(Indicator* ind);

};


#endif
