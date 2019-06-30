#include "_cpu_compute.h"

#define PI 3.1415926535897932384626433832795

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


_Tools::ModelData _Cpu_Compute::generateModelMesh(int *wrap_frame, unsigned int iwidth, unsigned int iheight)
{
    std::vector<float> vertsG;
    std::vector<unsigned int> indiceG;

    unsigned int index[4] = {0,0,0,0};

    glm::ivec2 resolution = glm::ivec2(iwidth,iheight);//wrap texture size

    for (unsigned int h = 0; h < resolution.y; h++) {
        for (unsigned int w = 0; w < resolution.x; w++) {

            glm::vec2 pixel_cord = glm::vec2(w,h);

            //glm::vec2 texture_cord = glm::vec2((pixel_cord.x +0.5)/resolution.x,(pixel_cord.y +0.5)/resolution.y);

            int index = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
            //texture_positions
            float r = wrap_frame[index];
            float theta = 2 * PI * float(w) /resolution.x;

            float x=r*cos(theta) ,y=r*sin(theta) ,z = h-(resolution.y*0.5);

            vertsG.push_back(-x);//x = s
            vertsG.push_back(-z);//y = t
            vertsG.push_back(y);//z = 0.0
        }
    }

    glm::ivec2 step_size = glm::ivec2(1,1);

    glm::vec3 origin = glm::vec3(0,0,resolution.y/2);

    float volume = 0.0;
    for (unsigned int h = 0; h < resolution.y; h+=step_size.y) {
        for (unsigned int w = 0; w < resolution.x; w+=step_size.x) {

            glm::vec2 pixel_cord = glm::vec2(w,h);

            //indexes of neibhouring vertexes
            index[0] = _Tools::indexFromPixelCordinates(pixel_cord,resolution);
            index[1] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,0),resolution);
            index[2] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(step_size.x,step_size.y),resolution);
            index[3] = _Tools::indexFromPixelCordinates(pixel_cord+glm::vec2(0,step_size.y),resolution);

            //            if((pixel_cord.y < resolution.y) && (pixel_cord.y > 80))
            //            {


            if((pixel_cord.y < resolution.y))
            {
                //indexs of fisrt triangle in quad
                indiceG.push_back(index[0]);
                indiceG.push_back(index[1]);
                indiceG.push_back(index[2]);

                //indexs of second triangle in quad
                indiceG.push_back(index[0]);
                indiceG.push_back(index[2]);
                indiceG.push_back(index[3]);
            }
        }
    }
    _Tools::SaveObjModel(vertsG,indiceG,"centered_clay.obj");
    return _Tools::ModelData{vertsG,indiceG};
}
void _Cpu_Compute::checkCollision(std::vector<float>& stone_verts, std::vector<float> vertsD, std::vector<unsigned int>& indiceD)
{
    std::vector<glm::vec4> triangle_equations;

    bool inside=true;

    for (int v = 0; v < indiceD.size()/3; v++) {
        std::vector<glm::vec3> tri_points = _Tools::getTriangleFromElementsIndex(vertsD,indiceD,v);
        glm::vec3 normal = glm::cross( tri_points[1]-tri_points[0], tri_points[2]-tri_points[0]);
        float distance = glm::dot( normal, tri_points[0] ); //P is a point in the plane, like a, b or c
        glm::vec4 eqn(normal,-distance);
        if(distance<0)
            eqn*=-1;
        triangle_equations.push_back(eqn);
    }

    for (int v = 0; v < stone_verts.size()/3; v++) {
        glm::vec4 point(_Tools::getPointFromIndex(stone_verts,v),1.0);

        bool point_inside=true;
        for (auto eqn : triangle_equations) {
            float distance = glm::dot( eqn, point);
            point_inside &= (distance < 0);
        }
        inside &= point_inside;
    }
}
