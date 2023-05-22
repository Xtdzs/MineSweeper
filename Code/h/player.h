
#ifndef PLAYER_H
#define PLAYER_H


#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDateTime>


class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = nullptr);
    void saveScore(int score);
signals:

private:
    QString name;
    int score;
    QString pwd;
};

#endif // PLAYER_H
