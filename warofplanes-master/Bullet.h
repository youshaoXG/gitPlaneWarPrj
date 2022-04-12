#ifndef BULLET_H
#define BULLET_H

#include <QtWidgets>

#include "Object.h"
#include <utility>
#include <fstream>
#include <iostream>
using namespace std;

// 公共继承自Object
class Bullet: public Object
{
    friend class Control;  	// 声明Control为友元类
public:
    Bullet();
    Bullet(WarPart part, int x, int y, const string &imageFile, const QPointF &dir, int power);
    /* 更新位置 */
    // 返回的是其x坐标、y坐标。pair是将2个数据组合成一组数据，用法之一就是当函数需要两个返回值时作为返回值类型
    // pair的实现是一个结构体，主要的两个成员变量是first、second，因为是使用struct不是class，所以可以直接使用pair的成员变量。
    pair<qreal,qreal> updatePosition();
    bool hit(QGraphicsScene *scene);// 用于子弹与飞机相撞时降低子弹生命值，生命值降为 0 时抹去子弹

private:
    QPointF dir;    		// 子弹方向，同时也控制了子弹速度
    int power;      		// 子弹杀伤力
};

#endif // !BULLET_H
