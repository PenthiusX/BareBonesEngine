#ifndef _CPU_COMPUTE_H
#define _CPU_COMPUTE_H
#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <_tools.h>

class _Cpu_Compute
{

public:
    _Cpu_Compute();
    static std::vector<int> compute_k_means(std::array<unsigned int, 256> arr);
    _Tools::ModelData generateModelMesh(int *wrap_frame, unsigned int iwidth, unsigned int iheight);
    void checkCollision(std::vector<float> &stone_verts, std::vector<float> vertsD, std::vector<unsigned int> &indiceD);
};

#endif // _CPU_COMPUTE_H
