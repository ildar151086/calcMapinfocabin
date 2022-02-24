#include "readsps.h"
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QSet>
#include <QTextStream>




///Определяем позиции в спс файлах в зависимости от версии
namespace POSITION {
    QPair <int,int>   pairLineVer1 (1, 4);
    QPair <int,int> pairPicketVer1 (21, 4);
    QPair <int,int> pairXVer1 (46, 9);
    QPair <int,int> pairYVer1 (56, 9);
} // namespace POSITION

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

    // Сохраняем все пикеты в точки в ормате мапинфо
    savePointToMapinfo(&sps, simbol);

    // Сохраняем в линии в формате мипинфо
    saveLineToMapinfo(simbol);



}
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
void readSps::importListToFile(QList<QString>* listString, QString rootNameFile){
    // Сохраняем полученный список  в файл
    QFile outputFile (rootNameFile);
    outputFile.open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream tempStream( &outputFile );

    // Итераор по списку
    QList<QString>::iterator iterListO;
    iterListO = listString->begin();

    // Начинаем цикл по всем записям в списке
    while( iterListO != listString->end() ){
        // Записываем данные в поток
        tempStream << *iterListO << endl;
        iterListO++;
    }
    outputFile.close();
}
void readSps::savePointToMapinfo(QList<STRUCK_SPS> *sps , QString simbol){
    // Сохранем введенную структур в фформате Мапинфо и поместим ее в список
    QList<QString> listPointMapMif, listPointMapMid;
    QString strMif, strMid;

    // Итератор для списка
    QList<STRUCK_SPS>::iterator iterList;
    iterList = sps->begin();

    // Пробегаем по всему списку структры заданного спс файла
    while(iterList != sps->end()){
        // Преобразуем координаты пикеты в строки в формате МИФ
        strMif = "Point " + QString::number(iterList->x,'f', 1) + " " + QString::number(iterList->y,'f', 1);
        listPointMapMif.append(strMif);
        // Добавляем фразу что это символ
        strMif = "Symbol (38,0,12)";
        listPointMapMif.append(strMif);

        // Преобразуем линии и пикеты и координаты пикеты в строки в формате МИД
        strMid  = QString::number(iterList->line, 10) + "," + QString::number(iterList->picket, 10) + ",";
        strMid += QString::number(iterList->x,'f', 1) + "," + QString::number(iterList->y,'f', 1);
        listPointMapMid.append(strMid);

        iterList++;
    }

    // Сохраняем полученный список  в файл
    importListToFile(&listPointMapMif, "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\" + simbol+ "_point.mif");
    importListToFile(&listPointMapMid, "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\" + simbol+ "_point.mid");

}
void readSps::saveLineToMapinfo(QString simbol){
    QString str;
    QList<QString> listLineMapMif,listLineMapMid;
    // Пробигаемяс по всему списку структуры максимальных и минимальных значений
    QList<STRUCK_MIN_MAX>::iterator iterMInMax;
    iterMInMax = spsMinMax.begin();

    while (iterMInMax != spsMinMax.end()){
        // Создаем файл для Линий в формате МИФ
        str  = "Line " + QString::number( iterMInMax->minX, 'f', 1) + " " + QString::number( iterMInMax->minY, 'f', 1);
        str += " " + QString::number( iterMInMax->maxX, 'f', 1) + " " + QString::number( iterMInMax->maxY, 'f', 1);
        listLineMapMif.append(str);
        str = "Pen (4,2,16711680)";
        listLineMapMif.append(str);

        // Создаем файл для Линий в формате МИД
        listLineMapMid.append("....");

        iterMInMax++;
    }

    importListToFile(&listLineMapMif, "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\" + simbol+ "_line.mif" );
    importListToFile(&listLineMapMid, "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\" + simbol+ "_line.mid" );

}
inline float readSps::koffK(STRUCK_MIN_MAX *sps ) {

    // Находим смещения по оси х и у
    float offsetX = sps->maxX - sps->minX;
    float offsetY = sps->maxY - sps->minY;

    return (offsetX != 0) ? offsetY / offsetX : 0;
}

inline float readSps::koffD(STRUCK_MIN_MAX *sps ) {
    // Находим смещения по оси х и у
    float offsetX = sps->maxX - sps->minX;
    float multOffsetY = (sps->minY * sps->maxX - sps->maxY * sps->minX);

    return (offsetX != 0) ? multOffsetY / offsetX : 0;
}
bool readSps::checkSeg(float x1, float y1, float x2, float y2, float  x,float  y){
    int minX = 0;
    int maxX = 0;
    int minY = 0;
    int maxY = 0;

    // Находим граничные точки
    minX =  (x1 >= x2) ? x2 : x1;
    maxX =  (x1 >= x2) ? x1 : x2;
    minY =  (y1 >= y2) ? y2 : y1;
    maxY =  (y1 >= y2) ? y1 : y2;

   // Входит ли в диапазон координат введенная точка
    if (((x >= minX) & (x <= maxX)) && ((y >= minY) & (y <= maxY))) {
        // Кордината входит в этот диапазон
        return true;
    }
    return false;
}
void readSps::intersection(STRUCK_MIN_MAX iterMInMaxS, STRUCK_MIN_MAX iterMInMaxR, float &x, float &y){

    // Определяем уравнение прямых для наших линий K1*x+d1-y=0
    /// Коэффиинт прямой Для ЛПВ
    float ks = koffK( &iterMInMaxS);
    float ds = koffD( &iterMInMaxS);

    ///  Коэффиинт прямой Для ЛПП
    float kr = koffK( &iterMInMaxR);
    float dr = koffD( &iterMInMaxR);

    // Находим общую точку пересечния данных ПРЯМЫХ
    if ((ks - kr) != 0) {
        x = (dr - ds) / (ks - kr);
        y = ks * (dr - ds) / (ks - kr) + ds;
    } else {
        // Прямые паралельны
        x = NULL;
        y = NULL;
      }

    // Проверяем на пренадлежность данным отрезкам
    if (x != NULL) {
        if ( checkSeg(iterMInMaxS.minX, iterMInMaxS.minY, iterMInMaxS.maxX, iterMInMaxS.maxY, x, y) &&
             checkSeg(iterMInMaxR.minX, iterMInMaxR.minY, iterMInMaxR.maxX, iterMInMaxR.maxY, x, y)){
            // Точка пренадлежит данным отрезкам

        } else {
            // точка не принадлежит данным отрезкам
            x = NULL;
            y = NULL;
        }
    }
}
void readSps::saveTableToMapinfo( QList<STRUCK_MIN_MAX> * ssps, QList<STRUCK_MIN_MAX> *rsps ){
    ///Диагональ ромба
    float const ROMB_D = 10.6066;
    QList<QString> listRombMif;
    QList<QString> listRombMid;

    QString str ="";
    // Координаты пересчения
    float x = 0;
    float y = 0;

    QList<STRUCK_MIN_MAX>::iterator iterMInMaxS, iterMInMaxR;
    iterMInMaxS = ssps->begin();
    iterMInMaxR = rsps->begin();

    //Внешний цикл по всем линиям ЛПВ
    while(iterMInMaxS != ssps->end()){
        // Внутренний цикл по всем линиям KGG
        while (iterMInMaxR != rsps->end() ){

            // Находим точки перечечения между линиями
            intersection( *iterMInMaxS , *iterMInMaxR, x, y);
            if (x != NULL) {
                // Нашли общую точку пересечения отрезков
                // вормируем площадку с нужными координами в в иде ромба
                listRombMif.append("Region  1");
                listRombMif.append("5");
                // вычисляем координаты углов ромба
                // Левая координата
                str = QString::number(x - ROMB_D, 'f', 2) + " " + QString::number(y, 'f', 2);
                listRombMif.append(str);
                // Верхяння координата
                str = QString::number(x , 'f', 2) + " " + QString::number(y + ROMB_D, 'f', 2);
                listRombMif.append(str);
                // Правая координта
                str = QString::number(x + ROMB_D, 'f', 2) + " " + QString::number(y, 'f', 2);
                listRombMif.append(str);
                // Нижняя координта
                str = QString::number(x, 'f', 2) + " " + QString::number(y - ROMB_D, 'f', 2);
                listRombMif.append(str);
                // Конечная координта совпадает с левой координтой
                str = QString::number(x - ROMB_D, 'f', 2) + " " + QString::number(y, 'f', 2);
                listRombMif.append(str);
                // Добавляем параметры кисти и цвета
                listRombMif.append("   Pen (1,2,0)");
                listRombMif.append("   Brush (2,16777215,16777215)");
                // Добавляем координаты центра
                listRombMif.append("   Center " +  QString::number(x, 'f', 2) + " " + QString::number(y, 'f', 2));

                // Создаем файл ромбов в формате МИД
                listRombMid.append("....");
            }
            iterMInMaxR++;
        }
        iterMInMaxS++;
    }

    //сохраняем в формате мапинфо в файле
    importListToFile(&listRombMif, "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\AreaRobm.mif" );
    importListToFile(&listRombMid, "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\AreaRobm.mid" );
}


