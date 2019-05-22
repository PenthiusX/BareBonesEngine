#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <QJsonObject>
#include <vector>
#include <glm/glm.hpp>

class _Tools
{
public:
    _Tools();
    static QString ReadStringFromQrc(QString Filename);
    static QJsonObject ReadJsonFromQrc(QString Filename);
    static void SaveImageToPgm(char *img, unsigned int rttWidth, unsigned int rttHeight, QString filename);
    static std::vector<int> GenerateFactors(int n);
    static glm::vec3 NormalizeSum(glm::vec3 in);
    static int argmax(glm::vec3 in);
    static glm::ivec3 GetGroupSize(int n, glm::vec3 ratio = glm::vec3(3,3,1));
    static bool WriteJsonToFile(QString Filename,QJsonObject jsonObject);
    static QString getFolderFromDialog(QString aplicationRelativePath);
    static QJsonObject ReadJsonFromSystem(QString Filename);
    static bool WriteJsonToFileSystem(QString filename, QJsonObject config);
    static int indexFromPixelCordinates(glm::vec2 pixel_cord, glm::ivec2 resolution);
    static glm::ivec3 vertIndexesFromElementIndex(int i);
    static glm::vec3 vertsFromIndex(glm::ivec3 indexes);
};


#endif // TOOLS_H
