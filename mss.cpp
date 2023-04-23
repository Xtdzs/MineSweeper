#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QRandomGenerator>
#include <iostream>
#include <queue>
#include <QFile>
#include "mss.h"
#include "qobjectdefs.h"

static int count = 0;

MSs::MSs(QWidget *parent)
    : QWidget{parent}
{

}

void MSs::closeEvent(QCloseEvent *event)
{
    timer->stop();
    count = 0;
    event->accept();
}

int MSs::read_highest_score_1()
{
    QFile file("../highest_score_1.txt");
    int highscore = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString highscoreString = in.readLine();
        highscore = highscoreString.toInt();
        file.close();
    }
    return highscore;
}

int MSs::read_highest_score_2()
{
    QFile file("../highest_score_2.txt");
    int highscore = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString highscoreString = in.readLine();
        highscore = highscoreString.toInt();
        file.close();
    }
    return highscore;
}

int MSs::read_highest_score_3()
{
    QFile file("../highest_score_3.txt");
    int highscore = 0;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString highscoreString = in.readLine();
        highscore = highscoreString.toInt();
        file.close();
    }
    return highscore;
}

void MSs::record_highest_score_1(int t)
{
    int past = read_highest_score_1();
    if(t > past && past != 0) {
        highest_score_1->setText(tr("Fastest speed for easy: %1 seconds").arg(past));
        return;
    }
    QFile file("../highest_score_1.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::number(t) << Qt::endl;
        file.close();
    }
    highest_score_1->setText(tr("Fastest speed for easy: %1 seconds").arg(t));
    return;
}


void MSs::record_highest_score_2(int t){
    int past = read_highest_score_2();
    if(t > past && past != 0) {
        highest_score_2->setText(tr("Fastest speed for medium: %1 seconds").arg(past));
        return;
    }
    QFile file("../highest_score_2.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::number(t) << Qt::endl;
        file.close();
    }
    highest_score_2->setText(tr("Fastest speed for medium: %1 seconds").arg(t));
}

void MSs::record_highest_score_3(int t){
    int past = read_highest_score_3();
    if(t > past && past != 0) {
        highest_score_3->setText(tr("Fastest speed for hard: %1 seconds").arg(past));
        return;
    }
    QFile file("../highest_score_3.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << QString::number(t) << Qt::endl;
        file.close();
    }
    highest_score_2->setText(tr("Fastest speed for hard: %1 seconds").arg(t));
}

int MSs::countAdjacentMines(int row, int col){
    int count = 0;
    for(int i = qMax(0, row-1); i <= qMin(ROWS-1, row+1); i++){
        for(int j = qMax(0, col-1); j <= qMin(COLS-1, col+1); j++){
            if(minesweepergrid.grid[i][j]->getMine()){
                count++;
            }
        }
    }
//    qDebug() << count;
    return count;
}

void MSs::startTimer() {
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
}

void MSs::updateTimer() {
    count++;
    lcd->display(count);
}


void MSs::saveGame(QString fileName) {
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream stream(&file);
        stream << ROWS << COLS << numMines << gameTime /*<< cellsRevealed*/;
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                stream << minesweepergrid.grid[row][col]->getMine() << minesweepergrid.grid[row][col]->getFlag() << minesweepergrid.grid[row][col]->getMarked();
            }
        }
        file.close();
    }
}


void MSs::revealBoard()
{
    for (int i = 0; i < minesweepergrid.mines.size(); ++i) {
        int row = minesweepergrid.mines[i].first;
        int col = minesweepergrid.mines[i].second;
        minesweepergrid.grid[row][col]->setMarked(true);
    }
}

void MSs::gameOver(bool win){
    timer->stop();
    revealBoard();
    QString message;
    if (win) {
        int t = lcd->value();
        if(this->difficulty == 1){
            record_highest_score_1(t);
        }
        if(this->difficulty == 2){
            record_highest_score_2(t);
        }
        if(this->difficulty == 3){
            record_highest_score_3(t);
        }
        message = "You won! Play again?";
    } else {
        message = "You lost. Play again?";
    }
    int result = QMessageBox::question(this, "Game Over", message, QMessageBox::Yes | QMessageBox::No);
    if (result == QMessageBox::Yes) {
        int r = this->ROWS;
        int c = this->COLS;
        int m = this->numMines;
        int d = this->difficulty;
        delete this;
        MSs* mss = new MSs(r, c, m, d);
        mss->newGame();
//        qDebug() << 1;
        mss->show();
        count = 0;
//        mss->timer = new QTimer(mss);
//        connect(mss->timer, SIGNAL(timeout()), mss, SLOT(updateTimer()));
//        mss->timer->start(1000);
    } else {
        close();
    }

}

void MSs::updateButtons()
{
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            QPushButton *button = cells[row][col];
            button->setFixedSize(30, 30);
            if (minesweepergrid.grid[row][col]->getMarked() == 0) {
                if(button->text() == "*"){
                    button->setText("*");
                    button->setEnabled(true);
                }
                else{
                    button->setText("");
                    button->setEnabled(true);
                }
            } else if (minesweepergrid.grid[row][col]->num > 0) {
                button->setText(QString::number(minesweepergrid.grid[row][col]->num));
                button->setEnabled(false);
            } else if (minesweepergrid.grid[row][col]->getMine()) {
                button->setText("");
                button->setEnabled(true);
            } else if (minesweepergrid.grid[row][col]->num == 0) {
                button->setText("");
                button->setEnabled(false);
            }
        }
    }
}


void MSs::flipCell(int row, int col){
    if(minesweepergrid.grid[row][col]->getFlag() || minesweepergrid.grid[row][col]->getMarked()){
        return;
    }
    minesweepergrid.grid[row][col]->setMarked(true);
    if(minesweepergrid.grid[row][col]->num == 0){
        std::queue<std::pair<int, int>> queue;
        queue.push({row, col});

        while(!queue.empty()){
            int cur_x = queue.front().first;
            int cur_y = queue.front().second;
            queue.pop();

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }

                    int nx = cur_x + dx;
                    int ny = cur_y + dy;

                    if(nx >= minesweepergrid.rows || ny >= minesweepergrid.cols || nx < 0 || ny < 0){
                        continue;
                    }

                    if (minesweepergrid.grid[nx][ny]->num >= 0 && minesweepergrid.grid[nx][ny]->getMarked() == false) {
                        minesweepergrid.grid[nx][ny]->setMarked(true);
                        if (minesweepergrid.grid[nx][ny]->num == 0) {
                            queue.push({nx, ny});
                        }
                    }
                }
            }
        }
    }
}

void MSs::newGame(){
    setWindowIcon(QIcon(":/mine.ico"));
    QLayout* existingLayout = layout();
    if (existingLayout != nullptr) {
        delete existingLayout;
    }
    QGridLayout* gridLayout = new QGridLayout(this);
    timer->start(1000);
    gridLayout->addWidget(lcd, ROWS + 1, 0, 1, 2);
    gridLayout->setSpacing(0);
    if(this->difficulty == 1){
        highest_score_1->setStyleSheet("QLabel { color : blue; }");
        gridLayout->addWidget(highest_score_1, ROWS + 1, 3, 1, 7);
    }
    if(this->difficulty == 2){
        highest_score_2->setStyleSheet("QLabel { color : blue; }");
        gridLayout->addWidget(highest_score_2, ROWS + 1, 3, 1, 8);
    }
    if(this->difficulty == 3){
        highest_score_3->setStyleSheet("QLabel { color : blue; }");
        gridLayout->addWidget(highest_score_3, ROWS + 1, 3, 1, 7);
    }
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            QPushButton* button = new QPushButton(this);
            button->setFixedSize(CELL_SIZE, CELL_SIZE);
            button->setProperty("row", row);
            button->setProperty("col", col);
            gridLayout->addWidget(button, row, col);
            connect(button, &QPushButton::clicked, this, [this, col, row]() {
                onCellClicked(row, col);
            });
            button->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(button, &QPushButton::customContextMenuRequested, this, [this, row, col](){
                onCellRightClicked(row, col);
            });
            cells[row][col] = button;
        }
    }
}

bool MSs::isGameWon(){
    int cnt1 = 0, cnt2 = 0;
    for(int x = 0; x < ROWS; x++){
        for(int y = 0; y < COLS; y++){
            if(minesweepergrid.grid[x][y]->getMarked() == 0 && minesweepergrid.grid[x][y]->getMine()){
                cnt1++;
            }
            else if(minesweepergrid.grid[x][y]->getMarked()){
                cnt2++;
            }
        }
    }
    if(cnt1 == minesweepergrid.mines.size() && cnt1 + cnt2 == ROWS*COLS){
        return true;
    }
    else{
        return false;
    }
}

void MSs::onCellClicked(int row, int col) {
//    qDebug() << minesweepergrid->grid[row][col]->isMine;
    if (minesweepergrid.grid[row][col]->getMine()) {
        // 点到了地雷，游戏结束
//        qDebug() << 1;
        revealBoard();
        updateButtons();
        gameOver(false);
    } else {
        // 翻开方格
//        qDebug() << 0;
        flipCell(row, col);
        if(isGameWon()){
            revealBoard();
            gameOver(true);
        }
        updateButtons();
    }
}

void MSs::onCellRightClicked(int row, int col) {
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (minesweepergrid.grid[row][col]->getFlag()) {
        // 取消标记
        button->setText("");
        minesweepergrid.grid[row][col]->setFlag(false);
    } else {
        // 标记为地雷
        button->setText("*");
        minesweepergrid.grid[row][col]->setFlag(true);
    }
}

