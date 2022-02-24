#ifndef READSPS_H
#define READSPS_H

#include <QMainWindow>
#include <QString>
///
/// \brief Класс для работы с спс файлами, загрзка и хранения
/// данных S и R sps файла
///
class readSps
{
    /// Вывод списка строка в файл с заданным именем
   void importListToFile(QList<QString>*, QString rootNameFile);

   ///
   /// \brief Определяем принадлежит ли точка с координаи=тами х и у данному отрезку
   /// \return Да или нет
   ///
   bool checkSeg(float,float,float,float,float,float);

    public:
        readSps();
    ///
    /// \brief Структура по хранению данных из спс
    ///
    struct STRUCK_SPS {
        int line = -1;   // int
        int picket = -1; // int
        float x = -1; //	float
        float y = -1; //	float
        float z = -1; //	float
    } tempSps;

    ///
    /// \brief Поиск минимального и минимального пикета для каждой линии
    ///
    struct STRUCK_MIN_MAX {
        int line;
        int minPicket = -1;
        int maxPicket = -1;
        float minX = -1;
        float minY = -1;
        float maxX = -1;
        float maxY = -1;
    };

    //Структура в которой будем хранить спс файл общая структура
    QList<STRUCK_SPS> sps;

    //Структура в которой будем хранить минимальный и максимальный пикет для каждой линии
    QList<STRUCK_MIN_MAX> spsMinMax;

    ///
    /// \brief Чтение SPS файла в список
    /// \param задаем путь к файлу
    ///
    void readSPSfile(QString namefile, QList<QString> &readFile);

    ///
    /// \brief Проверяет чтобы список был не пустой
    /// \param Список сырой
    /// \return Возврашает список заполнен 1 или нет 0
    ///
    bool listFileCheck(QList<QString> &readFile);


    ///
    /// \brief Переводим из строки в позиции структуры
    /// \param строка из нашего спс фала для перевода
    /// \return Возвращаем структуру для данной строки
    ///
    STRUCK_SPS transStr(QString, int);

    ///
    /// \brief Определяем формат спс файла
    /// \param namefile
    /// \param simbol
    ///
    void formatSPS(QString namefile, QString simbol = " ");

    ///
    /// \brief Определяем макисмальное и минимальное значение линии
    /// \param Список по спс файлу
    ///
    void findMinMaxLinePicket(QList<STRUCK_SPS> *);

    /// сохранить точки в формате мапинфо
    void savePointToMapinfo(QList<STRUCK_SPS> *, QString simbol);

    /// сохраните в линии в формте мапиинфо
    void saveLineToMapinfo( QString simbol );

    /// сохраните площадки с сзаданным размером на пересечениях в формте мапиинфо
    void saveTableToMapinfo( QList<STRUCK_MIN_MAX> *, QList<STRUCK_MIN_MAX> *);

    /// Ищем точки пересечения указаных линий и возврашаем х и у пересечения
    void intersection(STRUCK_MIN_MAX ,STRUCK_MIN_MAX , float&, float&);

    /// Определяем коэфициен пропорциональнсоти прямой
    float koffK(STRUCK_MIN_MAX *);

    /// Определяем свободный коэффициент уравнения прямой
    float koffD(STRUCK_MIN_MAX *);

    /// Увеличть длину профилей ЛПП и ЛПВ

    /// Сохранение выбранных линий с заданной шириной линии в формате мапинфо
};



#endif // READSPS_H
