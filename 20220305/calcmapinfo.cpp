#include "calcmapinfo.h"
#include "readsps.h"
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QSet>
#include <QTextStream>
#include <QtMath>

calcMapinfo::calcMapinfo(){
    set_dLpv (25);
    set_dLpp (25);
    set_dRomb (10.6066);
}
calcMapinfo::calcMapinfo( double userdLpv, double userdLpp, double userdRomb){
    set_dLpv (userdLpv);
    set_dLpp (userdLpp);
    set_dRomb (userdRomb);
}

inline void calcMapinfo::set_dLpv (double value){
    if ( value >= 0 ) {
        //если введено неправильное число
        dLpv = value;
    } else {
        dLpv = -1;
    }
}
inline void calcMapinfo::set_dLpp(double value){
    if ( value >= 0 ) {
        //если введено неправильное число
        dLpp = value;
    } else {
        dLpp = -1;
    }
}
inline void calcMapinfo::set_dRomb(double value){
    if ( value >= 0 ) {
        //если введено неправильное число
        dRomb = value;
    } else {
        dRomb = -1;
    }
}

inline double calcMapinfo::get_dLpv (){
    return dLpv;
}
inline double calcMapinfo::get_dLpp (){
    return dLpp;
}
inline double calcMapinfo::get_dRomb (){
    return dRomb;
}

void calcMapinfo::importListToFile(QList<QString>* listString, QString rootNameFile){
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
void calcMapinfo::savePointToMapinfo(QList<STRUCK_SPS> *sps , QString simbol){
    // Сохранем введенную структур в фформате Мапинфо и поместим ее в список
    QList<QString> listPointMapMif, listPointMapMid;
    QString strMif, strMid;

    // Итератор для списка
    QList<STRUCK_SPS>::iterator iterList;
    iterList = sps->begin();

    // Пробегаем по всему списку структры заданного спс файла
    while(iterList != sps->end()){
        // Преобразуем координаты пикеты в строки в формате МИФ
        strMif = "Point " + QString::number(iterList->x,'d', 2) + " " + QString::number(iterList->y,'d', 2);
        listPointMapMif.append(strMif);
        // Добавляем фразу что это символ
        strMif = "Symbol (38,0,12)";
        listPointMapMif.append(strMif);

        // Преобразуем линии и пикеты и координаты пикеты в строки в формате МИД
        strMid  = QString::number(iterList->line, 10) + "," + QString::number(iterList->picket, 10) + ",";
        strMid += QString::number(iterList->x,'d', 2) + "," + QString::number(iterList->y,'d', 2);
        listPointMapMid.append(strMid);

        iterList++;
    }

    // Сохраняем полученный список  в файл
    importListToFile(&listPointMapMif, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simbol+ "_point.mif");
    importListToFile(&listPointMapMid, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simbol+ "_point.mid");

}
void calcMapinfo::increaseLength(STRUCK_MIN_MAX iterMInMax, double &dlХ, double &dlY, double dOffset){
    /// Угол при перпендикулярном расположениии ЛПП и ЛПВ в радианах
    int const ANGLE_ZERO_RAD = 1.57;

    // Находим угол наклона прямой
    double dX = (iterMInMax.minX - iterMInMax.maxX);
    double dY = (iterMInMax.minY - iterMInMax.maxY);
    double angle = (dX != 0) ? qAtan( dY / dX ) : ANGLE_ZERO_RAD;

    //Определяем направление отрезка
    double ks = koffK( iterMInMax);

    // Расчет увеличение длины линии ЛПВ и ЛПП на заданую длину
    dlХ = (ks > 0) ? dOffset * qCos( angle ) : - dOffset * qCos( angle );
    dlY = (ks > 0) ? dOffset * qSin( angle ) : - dOffset * qSin( angle );
}
void calcMapinfo::saveLineToMapinfo(QString simbol){
    QString str;
    QList<QString> listLineMapMif,listLineMapMid;
    // Пробигаемяс по всему списку структуры максимальных и минимальных значений
    QList<STRUCK_MIN_MAX>::iterator iterMInMax;
    iterMInMax = spsMinMax.begin();

    // Расчет увеличение длины линии ЛПВ/ЛПП на заданую длину
    double dlХ = 0;
    double dlY = 0;

    if (simbol == "S"){
        increaseLength(*iterMInMax, dlХ, dlY, get_dLpv());
    } else {
        increaseLength(*iterMInMax, dlХ, dlY, get_dLpp());
    }

    while (iterMInMax != spsMinMax.end()){
        // Создаем файл для Линий в формате МИФ
        str  = "Line " + QString::number( iterMInMax->minX - dlХ, 'd', 2) + " "
                       + QString::number( iterMInMax->minY - dlY, 'd', 2);
        str += " " + QString::number( iterMInMax->maxX + dlХ, 'd', 2) + " "
                   + QString::number( iterMInMax->maxY + dlY, 'd', 2);
        listLineMapMif.append(str);
        str = "Pen (4,2,16711680)";
        listLineMapMif.append(str);

        // Создаем файл для Линий в формате МИД
        listLineMapMid.append("....");

        iterMInMax++;
    }

    importListToFile(&listLineMapMif, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simbol+ "_line.mif" );
    importListToFile(&listLineMapMid, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simbol+ "_line.mid" );

}
void calcMapinfo::savePolyLineToMapinfo( QString simbol ){
     // Будем считать что список по СПС файлам отсортирован
    QString str;
    QList<QString> listPolyLineMapMif, listPolyLineMapMid;

    // Пробигаемяс по всему списку спс
    QList<STRUCK_SPS>::Iterator iterSps;
    iterSps = spsFile.begin();

    while (iterSps != spsFile.end()){
        // Создаем файл для Линий в формате МИФ
        str  = "Line " + QString::number( iterSps->x, 'd', 2) + " "
                       + QString::number( iterSps->y, 'd', 2);
        int lineFirst = iterSps->line;

        // для получения следующей точки
        iterSps++;

        if (iterSps == spsFile.end()) {
            // Если конец списка то выходим из цикла
            break;
        }
        int lineSecond = iterSps->line;

        // Проверяем линия таже или другая
        if (lineFirst ==  lineSecond) {
            str  +=" "     + QString::number( iterSps->x, 'd', 2) + " "
                           + QString::number( iterSps->y, 'd', 2);
            listPolyLineMapMif.append(str);
            str = "Pen (4,2,16711680)";
            // Создаем файл для Линий в формате МИФ
            listPolyLineMapMif.append(str);
            // Создаем файл для Линий в формате МИД
            listPolyLineMapMid.append("....");
        }
    }

    importListToFile(&listPolyLineMapMif, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simbol+ "_polyLine.mif" );
    importListToFile(&listPolyLineMapMid, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simbol+ "_polyLine.mid" );
}
inline double calcMapinfo::koffK(STRUCK_MIN_MAX sps ) {

    // Находим смещения по оси х и у
    double offsetX = sps.maxX - sps.minX;
    double offsetY = sps.maxY - sps.minY;

    return (offsetX != 0) ? offsetY / offsetX : 0;
}
inline double calcMapinfo::koffD(STRUCK_MIN_MAX sps ) {
    // Находим смещения по оси х и у
    double offsetX = sps.maxX - sps.minX;
    double multOffsetY = (sps.minY * sps.maxX - sps.maxY * sps.minX);

    return (offsetX != 0) ? multOffsetY / offsetX : 0;
}
bool calcMapinfo::checkSeg(float x1, float y1, float x2, float y2, float  x,float  y){
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
void calcMapinfo::intersection(STRUCK_MIN_MAX iterMInMaxS, STRUCK_MIN_MAX iterMInMaxR, double &x, double &y){
    // Определяем уравнение прямых для наших линий K1*x+d1-y=0
    /// Коэффиинт прямой Для ЛПВ
    double ks = koffK( iterMInMaxS);
    double ds = koffD( iterMInMaxS);

    ///  Коэффиинт прямой Для ЛПП
    double kr = koffK( iterMInMaxR);
    double dr = koffD( iterMInMaxR);

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
void calcMapinfo::saveTableToMapinfo( QList<STRUCK_MIN_MAX> * ssps, QList<STRUCK_MIN_MAX> *rsps ){

    QList<QString> listRombMif;
    QList<QString> listRombMid;

    QString str ="";
    // Координаты пересчения
    double x = 0;
    double y = 0;

    QList<STRUCK_MIN_MAX>::iterator iterMInMaxS, iterMInMaxR;
    iterMInMaxS = ssps->begin();

    //Внешний цикл по всем линиям ЛПВ
    while(iterMInMaxS != ssps->end()){
        // Внутренний цикл по всем линиям KGG
        iterMInMaxR = rsps->begin();
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
                str = QString::number(x - get_dRomb(), 'd', 2) + " " + QString::number(y, 'd', 2);
                listRombMif.append(str);
                // Верхяння координата
                str = QString::number(x , 'd', 2) + " " + QString::number(y + get_dRomb(), 'd', 2);
                listRombMif.append(str);
                // Правая координта
                str = QString::number(x + get_dRomb(), 'd', 2) + " " + QString::number(y, 'd', 2);
                listRombMif.append(str);
                // Нижняя координта
                str = QString::number(x, 'd', 2) + " " + QString::number(y - get_dRomb(), 'd', 2);
                listRombMif.append(str);
                // Конечная координта совпадает с левой координтой
                str = QString::number(x - get_dRomb(), 'd', 2) + " " + QString::number(y, 'd', 2);
                listRombMif.append(str);
                // Добавляем параметры кисти и цвета
                listRombMif.append("   Pen (1,2,0)");
                listRombMif.append("   Brush (2,16777215,16777215)");
                // Добавляем координаты центра
                listRombMif.append("   Center " +  QString::number(x, 'd', 2) + " " + QString::number(y, 'd', 2));

                // Создаем файл ромбов в формате МИД
                listRombMid.append("....");
            }
            iterMInMaxR++;
        }
        iterMInMaxS++;
    }

    //сохраняем в формате мапинфо в файле
    importListToFile(&listRombMif, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\AreaRobm.mif" );
    importListToFile(&listRombMid, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\AreaRobm.mid" );
}


