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
