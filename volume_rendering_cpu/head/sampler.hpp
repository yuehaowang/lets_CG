#pragma once
#include <stdlib.h>

class Sampler
{
private:
    /* data */
public:
    Sampler(/* args */){};
    ~Sampler(){};
    virtual float next() = 0;
};

class UniformSampler01 : public Sampler
{
    unsigned int seed;

public:
    void setSeed(unsigned int seed)
    {
        this->seed = seed;
        srand(this->seed);
    }
    float next()
    {
        int N = 9999;
        return rand() % (N + 1) / (float)(N + 1);
    }
};