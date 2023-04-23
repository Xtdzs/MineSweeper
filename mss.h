
#ifndef MSS_H
#define MSS_H

#include "minesweepergrid.h"
#include <QRandomGenerator>
#include <QLCDNumber>
#include <QCloseEvent>
#include <QPushButton>
#include <QWidget>
#include <QVector>
#include <QString>
#include <QTimer>
#include <QLabel>
#include <QFile>


class MSs : public QWidget
{
    Q_OBJECT
public:
    explicit MSs(QWidget *parent = nullptr);

    MSs(int a, int b, int c, int difficulty){
        ROWS = a;
        COLS = b;
        numMines = c;
        this->difficulty = difficulty;
        cells.resize(ROWS);
        for (int i = 0; i < ROWS; i++) {
            cells[i].resize(COLS);
        }
        minesweepergrid.rows = a;
        minesweepergrid.cols = b;
        minesweepergrid.mineCount = c;
        for (int i = 0; i < a; i++) {
            QVector<Cell*> n;
            for(int j = 0; j < b; j++){
                Cell *cell = new Cell(0,0,0,0);
                n.push_back(cell);
            }
            minesweepergrid.grid.append(n);
        }
        Q_ASSERT(numMines < ROWS * COLS);

        // Generate mines
        int minesSet = 0;
        while (minesSet < numMines) {
            int r = QRandomGenerator::global()->bounded(ROWS);
            int c = QRandomGenerator::global()->bounded(COLS);
            if (!minesweepergrid.grid[r][c]->getMine()) {
                minesweepergrid.grid[r][c]->setMine(true);
//                qDebug() << minesweepergrid.grid[r][c]->getMine();
                minesweepergrid.mines.append(QPair<int, int>(r, c));
                ++minesSet;
            }
        }
        // Calculate adjacent mines
        for (int r = 0; r < ROWS; ++r) {
            for (int c = 0; c < COLS; ++c) {
                if (!minesweepergrid.grid[r][c]->getMine()) {
                    minesweepergrid.grid[r][c]->num = countAdjacentMines(r, c);
//                    int t = countAdjacentMines(r, c);
//                    qDebug() << t;
                }
            }
        }
        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &MSs::updateTimer);
        lcd = new QLCDNumber(this);
        lcd->setDigitCount(3);
        lcd->setSegmentStyle(QLCDNumber::Outline);
        QString styleSheet = "QLCDNumber { background-color: grey; color: white; }";
        lcd->setStyleSheet(styleSheet);
        lcd->display(0);
        highest_score_1 = new QLabel(tr("Fastest Speed for easy: %1 seconds").arg(read_highest_score_1()));
        highest_score_1->setStyleSheet("QLabel { color : blue; }");
        highest_score_2 = new QLabel(tr("Fastest Speed for medium: %1 seconds").arg(read_highest_score_2()));
        highest_score_2->setStyleSheet("QLabel { color : blue; }");
        highest_score_3 = new QLabel(tr("Fastest Speed for hard: %1 seconds").arg(read_highest_score_3()));
        highest_score_3->setStyleSheet("QLabel { color : blue; }");

        QFile file("../highest_score.txt");
        if (file.open(QIODevice::ReadWrite)) {
            QTextStream stream(&file);
            stream << 0 << 0 << 0;
            file.close();
        }

    }
    QCloseEvent *event = nullptr;
    QTimer* timer = nullptr;
    QLCDNumber* lcd = nullptr;
    QTimer cur_time;
    QTimer start_time;
    QLabel* highest_score_1 = nullptr;
    QLabel* highest_score_2 = nullptr;
    QLabel* highest_score_3 = nullptr;
    void updateButtons();
    int read_highest_score_1();
    int read_highest_score_2();
    int read_highest_score_3();
    void record_highest_score_1(int t);
    void record_highest_score_2(int t);
    void record_highest_score_3(int t);
    int countAdjacentMines(int row, int col);
    void closeEvent(QCloseEvent *event);
    bool isGameWon();

public slots:
    void newGame();
    void onCellClicked(int row, int col);
    void onCellRightClicked(int row, int col);
    void gameOver(bool win);
    void flipCell(int row, int col);
    void revealBoard();
    void saveGame(QString fileName);
    void startTimer();
    void updateTimer();

signals:

private:
    int ROWS;
    int COLS;
    int numMines;
    int CELL_SIZE = 30;
    int gameTime;
    int difficulty;
    QLabel *label_timer = nullptr;
    QVector<QVector<QPushButton*>> cells;
    MineSweeperGrid minesweepergrid = MineSweeperGrid(0, 0, 0);
};

#endif // MSS_H
