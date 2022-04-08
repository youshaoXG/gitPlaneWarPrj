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

//    /* 文本提示 */
//    gameWelcomeText = new QGraphicsTextItem;                                // 新建 游戏结束提示文本项 文本项
//    addItem(gameWelcomeText);                                               // 添加 游戏结束提示文本项 文本项
//    gameWelcomeText->setHtml(tr("<font color=white>Air War Game</font>"));  // 设置文本Html样式
//    gameWelcomeText->setFont(QFont("Algerian", 22));                        // 设置 文本字体
//    gameWelcomeText->setPos(150, 150);                                      // 设置 文本位置
//    gameWelcomeText->setZValue(2);                                          // 设置 ZValue值
//    gameWelcomeText->show();                                                // 隐藏 游戏结束提示文本
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
    QPixmap bkpixmap(QPixmap(":/images/background.png"));                   // 加载 游戏背景图片资源
    view->setBackgroundBrush(QBrush(bkpixmap));                             // 设置 背景填充图
//    int BKWidth = bkpixmap.width();                                         // 背景宽
//    int BKHeight = bkpixmap.height();                                       // 背景高
    view->show();                                                           // 显示 游戏界面

    return app.exec();                                                      // 阻塞程序到这一行
}
