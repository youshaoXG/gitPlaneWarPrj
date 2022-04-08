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

/* ���ӵ���ʾ����Ļ��:Display planes or bullets on the screen */
void Object::synScreen(QGraphicsScene *scene)
{
    if(!scene->items().contains(this))
    {
        scene->addItem(this);
        update();
    }
}

/* ���ӵ�����Ļ��Ĩȥ:Erase planes or bullets from the screen */
void Object::delScreen(QGraphicsScene *scene)
{
    scene->removeItem(this);
    update();
}

