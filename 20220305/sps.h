#ifndef SPS_H
#define SPS_H

#include <QMainWindow>
#include <QString>
///
/// \brief Класс для работы с спс файлами, загрзка и хранения
/// данных S и R sps файла. Нахождение макисмальных и минимальных пикетов в каждой линии
///
class sps
{
protected:
    /// Версия SPS фалов
    int versionSps = -1;

public:

    /// Задаем значение версии спс фалов
    void set_versionSps (int);

    /// Берем значение версии спс фалов
    double get_versionSps ();

    public:
        sps(int);
        sps();
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
    void findMinMaxLinePicket(QList<STRUCK_SPS> &);

};

#endif // SPS_H
