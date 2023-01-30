#include "volume.h"

template <typename T>
void SwapEnd(T &var)
{
    char *varArray = reinterpret_cast<char *>(&var);
    for (long i = 0; i < static_cast<long>(sizeof(var) / 2); i++)
        std::swap(varArray[sizeof(var) - 1 - i], varArray[i]);
}

Volume::Volume(){};

Volume::~Volume(){};

Volume::Volume(std::string volumefile)
{
    std::cout << "-- loading " << volumefile << std::endl;
    FILE *fp = fopen(volumefile.c_str(), "rb");
    if (fp)
    {
        fread(this->size.data(), sizeof(int), 3, fp);
        fread(this->size_physics.data(), sizeof(float), 3, fp);
        this->bbox = AABB(0, 0, 0, size_physics.x(), size_physics.y(), size_physics.z());
        this->dx = size_physics.x() / (this->size.x() - 1);
        int count = this->size.x() * this->size.y() * this->size.z();
        this->raw_data.clear();
        float den = 0;
        float min_den = 9999;
        float max_den = -9999;
        for (int curind = 0; curind < count; curind++)
        {
            int z_idx = curind / (this->size.y() * this->size.x());
            int y_idx = (curind - z_idx * this->size.x() * this->size.y()) / this->size.x();
            int x_idx = curind - z_idx * this->size.x() * this->size.y() - y_idx * this->size.x();
            fread(&den, sizeof(float), 1, fp);
            min_den = std::min(min_den, den);
            max_den = std::max(max_den, den);
            this->raw_data.push_back(volumeData(this->dx * x_idx, this->dx * y_idx, this->dx * z_idx, den));
        }
        printf("density range [%.3f ,%.3f]\n", min_den, max_den);

        is_null = false;
        fclose(fp);

        this->computeGradients();
    }
    else
    {
        printf("failed to open %s\n", volumefile.c_str());

        is_null = true;
    }
};

bool Volume::getRayStartEnd(Ray &ray, float &t_start, float &t_end) { return bbox.rayIntersection(ray, t_start, t_end); };

volumeData &Volume::indexToData(Eigen::Vector3i index) { return this->raw_data.at(index.z() * size.y() * size.x() + index.y() * size.x() + index.x()); };

voxel Volume::getVoxel(Eigen::Vector3f position)
{
    position = position.cwiseMin(this->size_physics);
    Eigen::Vector3f index = (position - bbox.lb);
    index = index / this->dx;
    index = index.cwiseMax(Eigen::Vector3f::Zero());
    index = index.cwiseMin((this->size - Eigen::Vector3i(2, 2, 2)).cast<float>());

    Eigen::Vector3i lb(std::floor(index.x()), std::floor(index.y()), std::floor(index.z()));
    volumeData &c000 = indexToData(lb);
    volumeData &c100 = indexToData(lb + Eigen::Vector3i(1, 0, 0));
    volumeData &c010 = indexToData(lb + Eigen::Vector3i(0, 1, 0));
    volumeData &c110 = indexToData(lb + Eigen::Vector3i(1, 1, 0));
    volumeData &c001 = indexToData(lb + Eigen::Vector3i(0, 0, 1));
    volumeData &c101 = indexToData(lb + Eigen::Vector3i(1, 0, 1));
    volumeData &c011 = indexToData(lb + Eigen::Vector3i(0, 1, 1));
    volumeData &c111 = indexToData(lb + Eigen::Vector3i(1, 1, 1));
    return voxel(c000, c100, c010, c110, c001, c101, c011, c111);
};

void Volume::computeGradients()
{
    auto central_diff = [this](Eigen::Vector3i coords, int var_idx) -> float {
        /* Handle boudary conditions.
		   Set boundary's graident as its neighbours' gradient */
        bool on_lower_bound = coords(var_idx) <= 0, on_upper_bound = coords(var_idx) >= size(var_idx) - 1;
        if (on_lower_bound && on_upper_bound)
        {
            return 0.0f;
        }
        if (on_lower_bound)
        {
            coords(var_idx) += 1;
        }
        else if (on_upper_bound)
        {
            coords(var_idx) -= 1;
        }

        Eigen::Vector3i prev_coords = coords;
        prev_coords(var_idx) -= 1;
        Eigen::Vector3i next_coords = coords;
        next_coords(var_idx) += 1;

        const volumeData &curr = indexToData(coords);
        const volumeData &prev = indexToData(prev_coords);
        const volumeData &next = indexToData(next_coords);

        /* Find gradient */
        return (next.density - prev.density) / (dx * 2);
    };

    this->grad_maxnorm = std::numeric_limits<float>::min();
    int count = this->raw_data.size();

    for (int i = 0; i < count; i++)
    {
        int z_idx = i / (this->size.y() * this->size.x());
        int y_idx = (i - z_idx * this->size.x() * this->size.y()) / this->size.x();
        int x_idx = i - z_idx * this->size.x() * this->size.y() - y_idx * this->size.x();
        Eigen::Vector3i idx_coords(x_idx, y_idx, z_idx);

        float x_grad = central_diff(idx_coords, 0);
        float y_grad = central_diff(idx_coords, 1);
        float z_grad = central_diff(idx_coords, 2);
        Eigen::Vector3f grad = Eigen::Vector3f(x_grad, y_grad, z_grad);

        this->grad_maxnorm = std::max(this->grad_maxnorm, grad.norm());
        this->raw_data[i].gradient = grad;
    }
    printf("max_gradient norm %f \n", grad_maxnorm);
};
