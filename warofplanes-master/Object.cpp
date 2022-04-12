#include "Object.h"

Object::Object()
{

}

Object::Object(WarPart part, const string &imageFile):
    QGraphicsPixmapItem(QPixmap(QString::fromStdString(imageFile)))
{
    this->part = part;
}

Object::~Object()
{

}

/* 把飞机或者子弹显示到屏幕上：Display planes or bullets on the screen */
void Object::synScreen(QGraphicsScene *scene)
{
    if(!scene->items().contains(this))
    {
        scene->addItem(this);
        update();
    }
}

/* 把飞机或者子弹从屏幕中清除：Erase planes or bullets from the screen */
void Object::delScreen(QGraphicsScene *scene)
{
    scene->removeItem(this);
    update();
}