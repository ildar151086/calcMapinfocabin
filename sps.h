/*
    Класс отвечает за загрузку и обработку спс файлов.
    Создает структуру соглавно спс фалу.
    Находит макисмальные и минимальные пикеты каждой линии
*/

#ifndef SPS_H
#define SPS_H

#include <QMainWindow>
#include <QString>
#include <datasps.h>

///
/// \brief Класс для работы с спс файлами, загрзка и хранения
/// данных S и R sps файла. Нахождение макисмальных и минимальных пикетов в каждой линии
///
class sps
{
    ///
    /// \brief Переводим из строки в позиции структуры
    /// \param строка из нашего спс фала для перевода
    /// \return Возвращаем структуру для данной строки
    ///
    dataSps::STRUCK_SPS transStr(QString, int);

    ///
    /// \brief Определяем макисмальное и минимальное значение линии
    /// \param Список по спс файлу
    ///
    void findMinMaxLinePicket(QList<dataSps::STRUCK_SPS> &);

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

    /// Версия SPS фалов
    int versionSps = -1;    

public:
    /// Конструктор
    sps(int);
    sps();

    /// Задаем значение версии спс фалов
    void set_versionSps (int);

    /// Берем значение версии спс фалов
    double get_versionSps ();

    ///Структура в которой будем хранить спс файл общая структура
    QList<dataSps::STRUCK_SPS> spsFile;

    ///Структура в которой будем хранить минимальный и максимальный пикет для каждой линии
    QList<dataSps::STRUCK_MIN_MAX> spsMinMax;

    ///
    /// \brief Определяем формат спс файла
    /// \param namefile
    /// \param simbol
    ///
    void formatSPS(QString namefile, QString simbol = " ");

};

#endif // SPS_H
