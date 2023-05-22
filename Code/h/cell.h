
#ifndef CELL_H
#define CELL_H


#include <QObject>


class Cell : public QObject
{
    Q_OBJECT
public:
    explicit Cell(QObject *parent = nullptr);
    Cell(bool isMine, bool isMarked, bool isFlag, int num){
        this->isMine = isMine;
        this->isMarked = isMarked;
        this->isFlag = isFlag;
        this->num=  num;
    }

    int num;
    bool getMarked();
    bool getFlag();
    bool getMine();
    void setMarked(bool x);
    void setFlag(bool x);
    void setMine(bool x);
signals:

private:
    bool isMine;
    bool isMarked;
    bool isFlag;
};

#endif // CELL_H
