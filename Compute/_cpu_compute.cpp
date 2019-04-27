#include "_cpu_compute.h"

_Cpu_Compute::_Cpu_Compute()
{

}

/*calculates 2-means for clusters in histogram*/
std::vector<int> _Cpu_Compute::compute_k_means(std::array<unsigned int, 256> arr)
{
    float factor = 0.5;
    std::vector<int> k_means={20,230},distance_from_means={0,0},sigma_distance={0,0},sigma_number={0,0};
    std::vector<float> change={0,0};
    do{
    for (int i = 0; i < 256; ++i) {
        for (int k = 0; k < k_means.size(); ++k) {
            distance_from_means[k] = glm::abs(k_means[k]-i);
        }
        if(distance_from_means[0] < distance_from_means[1])
        {
            sigma_distance[0]+=arr[i]*i;
            sigma_number[0]+=arr[i];
        }
        else {
            sigma_distance[1]+=arr[i]*i;
            sigma_number[1]+=arr[i];
        }
    }
    for (int k = 0; k < k_means.size(); ++k) {
        if(sigma_number[k]!=0) change[k]= factor*(float(sigma_distance[k]/sigma_number[k])-k_means[k]);
        else change[k] = 0;
        k_means[k]=k_means[k]+change[k];
    }
    }while((glm::abs(change[0])+glm::abs(change[1]))>0.5);

    return k_means;
}


