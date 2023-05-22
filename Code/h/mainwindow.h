
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mss.h"
#include "qpushbutton.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow

{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    void startGame();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QPushButton *startButton;
    std::unique_ptr<MSs> mss;
};

#endif // MAINWINDOW_H
