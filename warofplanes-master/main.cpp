#include "Control.h"
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    /* 启动画面 */
    QPixmap welcome(":/images/welcome_new.png");                            // 加载欢迎界面图片资源
    QSplashScreen splash(welcome);
    splash.resize(welcome.size());                                          // 重置窗口大小 为启动背景图片的大小
    splash.show();                                                          // 显示图片
    app.processEvents();
    Sleep(1000);                                                            // 延时1秒

    // 实例化一个Control对象
    Control *ctrl = new Control;

    /* 显示界面 */
    QGraphicsView *view = new QGraphicsView;
    splash.finish(view);
    view->setWindowIcon(QPixmap(":/images/warofplanesicon.ico"));           // 游戏窗口图标
    view->setWindowTitle(QObject::tr("Air War Game"));                      // 游戏窗口标题
    view->setScene(ctrl);                                                   // 设置场景 为ctrl类
    QPixmap bkPixmap(QPixmap(":/images/background.png"));                   // 加载 游戏背景图片资源
    view->setBackgroundBrush(QBrush(bkPixmap));                             // 设置 背景填充图
    view->show();                                                           // 显示 游戏界面

    return app.exec();                                                      // 阻塞程序到这一行
}
