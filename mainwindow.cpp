#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QFile>
#include <sps.h>
#include <calcmapinfo.h>
#include <QMessageBox>
#include <time.h>
#include <QTextStream>
#include <datasps.h>

/*
 * При сохранение в линии с увеличением длины то есть смещение около 20см на Р и С спс если профили находяться под углом
 * без увеличения длины все совпадает.и Совпадают когда профиля ортогональны
 * Площадки не поворачиваються если профиля под углом
*/


/// Сохраняем пути к нашим спс файлам и основные введенные параметры пользователем
namespace GLOBAL_USER {
//     Путь к нашим спс
//    QString NAME_S_SPS = "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\Проект_Терсинский_S.sps";
//    QString NAME_R_SPS = "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\Проект_Терсинский_R.sps";
//    QString NAME_S_SPS = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\Проект_Терсинский_S.sps";
//    QString NAME_R_SPS = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\Проект_Терсинский_R.sps";

//    QString NAME_S_SPS_EDIT = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Edit_Терсинский_S.sps";
//    QString NAME_S_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Проект_Терсинский_S.sps";
//    QString NAME_R_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Проект_Терсинский_R.sps";

//    QString NAME_S_SPS_EDIT = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Edit_Арласнкий_S.sps";
//    QString NAME_S_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Арласнкий_S.sps";
//    QString NAME_R_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Арласнкий_R.sps";

//    QString NAME_S_SPS_EDIT;
//    QString NAME_S_SPS        = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\S_K-Novokub_355.sps";
//    QString NAME_R_SPS        = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\R_K-Novokub_355.rps";

    QString NAME_S_SPS_EDIT = "";
    QString NAME_S_SPS      = "";
    QString NAME_R_SPS      = "";

}


///
/// \brief Выводит диалог с пользователем в котором он выбирает необходимы файл
/// \return Путь к выбранному файлу
///
QString nameSps (QString nameSPS = " ... ") {
    //Вызвать системный диалог открытия файла в домашней папке пользователя
    QString lFileName=QFileDialog::getOpenFileName( Q_NULLPTR , " Выбирите " + nameSPS,
    QDir::homePath ( ), " SPS files ( *.* );; All files ( *.* ) " );
    //указываем фильтры для просмотра файлов
    if ( lFileName.isEmpty ( ) ) {
        //Если пользователь не выбрал ни одного файла
        QMessageBox::warning ( Q_NULLPTR, " Error ",QString ("Вы не выбрали файл"), QMessageBox::Ok);
        return " "; //выйти из метода
    }
    return lFileName;
}

///
/// \brief Проверяем не пустые ли пути к нашим спс фалам
///
bool chekRoodSps (QString name, QString file){
    // Проверка заданных путей
    if (name == " ") {
        // Небыли выбраны спс
        QMessageBox::warning ( Q_NULLPTR, " Error ",QString ("Вы не выбрали файл "+file ), QMessageBox::Ok);
        return 0;
    }
    return 1;
}

///
/// \brief Вывод в текстовой файл загруженного спс файла
/// \param sps  класс в которую загружен спс файл
/// \param имя файла которое будет создан на диске
///
void outputFile(sps* sps, QString nameFile){
    dataSps::STRUCK_SPS tempsps;

    // Файл в которой будем выгружать файл
    QFile outputFile (nameFile);
    outputFile.open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream tempStream( &outputFile );
    tempStream.setRealNumberNotation(QTextStream::FixedNotation);//tempStream.setRealNumberNotation( FixedNotation );
    //tempStream.setRealNumberPrecision(2);

    // Итераор по списку
    QList<dataSps::STRUCK_SPS>::iterator iterList;
    iterList = sps->spsFile.begin();

    // Начинаем цикл по всем записям в списке
    while( iterList != sps->spsFile.end() ){
        tempsps = *iterList;
        // Записываем данные в поток
        tempStream << "   " << tempsps.line << "   " << tempsps.picket << "   " << tempsps.x  << "   " <<  tempsps.y  << endl;
        iterList++;
    }
    outputFile.close();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{

    close();
}


void MainWindow::on_pushButton_clicked()
{
    // Если пользователь выбрал файл то сохраняем путь к нему
    GLOBAL_USER::NAME_S_SPS =nameSps ("S_sps");
    QMessageBox::warning ( this,"OK", GLOBAL_USER::NAME_S_SPS, QMessageBox::Ok);
}


void MainWindow::on_pushButton_2_clicked()
{
    // Если пользователь выбрал файл то сохраняем путь к нему
    GLOBAL_USER::NAME_R_SPS =nameSps ("R_sps");
    QMessageBox::warning ( this,"OK", GLOBAL_USER::NAME_R_SPS, QMessageBox::Ok);
}


void MainWindow::on_pushButton_4_clicked()
{
    /// Сохранить S и R sps в линиях в формате Мапинфо
    bool saveSRinLineForMapinfo =  ui->checkBox->checkState();

    /// Сохранить S и R sps в точках в формате Мапинфо
    bool saveSRinPointForMapinfo =  ui->checkBox_2->checkState();

    /// Сохранить площадки пересечения S и R sps в формате Мапинфо
    bool saveAreaIntersSRforMapinfo =  ui->checkBox_3->checkState();

    /// Сохранить линии сооединяющие смещенные ПВ в формате Мапинфо
    bool saveLineEditPVforMapinfo =  ui->checkBox_4->checkState();

    if (saveSRinLineForMapinfo || saveSRinPointForMapinfo
            || saveAreaIntersSRforMapinfo) {
        // Проверяем не пустые ли пути к нашим файлам
        if (!chekRoodSps (GLOBAL_USER::NAME_S_SPS, "s_sps")) {
            return;
        }
        if (!chekRoodSps (GLOBAL_USER::NAME_R_SPS, "r_sps")) {
            return;
        }
    }

    if (saveLineEditPVforMapinfo){
        // Проверяем не пустые ли пути к нашим файлам
        if (!chekRoodSps (GLOBAL_USER::NAME_S_SPS_EDIT, "s_sps_edit")) {
            return;
        }
    }

    /// На сколько удленять линии ПВ и ПП
    float dOffsetPV = -1;
    float dOffsetPP = -1;

    /// Диагональ ромба для площадки
    double userdRomb = -1;

    /// Делаем искуственное удлинение на 3 % той длины которую указал пользователь
    double const VALUE_FAKE = 1.03;

    // Присваеваем переменные пользователя
    QString str = ui->lineEdit_2->text();
    dOffsetPP = str.toFloat() * VALUE_FAKE;

    str = ui->lineEdit_3->text();
    dOffsetPV = str.toFloat() * VALUE_FAKE;

    /// Косинус угла наклона 45 град
    double const COS_ANGLE45 = 0.70710678;

    str = ui->lineEdit->text();
    userdRomb = str.toFloat() * COS_ANGLE45;

    /// Версия SPS файлов
    int   version = -1;
    str = ui->lineEdit_4->text();
    version = str.toInt();

    ///Создаем класс для работы с C_СПС
     sps s_sps(version);
     sps r_sps(version);
     sps sEdit(version);

    ///Создаем класс для сохранения в формате Мапинфо
    calcMapinfo calcMap(dOffsetPV, dOffsetPP, userdRomb);

    if (saveSRinLineForMapinfo || saveSRinPointForMapinfo
             || saveAreaIntersSRforMapinfo) {
         // Загружаем С_спс
         s_sps.formatSPS(GLOBAL_USER::NAME_S_SPS, "S");//Определяем формат указанного пользователем файла

         // Загружаем Р_спс
         r_sps.formatSPS(GLOBAL_USER::NAME_R_SPS, "R");//Определяем формат указанного пользователем файла
    }

    if (saveSRinLineForMapinfo) {
         // Определяем линии и сохраняем в формате Мапинфо
        calcMap.saveLineToMapinfo(  s_sps.spsMinMax, "S" );
        calcMap.saveLineToMapinfo(  r_sps.spsMinMax, "R" );
    }

    if (saveSRinPointForMapinfo) {
         // Определяем точки и сохраняем в формате Мапинфо
         calcMap.savePointToMapinfo( &s_sps.spsFile,  "S" );
         calcMap.savePointToMapinfo( &r_sps.spsFile,  "R" );
    }

    if (saveAreaIntersSRforMapinfo) {
         // Находим точки пересчение между ЛПП и ЛПВ
         calcMap.saveTableToMapinfo( &s_sps.spsMinMax, &r_sps.spsMinMax );
    }

    if (saveLineEditPVforMapinfo) {
         // Загружаем смещенки
         sEdit.formatSPS(GLOBAL_USER::NAME_S_SPS_EDIT, "S");//Определяем формат указанного пользователем файла
         calcMap.savePolyLineToMapinfo( &sEdit.spsFile,"S_edit_Polyl" );
    }

    /* Выгрузка в текстовые файлы для проверки
    outputFile(&spsS, "d:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\S_copy.txt");
    outputFile(&spsR, "d:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\R_copy.txt");
    */
}


void MainWindow::on_pushButton_5_clicked()
{
    // Если пользователь выбрал файл то сохраняем путь к нему
    GLOBAL_USER::NAME_S_SPS_EDIT =nameSps ("S_sps_edit");
    QMessageBox::warning ( this,"OK", GLOBAL_USER::NAME_S_SPS_EDIT, QMessageBox::Ok);
}

