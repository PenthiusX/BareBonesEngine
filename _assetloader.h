#ifndef _ASSETLOADER_H
#define _ASSETLOADER_H
#include <vector>
#include <qstring.h>
#include <qvector3d.h>

#include<glm/glm.hpp>
#include<glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/Logger.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>

#include "_modelinfo.h"

/*
 * Class: _AssetLoader
 * class sets values needed by the viewMatrix
 * for setting of values on Init or realtime
 * Author: Aditya
*/

class _AssetLoader
{
    public:
        _AssetLoader();
        ~_AssetLoader();
        void setModelInfo(_ModelInfo minfo);
        _ModelInfo getModelInfo();
        void objLoader(QString pathToFile);
        void extrenalObjLoader(std::string externalFilePath);//an unideal external file loader , not inteded for embeded executables.
        void assimpLoader(std::string externalFilePath);

        void loadAllModelsInfoFromFolder(QString folderName);//not in use
        _ModelInfo generateQuad();
        _ModelInfo generateTri();
        _ModelInfo generateTri(glm::vec3 p1,glm::vec3 p2,glm::vec3 p3);
        _ModelInfo generateCubeWithNormals();
        std::vector<float> calculateNormalsOfVertices(std::vector<float>vertices);

        //-----------Animation----
        int GetBoneId(const aiBone* pBone);//Wip aditya
        void LoadSingleBone(uint MeshIndex, const aiBone* pBone);
        const aiScene* scene = nullptr;//wip aditya
        //------------------------

    private:
        _ModelInfo modelInfo;
        //
        std::vector<float> vertices;
        std::vector<float>normalVertices;//replaces the vertices array
        std::vector<uint> indices;
        std::vector<float> normals;
        std::vector<float> uvs;
        //
        glm::vec4 vertMin;
        glm::vec4 vertMax;

        int posCounter;
        int arrayCounter;

        //Assimp
        void loadAssimpScene(const aiScene* pScene, std::string path);

        // For converting between ASSIMP and glm
        static inline glm::vec3 vec3_cast(const aiVector3D &v) { return glm::vec3(v.x, v.y, v.z); }
        static inline glm::vec2 vec2_cast(const aiVector3D &v) { return glm::vec2(v.x, v.y); }
        static inline glm::quat quat_cast(const aiQuaternion &q) { return glm::quat(q.w, q.x, q.y, q.z); }
        static inline glm::mat4 mat4_cast(const aiMatrix4x4 &m) { return glm::transpose(glm::make_mat4(&m.a1)); }
        static inline glm::mat4 mat4_cast(const aiMatrix3x3 &m) { return glm::transpose(glm::make_mat3(&m.a1)); }
};

#endif // _ASSETLOADER_H
