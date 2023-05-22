#include "mainwindow.h"
#include "qmessagebox.h"
#include <QFile>
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
//    // 创建一个 startButton 按钮，并添加到主窗口中
//    QPushButton* startButton = new QPushButton("Start", this);
//    startButton->move(200, 200);
//    connect(startButton, SIGNAL(clicked()), this, SLOT(startGame()));
//    layout()->addWidget(startButton);
    ui->setupUi(this);
    setWindowIcon(QIcon(":/mine.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    MSs* mss = new MSs(9, 9, 10, 1);
    mss->lcd->display(0); // 重置计时器
    mss->timer->start(1000);
    mss->newGame();
    mss->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    MSs* mss = new MSs(16, 16, 40, 2);
    mss->lcd->display(0); // 重置计时器
    mss->timer->start(1000);
    mss->newGame();
    mss->show();
}


void MainWindow::on_pushButton_3_clicked()
{
    MSs* mss = new MSs(16, 30, 99, 3);
    mss->lcd->display(0); // 重置计时器
    mss->timer->start(1000);
    mss->newGame();
    mss->show();
}


void MainWindow::on_pushButton_4_clicked()
{
    QFile file1("../highest_score_1.txt");
    int a1 = 0;
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file1);
        QString highscoreString1 = in.readLine();
        a1 = highscoreString1.toInt();
        file1.close();
    }
    QFile file2("../highest_score_2.txt");
    int a2 = 0;
    if (file1.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file2);
        QString highscoreString2 = in.readLine();
        a2 = highscoreString2.toInt();
        file2.close();
    }
    QFile file3("../highest_score_3.txt");
    int a3 = 0;
    if (file3.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file3);
        QString highscoreString3 = in.readLine();
        a3 = highscoreString3.toInt();
        file3.close();
    }
    QMessageBox msgBox;
    msgBox.setText(tr("Fastest speed for easy: %1 seconds\nFastest speed for medium: %2 seconds\nFastest speed for hard: %3 seconds").arg(a1).arg(a2).arg(a3));
    msgBox.exec();
}



void MainWindow::on_pushButton_6_clicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("Game Producer: Gao Jinliang, Xu Zixiang"));
    msgBox.exec();
}

