/*
 * Класс в котором будем хранить шаблоны шапок для разных типов файлов Мапинфо.
 * Каждая шапка будет храниться в списке стринг
*/

#ifndef HEADERMAPINFOFILE_H
#define HEADERMAPINFOFILE_H
#include<QList>

class HeaderMapinfoFile
{

public:
    HeaderMapinfoFile();

    ///Получение шапки мапинфо для точечного объекта
    QList<QString> getHeaderPointForMap();

    ///Получение шапки мапинфо для линейного объекта
    QList<QString> getHeaderLineForMap();

    ///Получение шапки мапинфо для площадного объекта
    QList<QString> getHeaderAreaForMap();

};

#endif // HEADERMAPINFOFILE_H
