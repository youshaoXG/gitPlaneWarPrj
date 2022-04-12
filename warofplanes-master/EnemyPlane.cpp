#include "EnemyPlane.h"
#include <cstdlib>

EnemyPlane::EnemyPlane()
{
}

EnemyPlane::EnemyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, EnemyType type, int life):
    Plane(x, y, imageFile, scene, life, ENEMY)
{
    this->type = type;
}

pair<int,int> EnemyPlane::updatePosition()
{
    // 利用make_pair创建新的pair对象，比较方便，可自动匹配数据类型
    return make_pair(x(), y()+2);
}
