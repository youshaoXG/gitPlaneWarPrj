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
    void synScreen(QGraphicsScene *scene);  // 将子弹或者飞机显示在屏幕上
    void delScreen(QGraphicsScene *scene);  // 将子弹或者飞机在屏幕上抹去

protected:
    enum WarPart part;                      // 玩家、敌机或供给

    /* TODO√ */
    int width;                              // Object对象的 宽度
    int height;                             // Object对象的 高度
};

#endif // OBJECT_H
