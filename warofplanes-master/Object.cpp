#include "Object.h"
//#include <QDebug>

Object::Object()
{

}

Object::Object(WarPart part, const string &imageFile):
    QGraphicsPixmapItem(QPixmap(QString::fromStdString(imageFile)))
{
    this->part = part;

    /* TODO√ */
    this->width = pixmap().width();
    this->height = pixmap().height();

    /* TODO-test√ */
    /*if(this->width == 79 && this->height == 90){
        qDebug("【玩家飞机】w:%d，h:%d", this->width, this->height);     // 十进制输出整型数据
    }else if(this->width == 70 && this->height == 70){
        qDebug("【普通敌机】w:%d，h:%d", this->width, this->height);     // 十进制输出整型数据
    }else if(this->width == 200 && this->height == 97){
        qDebug("【BOSS】w:%d，h:%d", this->width, this->height);       // 十进制输出整型数据
    }else if(this->width == 26 && this->height == 24){
        qDebug("【物资补给】w:%d，h:%d", this->width, this->height);     // 十进制输出整型数据
    }else if(this->width == 15 && this->height == 38){
        qDebug("【玩家子弹】w:%d，h:%d", this->width, this->height);     // 十进制输出整型数据
    }else if(this->width == 15 && this->height == 15){
        qDebug("【普通敌机子弹】w:%d，h:%d", this->width, this->height);  // 十进制输出整型数据
    }else if(this->width == 10 && this->height == 23){
        qDebug("【BOSS子弹】w:%d，h:%d", this->width, this->height);     // 十进制输出整型数据
    }*/

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
