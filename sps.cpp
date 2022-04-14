#include "sps.h"
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QSet>
#include <QTextStream>
#include <QtMath>




///Определяем позиции в спс файлах для версси спс 1.0
namespace POSITION_VERS1 {
//S1109                5427                    10317100.0 6209450.0
    QPair <int,int> pairLineVer   (1, 4);
    QPair <int,int> pairPicketVer (21, 4);
    QPair <int,int> pairXVer      (45, 10);
    QPair <int,int> pairYVer      (56, 10);
} // namespace POSITION

///Определяем позиции в спс файлах для версси спс 2.0
namespace POSITION_VERS2 {
//R      40.0     421.0  1G1   0 0.0   0 0   0.08473818.2 5648659.7   0.0  1235959
    QPair <int,int> pairLineVer   (5, 6);
    QPair <int,int> pairPicketVer (14, 6);
    QPair <int,int> pairXVer      (45, 10);
    QPair <int,int> pairYVer      (56, 10);
}

sps::sps(){
    set_versionSps (1);
}
sps::sps( int userversionSps ){
    set_versionSps (userversionSps);
}

inline void sps::set_versionSps(int value){
    switch (value) {
        case 1:
            versionSps = 1;
        break;
        case 2:
            versionSps = 2;
        break;
        default: versionSps = -1;
    };
}

inline double sps::get_versionSps (){
    return versionSps;
}


void sps::readSPSfile(QString namefile, QList<QString> &readFile){
    // Файл в котором будем хранить указанный файл
    QFile fileSps(namefile);
    // Окрываем файл
    if (fileSps.open(QIODevice::ReadOnly | QIODevice::Text)){
        // Файл смогли открыть
        // Читаем из файла
        while ( !fileSps.atEnd() ) {
            // Записываем в список
            readFile << fileSps.readLine();
        }
        fileSps.close();
    } else {
                QMessageBox::warning ( NULL,"ERROR","Файл не открыт " + namefile, QMessageBox::Ok);
           };
}
bool sps::listFileCheck(QList<QString> &readFile){
    // Проверяем чтобы список был не пустой
    if ( readFile.empty() ){
        QMessageBox::warning ( NULL,"ERROR","Файл оказался пустым ", QMessageBox::Ok);
        return 0;
    }
    return 1;
}
dataSps::STRUCK_SPS sps::transStr(QString tempString, int versionSps ){
    dataSps::STRUCK_SPS temp;
    QString strLine, strPicket, strX, strY;

    // Получаем нужные позиции из нашей строки
    if (versionSps == 1) {
        // Если версия спс фалов 1
        strLine   = tempString.mid( POSITION_VERS1::pairLineVer.first,   POSITION_VERS1::pairLineVer.second);
        strPicket = tempString.mid( POSITION_VERS1::pairPicketVer.first, POSITION_VERS1::pairPicketVer.second);
        strX      = tempString.mid( POSITION_VERS1::pairXVer.first,      POSITION_VERS1::pairXVer.second);
        strY      = tempString.mid( POSITION_VERS1::pairYVer.first,      POSITION_VERS1::pairYVer.second);
    } else {
        // Если версия спс фалов 2
        strLine   = tempString.mid( POSITION_VERS2::pairLineVer.first,   POSITION_VERS2::pairLineVer.second);
        strPicket = tempString.mid( POSITION_VERS2::pairPicketVer.first, POSITION_VERS2::pairPicketVer.second);
        strX      = tempString.mid( POSITION_VERS2::pairXVer.first,      POSITION_VERS2::pairXVer.second);
        strY      = tempString.mid( POSITION_VERS2::pairYVer.first,      POSITION_VERS2::pairYVer.second);
    }

    bool ok;
    // Переводим позиции в числа
    temp.line   = (int) strLine.toFloat(&ok);
    temp.picket = (int) strPicket.toFloat(&ok);
    temp.x = strX.toDouble(&ok);
    temp.y = strY.toDouble(&ok);

    return temp;
}

void sps::formatSPS(QString namefile, QString simbol){
    // Список в котором будем хранть сырые строки из тектового файла
    QList<QString> readFile;

    readSPSfile( namefile, readFile);
    if (!listFileCheck(readFile)) {
        return;
    };

    /// Итератор по сырому списку
    QList<QString>::iterator iterListSps;
    /// Для обработки одной строки
    QString tempString = "";
    iterListSps = readFile.begin();

    // Цикл по всему списку загруженному из файла
    while ( iterListSps != readFile.end() ){
        tempString = *iterListSps;
        // ищим строку с буквой S или R. Данные символы начинаються всегда на 0 позиции
        if ( tempString.indexOf(simbol) == 0 ){
            //Добавляем в список
            spsFile.append( transStr( tempString, get_versionSps() ) );
        } else {
            // Если не встретили символы r и s
            QMessageBox::warning ( Q_NULLPTR, " Error ",QString ("Выбранный файл не являеться SPS_ "+simbol ), QMessageBox::Ok);
            return;
        }
        iterListSps++;
    }
    // Определяем макисмальный и минимальный пикет в каждой линии
    findMinMaxLinePicket(spsFile);
}
void sps::findMinMaxLinePicket(QList<dataSps::STRUCK_SPS> &spsInput){
    /// для хранения промежуточной структы мин и макс пикета
    dataSps::STRUCK_MIN_MAX tempMinMax;
    // Множество для хранения всех линий без повторения
    QSet<int> setLine;

    // Итератор на список
     QList<dataSps::STRUCK_SPS>::iterator iterList;
    iterList = spsInput.begin();

    // Цикл по всему списку
    while (iterList != spsInput.end()){
        // Формируем множество из всех линий
        setLine.insert(iterList->line);
        iterList++;
    }

    QSet<int>::iterator iterSet;
    iterSet = setLine.begin();
    // Цикл по всему множеству
    while (iterSet != setLine.end()) {
        // Начинаем поиск для линии запис\анные в множетсва

        // Всегда начинаем поиск с начала списка вдруг спс файл не отсортирован
        iterList = spsInput.begin();

        // Инициализируем минимальный и максимальный пикет

        int min = 99999999;
        int max = -1;
        double xMin = 0;
        double xMax = 0;
        double yMin = 0;
        double yMax = 0;

        //Пробегаем весь список для поиска нашей линии
        while (iterList != spsInput.end()){
            // Если нашли линию
            if (*iterSet == iterList->line) {
                // Начинаем искать минимальный и максимальный пикет
                if (min > iterList->picket) {
                    // Миняем местами
                     min = iterList->picket;
                    xMin = iterList->x;
                    yMin = iterList->y;
                    continue;
                }
                if (max < iterList->picket){
                    // Миняем местами
                     max = iterList->picket;
                    xMax = iterList->x;
                    yMax = iterList->y;
                }
            }
            iterList++;
        }
        // После того как пробежались по всему списку значит мы нашли максимальный и минимальный пикет для данной линии
        tempMinMax.line = *iterSet;
        tempMinMax.maxPicket = max;
        tempMinMax.minPicket = min;
        tempMinMax.minX = xMin;
        tempMinMax.maxX = xMax;
        tempMinMax.minY = yMin;
        tempMinMax.maxY = yMax;

        // Добавляем в структуру хранения данных
        spsMinMax.append(tempMinMax);

        iterSet++;
    }
}


