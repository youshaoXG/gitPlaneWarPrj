#include "MyPlane.h"
#include <iostream>
using namespace std;

MyPlane::MyPlane():Plane()
{
}

MyPlane::MyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, int skill):
    Plane(x, y, imageFile, scene, life, ME)
{
//    m_pixmap = QPixmap(QString::fromStdString(imageFile));
    this->skill = skill;    // 杀伤力
}
