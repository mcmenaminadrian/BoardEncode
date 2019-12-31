#include <QGraphicsScene>
#include <QFileDialog>
#include <QString>
#include <QRegularExpression>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <iostream>
#include <bitset>
#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

#define BIT 9

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    jpegFile = QFileDialog::getOpenFileName(this, tr("Pick JPEG"), "",
        tr("JPEGs (*.jpeg)"));

    QRegularExpression qRegEx("JPEG_FRAG_([0-9]*[a-f]*(?!(_)))+");
    QRegularExpressionMatch qREM = qRegEx.match(jpegFile);
    displayInitalImage(qREM.captured());


}

void MainWindow::displayInitalImage(const QString& imgName)
{
    displayScene = new QGraphicsScene();
    coreImageName = imgName;
    ui->graphicsView->setScene(displayScene);
    rowNumber = 0;
    colNumber = 0;
    displayBoard(coreImageName, rowNumber, colNumber);

}


MainWindow::~MainWindow()
{
    delete displayScene;
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //validate data
    if (ui->checkBox_3->isChecked() & ui->radioButton_2->isChecked())
    {
        QMessageBox qMB(QMessageBox::Warning, "Coding error",
                        "Square cannot be empty and hold a piece",
                        QMessageBox::Ok);
        qMB.exec();
        return;
    }

    bitset<BIT> bitStatus;
    if (ui->checkBox->isChecked()) {
        bitStatus.set(0);
    }
    if (ui->checkBox_3->isChecked()) {
        bitStatus.set(1);
    }
    if (ui->radioButton_2->isChecked()) {
        bitStatus.set(2);
    }
    if (ui->radioButton_4->isChecked()) {
        bitStatus.set(3);
    }
    if (ui->radioButton_5->isChecked()) {
        bitStatus.set(4);
    }
    if (ui->radioButton_6->isChecked()) {
        bitStatus.set(5);
    }
    if (ui->radioButton_7->isChecked()) {
        bitStatus.set(6);
    }
    if (ui->radioButton_8->isChecked()) {
        bitStatus.set(7);
    }
    if (ui->radioButton_9->isChecked()) {
        bitStatus.set(8);
    }

    QString fileOutName = coreImageName + QString::number(rowNumber) + "_" +
                QString::number(colNumber) + ".dat";
    QFile statusOut(fileOutName);
    statusOut.open(QIODevice::WriteOnly);
    QDataStream out(&statusOut);
    out <<  bitStatus.to_string().c_str() << '\n';

    colNumber = (colNumber + 1)%8;
    if (colNumber == 0) {
        rowNumber = (rowNumber + 1)%8;
        if (rowNumber == 0) {
            QMessageBox qMB(QMessageBox::Information, "Completed",
                            "Coding for this board now complete.",
                            QMessageBox::Ok);
            qMB.exec();
            exit(1);
        }
    }
    displayBoard(coreImageName, rowNumber, colNumber);
}

void MainWindow::displayBoard(const QString& coreName, const int rowNumber,
                              const int colNumber)
{
    //load file
    QString fileName = coreName + QString::number(rowNumber) + "_" +
            QString::number(colNumber) + ".jpeg";
    QImage boardImage(fileName);
    if (boardImage.isNull()) {
        cerr << "Could not open" << fileName.toStdString() << endl;
        return;
    }
    ui->graphicsView->scene()->addPixmap(QPixmap::fromImage(boardImage));
}
