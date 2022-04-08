#ifndef OBJECT_H
#define OBJECT_H

#include <QtWidgets>
#include <string>
using namespace std;

// 枚举：玩家、敌机、供给
enum WarPart { ME, ENEMY, LIFESUPPLY };

class Object : public QGraphicsPixmapItem
{
public:
    Object();
    Object(WarPart part, const string &imageFile);
    ~Object();
    void synScreen(QGraphicsScene *scene);  // 将子弹显示在屏幕上
    void delScreen(QGraphicsScene *scene);  // 将子弹在屏幕上抹去

protected:
    enum WarPart part;
};

#endif // OBJECT_H
