#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QString>
#include <QDir>
#include <QFile>
#include <readsps.h>
#include <sps.h>
#include <calcmapinfo.h>
#include <QMessageBox>
#include <time.h>
#include <QTextStream>

/// Сохраняем пути к нашим спс файлам и основные введенные параметры пользователем
namespace GLOBAL_USER {
    // Путь к нашим спс
    //QString NAME_S_SPS = "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\Проект_Терсинский_S.sps";
    //QString NAME_R_SPS = "f:\\Mapinfo_RUBKA\\Mapinfo_RUBKA\\Проект_Терсинский_R.sps";
    //QString NAME_S_SPS = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\Проект_Терсинский_S.sps";
    //QString NAME_R_SPS = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\Проект_Терсинский_R.sps";

    //QString NAME_S_SPS_EDIT = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Edit_Терсинский_S.sps";
    //QString NAME_S_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Проект_Терсинский_S.sps";
    //QString NAME_R_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Проект_Терсинский_R.sps";

    QString NAME_S_SPS_EDIT = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Edit_Арласнкий_S.sps";
    QString NAME_S_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Арласнкий_S.sps";
    QString NAME_R_SPS      = "D:\\CALC_VIEW\\PSEVDOKOD\\Mapinfo_RUBKA\\test\\Арласнкий_R.sps";

}

enum viewSps {
    S_SPS,
    R_SPS
};


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

void inputUser() {

}

///
/// \brief Вывод в текстовой файл загруженного спс файла
/// \param sps  класс в которую загружен спс файл
/// \param имя файла которое будет создан на диске
///
void outputFile(readSps* sps, QString nameFile){
    readSps::STRUCK_SPS tempsps;

    // Файл в которой будем выгружать файл
    QFile outputFile (nameFile);
    outputFile.open ( QIODevice::WriteOnly | QIODevice::Truncate );
    QTextStream tempStream( &outputFile );
    tempStream.setRealNumberNotation(QTextStream::FixedNotation);//tempStream.setRealNumberNotation( FixedNotation );
    //tempStream.setRealNumberPrecision(2);

    // Итераор по списку
    QList<readSps::STRUCK_SPS>::iterator iterList;
    iterList = sps->sps.begin();

    // Начинаем цикл по всем записям в списке
    while( iterList != sps->sps.end() ){
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
    // Проверяем спс
    if (!chekRoodSps (GLOBAL_USER::NAME_S_SPS, "s_sps")) {
        return;
    } else if (!chekRoodSps (GLOBAL_USER::NAME_R_SPS, "r_sps")) {
                return;
    } else if (!chekRoodSps (GLOBAL_USER::NAME_S_SPS_EDIT, "s_sps_edit")) {
                return;
    }


    /// Диагональ ромба для площадки
    double userdRomb = 10.6066;

    /// На сколько удленять линии ПВ и ПП
    float dOffsetPV = -1;
    float dOffsetPP = -1;

    // Присваеваем переменные пользователя
    QString strOfsset = ui->lineEdit_2->text();
    dOffsetPP = strOfsset.toFloat();

    strOfsset = ui->lineEdit_3->text();
    dOffsetPV = strOfsset.toFloat();

    /// Версия SPS файлов
    int   version = -1;
    strOfsset = ui->lineEdit_4->text();
    version = strOfsset.toInt();

    //Создаем класс для работы с C_СПС
    readSps spsS(dOffsetPV, dOffsetPP, userdRomb, version),
            spsR(dOffsetPV, dOffsetPP, userdRomb, version),
            spsS_Edit(dOffsetPV, dOffsetPP, userdRomb, version),
            tempSps;

    spsS.formatSPS(GLOBAL_USER::NAME_S_SPS, "S");//Определяем формат указанного пользователем файла
    spsR.formatSPS(GLOBAL_USER::NAME_R_SPS, "R");//Определяем формат указанного пользователем файла

    // Сохраняем точки пересечения в виде площадки ромба
    tempSps.saveTableToMapinfo(&spsS.spsMinMax, &spsR.spsMinMax );

    // Загружаем s_sps файл со смещенными ПВ
    spsS_Edit.formatSPS(GLOBAL_USER::NAME_S_SPS_EDIT, "S");//Определяем формат указанного пользователем файла



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

