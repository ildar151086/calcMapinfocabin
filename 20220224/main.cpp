#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QPair>
#include <QMessageBox>

///Определяем позиции в спс файлах в зависимости от версии
namespace POSITION {
    QPair <int,int>   pairLineVer1 (1, 4);
    QPair <int,int> pairPicketVer1 (21, 4);
    QPair <int,int> pairXVer1 (46, 9);
    QPair <int,int> pairYVer1 (56, 9);
} // namespace POSITION


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}


readSps::readSps()
{

}

void readSps::readSPSfile(QString namefile, QList<QString> &readFile){
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


bool readSps::listFileCheck(QList<QString> &readFile){
    // Проверяем чтобы список был не пустой
    if ( readFile.empty() ){
        QMessageBox::warning ( NULL,"ERROR","Файл оказался пустым ", QMessageBox::Ok);
        return 0;
    }
    return 1;
}



readSps::STRUCK_SPS readSps::transStr(QString tempString, int versionSps = 1){
    readSps::STRUCK_SPS temp;
    QString strLine, strPicket, strX, strY;

    // Получаем нужные позиции из начей строки
    if (versionSps == 1) {
        // Если версия спс фалов 1
        strLine   = tempString.mid( POSITION::pairLineVer1.first, POSITION::pairLineVer1.second);
        strPicket = tempString.mid( POSITION::pairPicketVer1.first, POSITION::pairPicketVer1.second);
        strX      = tempString.mid( POSITION::pairXVer1.first, POSITION::pairXVer1.second);
        strY      = tempString.mid( POSITION::pairYVer1.first, POSITION::pairYVer1.second);
    } else {
        // Если версия спс фалов 2
    }

    bool ok;
    // Переводим позиции в числа
    temp.line = strLine.toInt(&ok, 10);
    temp.picket = strPicket.toInt(&ok, 10);
    temp.x = strX.toFloat(&ok);
    temp.y = strY.toFloat(&ok);

    return temp;
}



///
/// \brief Проверяем и формируем спс файлы
/// \param Путь до спс файла
///
void readSps::formatSPS(QString namefile, QString simbol){
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
            sps.append( transStr( tempString, 1 ) );
        }
        iterListSps++;
    }
    // Определяем макисмальный и минимальный пикет в каждой линии
    findMinMaxLinePicket(&sps);
}


///
/// \brief Определяем макисмальное и минимальное значение линии
/// \param Список по спс файлу
///
void readSps::findMinMaxLinePicket(QList<STRUCK_SPS> *spsInput){
    /// для хранения промежуточной структы мин и макс пикета
    readSps::STRUCK_MIN_MAX tempMinMax;
    // Множество для хранения всех линий без повторения
    QSet<int> setLine;

    // Итератор на список
     QList<readSps::STRUCK_SPS>::iterator iterList;
    iterList = spsInput->begin();

    // Цикл по всему списку
    while (iterList != spsInput->end()){
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
        iterList = spsInput->begin();

        // Инициализируем минимальный и максимальный пикет

        int min = iterList->picket;
        int max = iterList->picket;
        float xMin = iterList->x;
        float xMax = iterList->x;
        float yMin = iterList->y;
        float yMax = iterList->y;

        //Пробегаем весь список для поиска нашей линии
        while (iterList != spsInput->end()){
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





