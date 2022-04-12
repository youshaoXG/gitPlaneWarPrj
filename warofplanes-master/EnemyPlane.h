#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "Plane.h"

// 敌机种类：小敌机（普通敌机ordinary），BOSS机
enum EnemyType {ORD, BOSS};

// 敌机继承自 飞机类
class EnemyPlane :protected Plane
{
    friend class Control;// 声明Control为友元类
public:
    EnemyPlane();
    EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType type, int life);
    /* 更新位置 */
    // 返回的是其x坐标、y坐标。pair是将2个数据组合成一组数据，用法之一就是当函数需要两个返回值时作为返回值类型
    // pair的实现是一个结构体，主要的两个成员变量是first、second，因为是使用struct不是class，所以可以直接使用pair的成员变量。
    pair<int,int> updatePosition();

protected:
    EnemyType type; // 敌机种类

};

#endif // !ENEMYPLANE_H
