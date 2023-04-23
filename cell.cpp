
#include "cell.h"

Cell::Cell(QObject *parent)
    : QObject{parent}
{

}

void Cell::setMarked(bool x){
    this->isMarked = x;
}

void Cell::setFlag(bool x){
    this->isFlag = x;
}

void Cell::setMine(bool x){
    this->isMine = x;
}

bool Cell::getMarked(){
    return this->isMarked;
}
bool Cell::getFlag(){
    return this->isFlag;
}
bool Cell::getMine(){
    return this->isMine;
}
