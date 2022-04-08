#ifndef MYPLANE_H
#define MYPLANE_H

#include "Plane.h"

class MyPlane:protected Plane
{
    friend class Control;// 声明Control为友元类
public:
	MyPlane();
    MyPlane(int x, int y, const string &imageFile, QGraphicsScene *scene, int life, int skill);

private:
    int skill; //技能值
//    QPixmap m_pixmap;
};

#endif // !MYPLANE_H

