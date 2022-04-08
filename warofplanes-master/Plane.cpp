#include "Plane.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
using namespace std;

Plane::Plane()
{
}

Plane::Plane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, enum WarPart part):
    Object(part, imageFile)
{
    setPos(x, y);
    this->life = life;
    scene->addItem(this);
    update();
}

/* 用于飞机与飞机或子弹相撞时降低生命值，生命值降为0时抹去飞机 */
bool Plane::crash(QGraphicsScene *scene)
{
    if (life <= 0)          //若已经耗尽生命值
		return false;
    else if (--life <= 0)   //若生命值此次crash后耗尽
	{
        delScreen(scene);
		return false;
	}
	else
		return true;
}
