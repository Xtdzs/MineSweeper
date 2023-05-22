
#ifndef MINESWEEPERGRID_H
#define MINESWEEPERGRID_H


#include "cell.h"
#include <QWidget>
#include <QVector>
#include <QPair>


class MineSweeperGrid : public QWidget
{
    Q_OBJECT
public:
    explicit MineSweeperGrid(QWidget *parent = nullptr);
    MineSweeperGrid(int row, int col, int mineCount){
        rows = row;
        cols = col;
        this->mineCount = mineCount;
        for (int i = 0; i < rows; i++) {
            grid[i].resize(cols);
        }
    }
    int rows;
    int cols;
    int mineCount;
    QVector<QVector<Cell*>> grid;
    QVector<QPair<int, int>> mines;
    // 其他成员函数

private:


signals:

};

#endif // MINESWEEPERGRID_H
