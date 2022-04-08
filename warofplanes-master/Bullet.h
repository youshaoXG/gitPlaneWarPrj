#ifndef BULLET_H
#define BULLET_H

#include <QtWidgets>

#include "Object.h"
#include <utility>
#include <fstream>
#include <iostream>
using namespace std;

class Bullet: public Object
{
    friend class Control;  	// 声明Control为友元类
public:
	Bullet();
    Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power);
    pair<qreal,qreal> updatePosition();
    bool hit(QGraphicsScene *scene);// 用于子弹与飞机相撞时降低子弹生命值，生命值降为 0 时抹去子弹

private:
    QPointF dir;    		// 子弹方向，同时也控制了子弹速度
    int power;      		// 子弹杀伤力
};

#endif // !BULLET_H

