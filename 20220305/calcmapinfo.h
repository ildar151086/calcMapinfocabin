#ifndef CALCMAPINFO_H
#define CALCMAPINFO_H

#include <QMainWindow>
#include <QString>

/*
 * Класс для вычислений и сохранений в формате Мапинфо линий
 * точек, площадок, сединение точек смещений.
*/
class calcMapinfo
{

protected:

    ///  На сколько необходимо удленять линии ПВ
    double dLpv = -1;
    ///  На сколько необходимо удленять линии ПП
    double dLpp = -1;
    /// размер диаганаля ромба для плошадки
    double dRomb = -1;

public:

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

    /// Вывод списка строка в файл с заданным именем
   void importListToFile(QList<QString>*, QString rootNameFile);

   ///
   /// \brief Определяем принадлежит ли точка с координаи=тами х и у данному отрезку
   /// \return Да или нет
   ///
   bool checkSeg(float,float,float,float,float,float);

    public:
        calcMapinfo(double, double, double);
        calcMapinfo();
    ///
    /// \brief Структура по хранению данных из спс
    ///
    struct STRUCK_SPS {
        int line = -1;   // int
        int picket = -1; // int
        double x = -1; //	float
        double y = -1; //	float
        double z = -1; //	float
    } tempSps;

    ///
    /// \brief Поиск минимального и минимального пикета для каждой линии
    ///
    struct STRUCK_MIN_MAX {
        int line;
        int minPicket = -1;
        int maxPicket = -1;
        double minX = -1;
        double minY = -1;
        double maxX = -1;
        double maxY = -1;
    };

    //Структура в которой будем хранить спс файл общая структура
    QList<STRUCK_SPS> spsFile;

    //Структура в которой будем хранить минимальный и максимальный пикет для каждой линии
    QList<STRUCK_MIN_MAX> spsMinMax;

    /// сохранить точки в формате мапинфо
    void savePointToMapinfo(QList<STRUCK_SPS> *, QString simbol);

    /// сохраните в линии в формте мапиинфо
    void saveLineToMapinfo( QString simbol );

    /// сохраните площадки с сзаданным размером на пересечениях в формте мапиинфо
    void saveTableToMapinfo( QList<STRUCK_MIN_MAX> *, QList<STRUCK_MIN_MAX> *);

    /// Ищем точки пересечения указаных линий и возврашаем х и у пересечения
    void intersection(STRUCK_MIN_MAX ,STRUCK_MIN_MAX , double&, double&);

    /// Определяем коэфициен пропорциональнсоти прямой
    double koffK(STRUCK_MIN_MAX );

    /// Определяем свободный коэффициент уравнения прямой
    double koffD(STRUCK_MIN_MAX );

    /// Увеличть длину профилей ЛПП и ЛПВ
    void increaseLength(STRUCK_MIN_MAX , double &, double &, double);

    /// Соединение полигона линий смещенных ПВ в формате полигона Мапинфо
    void savePolyLineToMapinfo( QString simbol );


    /// Сохранение выбранных линий с заданной шириной линии в формате мапинфо
};

#endif // CALCMAPINFO_H
