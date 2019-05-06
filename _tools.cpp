#include "_tools.h"
#include <QFile>
#include <QString>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

//Commonly used Functions

_Tools::_Tools()
{

}

QString _Tools::ReadStringFromQrc(QString Filename)
{
    // Read text files from .qrc file
    // return as QString
    QString mText = "";
    QFile mFile(Filename);

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "could not open file for read";
    }
        QTextStream in(&mFile);
        mText = in.readAll();

        mFile.close();
        return mText;
}

QJsonObject _Tools::ReadJsonFromQrc(QString Filename)
{
    QFile loadFile(Filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc = QJsonDocument::fromJson(saveData);

    return loadDoc.object();
}

bool _Tools::WriteJsonToQrc(QString Filename,QJsonObject jsonObject)
{
    QFile saveFile(Filename);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonDocument saveDoc(jsonObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void _Tools::SaveImageToPgm(char* img,unsigned int rttWidth, unsigned int rttHeight, QString filename)
{
    static FILE* imagefile;
    //For saving processed image
            imagefile=fopen(filename.toLocal8Bit(), "wb");

            if( imagefile == NULL) {
                qDebug() << "Can't create:" << filename;
            }
            fprintf(imagefile,"P5\n%u %u 255\n", rttWidth, rttHeight);
            fwrite(img, 1, rttWidth*rttHeight, imagefile);
            fclose(imagefile);

    qDebug() << "wrote: " << filename;
    //delete imagefile;
    //deleting file pointer is giving error
}


/* Function : GenerateFactors(int n)
 * this function find out list of integers such that their product is n
 * created: 15_04_2019
*/
std::vector<int> _Tools::GenerateFactors(int n)
{
    std::vector<int> factors;
    for(int i = 2; i <= n;)
    {
        if(n % i == 0)
        {
            factors.push_back(i);
            n=n/i;
        }
        else  i++;
    }
    return factors;
}

/* Function : NormalizeSum(glm::vec3 in)
 * this function returns normalised vec3 such that th sum off elements is 1.0
 * divides orignal vec3 by sum of all elements
 * created: 15_04_2019
*/
glm::vec3 _Tools::NormalizeSum(glm::vec3 in)
{
    return in/(in.x+in.y+in.z);
}

/* Function : argmax(glm::vec3 in)
 * this function returns index of elemnt having maximum value
 * index 0 if x is max ,1 if y and 2 if z
 * created: 15_04_2019
 */
int _Tools::argmax(glm::vec3 in){
    int m = 0;
    float mv=0;
    if(mv<in.x)
    {
        mv=in.x;
        m=0;
    }
    if(mv<in.y)
    {
        mv=in.y;
        m=1;
    }
    if(mv<in.z)
    {
        mv=in.z;
        m=2;
    }
    return m;
}

/* Function : GetGroupSize(int n,glm::vec3 ratio = glm::vec3(3,3,1))
 * this function returns 3 component ivec3 such that their product is n
 * and ratio of these numbers is roughly close to given ratio
 * index 0 if x is max ,1 if y and 2 if z
 * created: 15_04_2019
*/
glm::ivec3 _Tools::GetGroupSize(int n,glm::vec3 ratio)
{
    glm::ivec3 bins=glm::ivec3(1);

    std::vector<int> fact = GenerateFactors(n);

    for (int f = fact.size()-1; f>-1; f--) {
        glm::vec3 error=NormalizeSum(ratio)-NormalizeSum(glm::vec3(bins));

        switch (argmax(error)) {
        case 0:
            bins.x*=fact[f];
            break;
        case 1:
            bins.y*=fact[f];
            break;
        case 2:
            bins.z*=fact[f];
            break;
        }
    }
    return bins;
}
