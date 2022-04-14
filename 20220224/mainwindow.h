#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H


///
/// \brief Класс для работы с спс файлами, загрзка и хранения
/// данных S и R sps файла
///
class readSps
{
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



    /// Определяем формат спс файла
    void formatSPS(QString namefile, QString simbol = " ");

    /// минимального и максимального элемента
    void findMinMaxLinePicket(QList<STRUCK_SPS> *);

    // сохранить точки в формате мапинфо

    // сохраните в линии в формте мапиинфо
    // Сохранение выбранных линий с заданной шириной линии в формате мапинфо
};
