#ifndef PLANE_H
#define PLANE_H

#include "Bullet.h"
#include "Object.h"
#include <QtWidgets>
#include <cstring>
#include <vector>
using namespace std;

class Plane: public Object
{
	friend class Control;
public:
	Plane();
    Plane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, enum WarPart part);
    bool crash(QGraphicsScene *scene); // 用于飞机与飞机或子弹相撞时降低生命值，生命值降为0时抹去飞机
	
protected:
	int life; //生命值
};

#endif // !PLANE_H
