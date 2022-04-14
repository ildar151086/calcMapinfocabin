#include "calcmapinfo.h"
#include "readsps.h"
#include <QList>
#include <QString>
#include <QFile>
#include <QMessageBox>
#include <QSet>
#include <QTextStream>
#include <QtMath>
#include <datasps.h>
#include <headermapinfofile.h>
#include <QDebug>
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
void calcMapinfo::savePointToMapinfo(QList<dataSps::STRUCK_SPS> *sps , QString simbol){
    // Сохранем введенную структур в фформате Мапинфо и поместим ее в список
    QList<QString> listPointMapMif, listPointMapMid;
    QString strMif, strMid;

    // Вставляем шапку
    HeaderMapinfoFile head;
    listPointMapMif.append(head.getHeaderPointForMap());

    // Итератор для списка
    QList<dataSps::STRUCK_SPS>::iterator iterList;
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
void calcMapinfo::increaseLength(dataSps::STRUCK_MIN_MAX iterMInMax, double &dlХ, double &dlY, double dOffset){
    /// Угол при перпендикулярном расположениии ЛПП и ЛПВ в радианах
    double const ANGLE_ZERO_RAD = M_PI_2;

    // Находим угол наклона прямой
    double dX = (iterMInMax.minX - iterMInMax.maxX);
    double dY = (iterMInMax.minY - iterMInMax.maxY);
    double angle = (dX != 0) ? qAtan( dY / dX ) : ANGLE_ZERO_RAD;

     qDebug() << " dX " <<  dX;
     qDebug() << " dY " <<  dY;
     qDebug() << " angle " <<  angle;

    //Определяем направление отрезка
    double ks = koffK(iterMInMax);
    qDebug() << " ks " <<  ks;


    // Расчет увеличение длины линии ЛПВ и ЛПП на заданую длину
    dlХ = (ks >= 0) ? dOffset * qCos( angle ) : - dOffset * qCos( angle );
    dlY = (ks >= 0) ? dOffset * qSin( angle ) : - dOffset * qSin( angle );

    qDebug() << " dlХ " <<  dlХ;
    qDebug() << " dlY " <<  dlY;
}

void calcMapinfo::intersectionCircle(dataSps::STRUCK_MIN_MAX iterMInMaxS, dataSps::STRUCK_MIN_MAX iterMInMaxR,
                                     double radius, calcMapinfo::IntersecCircl *generalXY, double centerX0, double centerY0)
{
    // Блок лямд выражений вычисляющих коэффициенты а,и,с в уравнинии квдратном
    auto lambdaA = [](double k)                                 { return   1 + k * k;                           };
    auto lambdaB = [](double x0, double y0, double k, double d) { return  2 * ( - x0 + k * (d - y0));                };
    auto lambdaC = [](double x0, double y0, double d, double r) { return  (x0 * x0) + (d - y0) * (d - y0) - (r * r);  };

    //определяем коэффициенты пямой ЛПВ
    double ks = koffK( iterMInMaxS);
    //определяем свободный коэффициент ЛПВ
    double ds = koffD( iterMInMaxS);

    //определяем коэффициенты пямой ЛПП
    double kr = koffK( iterMInMaxR);
    //определяем свободный коэффициент ЛПП
    double dr = koffD( iterMInMaxR);

    // Исчим координаты пересечнния с окружность с линией ЛПВ
    double a = lambdaA(ks);
    double b = lambdaB(centerX0, centerY0, ks, ds);
    double c = lambdaC(centerX0, centerY0, ds, radius);
    double detrminant = b * b - 4 * a * c;
    if (detrminant > 0) {
        // Если есть решение данного уровнеия
        generalXY->x1 = (- b + sqrt(detrminant)) / (2 * a);
        generalXY->x2 = (- b - sqrt(detrminant)) / (2 * a);
    }
    // Находим коорднат по оси ординат
    generalXY->y1 = ks * generalXY->x1 + ds;
    generalXY->y2 = ks * generalXY->x2 + ds;

    // Исчим координаты пересечнния с окружность с линией ЛПП
    a = lambdaA(kr);
    b = lambdaB(centerX0, centerY0, kr, dr);
    c = lambdaC(centerX0, centerY0, dr, radius);
    detrminant = b * b - 4 * a * c;
    if (detrminant > 0) {
        // Если есть решение данного уровнеия
        generalXY->x3 = (- b + sqrt(detrminant)) / (2 * a);
        generalXY->x4 = (- b - sqrt(detrminant)) / (2 * a);
    }
    // Находим коорднат по оси ординат
    generalXY->y3 = kr * generalXY->x3 + dr;
    generalXY->y4 = kr * generalXY->x4 + dr;

}
void calcMapinfo::saveLineToMapinfo(QList<dataSps::STRUCK_MIN_MAX> MinMax, QString simbol){
    QString str;
    QList<QString> listLineMapMif,listLineMapMid;

    // Вставляем шапку
    HeaderMapinfoFile head;
    listLineMapMif.append(head.getHeaderLineForMap());

    // Пробигаемяс по всему списку структуры максимальных и минимальных значений
    QList<dataSps::STRUCK_MIN_MAX>::iterator iterMInMax;
    iterMInMax = MinMax.begin();

    // Расчет увеличение длины линии ЛПВ/ЛПП на заданую длину
    double dlХ = 0;
    double dlY = 0;

    if (simbol == "S"){
        increaseLength(*iterMInMax, dlХ, dlY, get_dLpv());
    } else {
        increaseLength(*iterMInMax, dlХ, dlY, get_dLpp());
    }

    qDebug() << " dlХ " << dlХ;
    qDebug() << " dlY " << dlY;

    while (iterMInMax != MinMax.end()){
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
void calcMapinfo::savePolyLineToMapinfo( QList<dataSps::STRUCK_SPS>* sps, QString simvol ){
     // Будем считать что список по СПС файлам отсортирован
    QString str;
    QList<QString> listPolyLineMapMif, listPolyLineMapMid;

    // Вставляем шапку
    HeaderMapinfoFile head;
    listPolyLineMapMif.append(head.getHeaderLineForMap());

    // Пробигаемяс по всему списку спс
    QList<dataSps::STRUCK_SPS>::Iterator iterSps;
    iterSps = sps->begin();

    while (iterSps != sps->end()){
        // Создаем файл для Линий в формате МИФ
        str  = "Line " + QString::number( iterSps->x, 'd', 2) + " "
                       + QString::number( iterSps->y, 'd', 2);
        int lineFirst = iterSps->line;

        // для получения следующей точки
        iterSps++;

        if (iterSps == sps->end()) {
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

    importListToFile(&listPolyLineMapMif, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simvol+ "_LineEdit.mif" );
    importListToFile(&listPolyLineMapMid, "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\" + simvol+ "_LineEdit.mid" );
}
inline double calcMapinfo::koffK(dataSps::STRUCK_MIN_MAX sps ) {

    // Находим смещения по оси х и у
    double offsetX = sps.maxX - sps.minX;
    qDebug() << " offsetX " << offsetX;

    double offsetY = sps.maxY - sps.minY;
    qDebug() << " offsetY " << offsetY;

    return (offsetX != 0) ? offsetY / offsetX : 0;
}
inline double calcMapinfo::koffD(dataSps::STRUCK_MIN_MAX sps ) {
    // Находим смещения по оси х и у
    double offsetX = sps.maxX - sps.minX;
    qDebug() << " offsetX " << offsetX;

    double multOffsetY = (sps.minY * sps.maxX - sps.maxY * sps.minX);
    qDebug() << " multOffsetY " << multOffsetY;

    return (offsetX != 0) ? multOffsetY / offsetX : 0;
}


inline bool calcMapinfo::checkSeg(dataSps::STRUCK_MIN_MAX sps, float  x,float  y){
    double x1 = sps.minX;
    double y1 = sps.minY;
    double x2 = sps.maxX;
    double y2 = sps.maxY;

    double  minX = 0;
    double  maxX = 0;
    double  minY = 0;
    double  maxY = 0;

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
inline void calcMapinfo::zeroXY (double &x, double &y){
    x = NULL;
    y = NULL;
}
void calcMapinfo::ortogonalLineXY(dataSps::STRUCK_MIN_MAX iterMInMaxS, dataSps::STRUCK_MIN_MAX iterMInMaxR,
                                  CoefLine *coefLine, double &x, double &y){
    // Проверка если прямая ЛПВ перпендикулярна оси ОУ  ks = 0 ds = 0;
    if (coefLine->ks == 0 && coefLine->ds == 0){
        // Проверка если прямая ЛПП перпендикулярна оси ОУ. kr = 0 dr = 0;
        if (coefLine->kr == 0 && coefLine->dr == 0){
            // линии ЛПП и ЛПВ паралельны
            zeroXY ( x, y );
        }
        // Проверка если прямая перпендикулярна оси ОХ  kr = 0 dr != 0
        if (coefLine->kr == 0 && coefLine->dr != 0){
            x = iterMInMaxS.minX;
            y = iterMInMaxR.minY;
        }
    }

    // Проверка если прямая перпендикулярна оси ОХ  ks = 0 ds != 0
    if (coefLine->ks == 0 && coefLine->ds != 0){
        // Проверка если прямая ЛПП перпендикулярна оси ОУ. kr = 0 dr = 0;
        if (coefLine->kr == 0 && coefLine->dr == 0){
            // линии ЛПП и ЛПВ паралельны
            y = iterMInMaxS.minY;
            x = iterMInMaxR.minX;
        }
        // Проверка если прямая перпендикулярна оси ОХ  kr = 0 dr != 0
        if (coefLine->kr == 0 && coefLine->dr != 0){
            zeroXY ( x, y );
        }
    }
}
void calcMapinfo::intersection(dataSps::STRUCK_MIN_MAX iterMInMaxS, dataSps::STRUCK_MIN_MAX iterMInMaxR,
                               double &x, double &y){
    CoefLine coefLine;
    // Определяем уравнение прямых для наших линий K1*x+d1-y=0
    /// Коэффиинт прямой Для ЛПВ
    coefLine.ks = koffK( iterMInMaxS );
    coefLine.ds = koffD( iterMInMaxS );
    qDebug() << " Коэффиинт прямой Для ЛПВ ";
    qDebug() << " ks " << coefLine.ks;
    qDebug() << " ds " << coefLine.ds;

    ///  Коэффиинт прямой Для ЛПП
    coefLine.kr = koffK( iterMInMaxR );
    coefLine.dr = koffD( iterMInMaxR );
    qDebug() << " Коэффиинт прямой Для ЛПП ";
    qDebug() << " kr " << coefLine.kr;
    qDebug() << " dr " << coefLine.dr;

    // Находим общую точку пересечния данных ПРЯМЫХ
    if ((coefLine.ks -  coefLine.kr) != 0) {
        x = (coefLine.dr - coefLine.ds) / (coefLine.ks -  coefLine.kr);
        y = coefLine.ks * (coefLine.dr - coefLine.ds) / (coefLine.ks -  coefLine.kr) + coefLine.ds;
    } else {
        // Проверяем на перпендикулярность прямых
        ortogonalLineXY(iterMInMaxS, iterMInMaxR, &coefLine, x, y);
      }

    qDebug() << "Точка пересения";
    qDebug() << " x " << x;
    qDebug() << " y " << y;

    // Проверяем на пренадлежность данным отрезкам
    if (x != NULL) {
        if ( checkSeg(iterMInMaxS, x, y) &&
             checkSeg(iterMInMaxR, x, y)){
            // Точка пренадлежит данным отрезкам
        } else {
            // точка не принадлежит данным отрезкам
            zeroXY ( x, y );
        }
    }
}
void calcMapinfo::saveTableToMapinfo( QList<dataSps::STRUCK_MIN_MAX>* ssps, QList<dataSps::STRUCK_MIN_MAX>* rsps ){

    QList<QString> listRombMif;
    QList<QString> listRombMid;

    // Вставляем шапку
    HeaderMapinfoFile head;
    listRombMif.append(head.getHeaderPointForMap());

    QString str ="";
    // Координаты пересчения
    double x = 0;
    double y = 0;

    QList<dataSps::STRUCK_MIN_MAX>::iterator iterMInMaxS, iterMInMaxR;
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

                // Вычисяем обшие координаты окружности и прямых
                IntersecCircl generalXY;
                intersectionCircle(*iterMInMaxS, *iterMInMaxR, get_dRomb(), &generalXY, x, y);


                // вормируем площадку с нужными координами в в иде ромба
                listRombMif.append("Region  1");
                listRombMif.append("5");

                // вычисляем координаты углов ромба
                // Левая координата
                str = QString::number(generalXY.x1, 'd', 2) + " " + QString::number(generalXY.y1, 'd', 2);
                listRombMif.append(str);

                // Верхяння координата
                str = QString::number(generalXY.x3 , 'd', 2) + " " + QString::number(generalXY.y3, 'd', 2);
                listRombMif.append(str);

                // Правая координта
                str = QString::number(generalXY.x2, 'd', 2) + " " + QString::number(generalXY.y2, 'd', 2);
                listRombMif.append(str);

                // Нижняя координта
                str = QString::number(generalXY.x4, 'd', 2) + " " + QString::number(generalXY.y4, 'd', 2);
                listRombMif.append(str);

                // Конечная координта совпадает с левой координтой
                str = QString::number(generalXY.x1, 'd', 2) + " " + QString::number(generalXY.y1, 'd', 2);
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


