/*
 * Класс в котором реализованы типы данных которые необходимо использовать при
 * обработки СПС файлов
*/


#ifndef DATASPS_H
#define DATASPS_H
#include<QList>


class dataSps
{
public:
    dataSps();
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
        double k = -1;
        double d = -1;
    };

    //Структура в которой будем хранить спс файл общая структура
    QList<STRUCK_SPS> spsFile;

    //Структура в которой будем хранить минимальный и максимальный пикет для каждой линии
    QList<STRUCK_MIN_MAX> spsMinMax;
};

#endif // DATASPS_H
