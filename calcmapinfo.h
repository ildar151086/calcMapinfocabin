/*
 * Класс для вычислений и сохранений в формате Мапинфо линий
 * точек, площадок, сединение точек смещений.
*/

#ifndef CALCMAPINFO_H
#define CALCMAPINFO_H

#include <QMainWindow>
#include <QString>
#include <datasps.h>

class calcMapinfo
{

private:

    ///  На сколько необходимо удленять линии ПВ
    double dLpv = -1;

    ///  На сколько необходимо удленять линии ПП
    double dLpp = -1;

    /// размер диаганаля ромба для плошадки
    double dRomb = -1;

    ///Структура в которой будем хранить коэффициенты уравнения прямых которые проверяеться на общую точку пересечения
    struct CoefLine {
        double ks = 0;
        double ds = 0;
        double kr = 0;
        double dr = 0;
    };

    ///Структура в которой будем хранить координаты пересечнения окружности с ЛПП и ЛПВ
    struct IntersecCircl {
        double x1 = 0;
        double x2 = 0;
        double x3 = 0;
        double x4 = 0;
        double y1 = 0;
        double y2 = 0;
        double y3 = 0;
        double y4 = 0;
    };

    /// Определяет точку пересчение прямых если какая либо линия перпендикулярна оси Х и У
    void ortogonalLineXY(dataSps::STRUCK_MIN_MAX,dataSps::STRUCK_MIN_MAX, CoefLine*, double&,double&);

    /// Присваеваем нулевые координаты
    void zeroXY (double&,double&);

    ///
    /// \brief Определяем принадлежит ли точка с координаи=тами х и у данному отрезку
    /// \return Да или нет
    ///
    bool checkSeg(dataSps::STRUCK_MIN_MAX sps,float,float);

    /// Вывод списка строка в файл с заданным именем
    void importListToFile(QList<QString>*, QString rootNameFile);

    /// Ищем точки пересечения указаных линий и возврашаем х и у пересечения
    void intersection(dataSps::STRUCK_MIN_MAX ,dataSps::STRUCK_MIN_MAX , double&, double&);

    /// Определяем коэфициен пропорциональнсоти прямой
    double koffK(dataSps::STRUCK_MIN_MAX );

    /// Определяем свободный коэффициент уравнения прямой
    double koffD(dataSps::STRUCK_MIN_MAX );

    /// Вычисляем общие координаты пересечения уровнения окружности и прямой


    /// Увеличть длину профилей ЛПП и ЛПВ
    void increaseLength(dataSps::STRUCK_MIN_MAX  , double &, double &, double);

    /// Для определение вершин ромба определяем общие границы окружности с радиусом ромба с ЛПВ и ЛПП
    void intersectionCircle(dataSps::STRUCK_MIN_MAX ,dataSps::STRUCK_MIN_MAX , double , IntersecCircl *, double , double);

    ///Структура в которой будем хранить спс файл общая структура
    QList<dataSps::STRUCK_SPS> spsFile;

    ///Структура в которой будем хранить минимальный и максимальный пикет для каждой линии
    QList<dataSps::STRUCK_MIN_MAX> spsMinMax;

public:
    ///Конструктора
    calcMapinfo(double, double, double);
    calcMapinfo();

    /// Задаем значение удлинения линий ЛПВ
    void set_dLpv (double);

    /// Задаем значение удлинения линий ЛПП
    void set_dLpp (double);

    /// Задаем значение диагонали ромба
    void set_dRomb (double);

    /// Берем значение удлинения линий ЛПВ
    double get_dLpv ();

    /// Берем значение удлинения линий ЛПП
    double get_dLpp ();

    /// Берем значение диагонали ромба
    double get_dRomb ();

    ///
    /// \brief сохранить точки в формате мапинфо
    /// \param Какой файл обрабатываем
    ///
    void savePointToMapinfo(QList<dataSps::STRUCK_SPS> *, QString simbol);

    /// сохраните в линии в формте мапиинфо
    void saveLineToMapinfo(QList<dataSps::STRUCK_MIN_MAX> , QString);

    /// сохраните площадки с сзаданным размером на пересечениях в формте мапиинфо
    void saveTableToMapinfo( QList<dataSps::STRUCK_MIN_MAX> *, QList<dataSps::STRUCK_MIN_MAX> *);

    /// Соединение полигона линий смещенных ПВ в формате полигона Мапинфо
    void savePolyLineToMapinfo( QList<dataSps::STRUCK_SPS> *, QString );

    /// Сохранение выбранных линий с заданной шириной линии в формате мапинфо
};

#endif // CALCMAPINFO_H
