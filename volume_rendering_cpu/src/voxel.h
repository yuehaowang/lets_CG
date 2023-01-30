#pragma once
#include "volumeData.h"

class voxel
{
public:
    volumeData &c000;
    volumeData &c100;
    volumeData &c010;
    volumeData &c110;
    volumeData &c001;
    volumeData &c101;
    volumeData &c011;
    volumeData &c111;
    inline voxel(volumeData &c000, volumeData &c100, volumeData &c010, volumeData &c110, volumeData &c001, volumeData &c101, volumeData &c011, volumeData &c111)
        : c000(c000), c100(c100), c010(c010), c110(c110), c001(c001), c101(c101), c011(c011), c111(c111)
    {
    }
    inline ~voxel() {}
};