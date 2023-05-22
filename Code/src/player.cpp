
#include "player.h"

Player::Player(QObject *parent)
    : QObject{parent}
{

}

void Player::saveScore(int score) {
    QString fileName = QString("scores/%1.txt").arg(name);
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&file);
        stream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << "," << score << "\n";
        file.close();
    }
}


