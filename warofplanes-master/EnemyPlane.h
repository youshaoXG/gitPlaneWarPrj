#ifndef ENEMYPLANE_H
#define ENEMYPLANE_H

#include "Plane.h"

// 敌机种类：小敌机，BOSS机
enum EnemyType {ORD, BOSS};

// 敌机继承自 飞机类
class EnemyPlane :protected Plane
{
    friend class Control;// 声明Control为友元类
public:
	EnemyPlane();
    EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType type, int life);
	pair<int,int> updatePosition();

protected:
    EnemyType type; // 敌机种类
	
};

#endif // !ENEMYPLANE_H

