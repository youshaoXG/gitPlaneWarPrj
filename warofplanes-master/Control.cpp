#include "Control.h"

// 加载gif动图要用到的头文件
#include <QMovie>
#include <QCoreApplication>

/* -------------------------全局常量------------------------- */
// x=(width()-myplane_pixmap.width())/2, y=(height()+2*myplane_pixmap.height())/2
const QPointF myPlanePos = QPointF(360, 390);                   // 玩家飞机初始化位置，也可用于计算玩家血条的初始位置（屏幕中央偏下）
const QPointF scoreTextPos = QPointF(643, 503);                 // 玩家 分数文本坐标
const QPointF LifeBarPos   = QPointF(650, 550);                 // 玩家 血条坐标
const QPointF SkillBarPos  = QPointF(650, 570);                 // 玩家 技能条坐标

const int myBulletShootTimerItv = 200;                          // 我方发射一颗子弹的 时间间隔
const int myBulletShootTimerItv1 = 20;                          // 发射超级一颗子弹的 时间间隔
const int enemyBulletShootTimerItv = 1000;                      // 敌方发射一颗子弹的 时间间隔
const int allBulletMoveTimerItv = 10;                           // 所有子弹移动一下的 时间间隔

const int myPlaneMoveTimerItv = 30;                             // 我机移动一次的 时间间隔
const int enemyPlaneMoveTimerItv = 50;                          // 敌机移动一次的 时间间隔
const int enemyPlaneGenerateTimerItv = 3000;                    // 每生成一个敌机 时间间隔
const int bossGenerateTimeItv = 5000;                           // 每生成一个BOSS 时间间隔

/* -------------------------全局静态变量------------------------- */
static QPointF LifeBarPos1 = QPointF(myPlanePos.x()-10, myPlanePos.y()-10); // 玩家 血条坐标（跟随玩家飞机移动）

/* 管理者Control类的构造方法 */
Control::Control()
{
    setSceneRect(0, 0, 800, 600);                               // 设置 游戏场景矩形大小

    this->myPlaneImageFile = ":/images/myplane.png";            // 玩家飞机 资源路径
    this->myLife = 50;                                          // 玩家 初始生命值（满血）
    this->mySkill = 5;                                          // 玩家 初始技能值

    this->myBulletImageFile = ":/images/mybullet.png";          // 玩家子弹 资源路径
    this->myBulletPower = 1;                                    // 玩家 初始杀伤力

    this->enemyPlaneImageFile = ":/images/enemyplane.png";      // 敌机 资源路径
    this->enemyLife = 1;                                        // 敌机 初始生命值（满血）

    this->enemyBulletImageFile = ":/images/enemybullet.png";    // 敌机子弹 资源路径
    this->enemyBulletPower = 1;                                 // 敌机子弹 初始杀伤力

    this->bossImageFile = ":/images/boss.png";                  // BOSS飞机 资源路径
    this->bossLife = 10;                                        // BOSS飞机 初始生命值（满血）

    this->bossBulletImageFile = ":/images/bossbullet.png";      // BOSS子弹 资源路径
    this->bossBulletPower = 2;                                  // BOSS子弹 初始杀伤力

    this->lifeSupplyImageFile = ":/images/lifesupply.png";      // 生命补给 资源路径

    /* 生命值 */
    lifeFrameBar = new QGraphicsRectItem(LifeBarPos.x(), LifeBarPos.y(), myLife*2, 10);
    //    lifeFrameBar->setPen(QPen(Qt::white));
    lifeFrameBar->setBrush(QBrush(Qt::white));
    addItem(lifeFrameBar);
    lifeFrameBar->hide();
    lifeBar = new QGraphicsRectItem(LifeBarPos.x(), LifeBarPos.y(), myLife*2, 10);
    lifeBar->setBrush(QBrush(Qt::green));
    addItem(lifeBar);
    lifeBar->hide();

    /* 血条（跟随玩家移动） */
    lifeFrameBar1 = new QGraphicsRectItem(LifeBarPos1.x(), LifeBarPos1.y(), myLife*2, 7);
    lifeFrameBar1->setPen(QPen(Qt::white));
    addItem(lifeFrameBar1);
    lifeFrameBar1->hide();
    lifeBar1 = new QGraphicsRectItem(LifeBarPos1.x(), LifeBarPos1.y(), myLife*2, 7);
    lifeBar1->setBrush(QBrush(Qt::red));
    addItem(lifeBar1);
    lifeBar1->hide();

    /* 技能值 */
    skillFrameBar = new QGraphicsRectItem(SkillBarPos.x(), SkillBarPos.y(), myLife*2, 10);
    //    skillFrameBar->setPen(QPen(Qt::white));
    skillFrameBar->setBrush(QBrush(Qt::white));
    //    skillFrameBar->setZValue(3);                                  // 设置技能条在最顶层（飞机上方）（不太行，这样它在遮罩面板哪里也显示很明显）
    addItem(skillFrameBar);
    skillFrameBar->hide();
    skillBar = new QGraphicsRectItem(SkillBarPos.x(), SkillBarPos.y(), mySkill*2, 10);
    skillBar->setBrush(QBrush(Qt::blue));
    //    skillBar->setZValue(3);                                       // 设置技能条在最顶层（飞机上方）
    addItem(skillBar);
    skillBar->hide();

    /* 初始化场景，播放背景音乐 */
    playList = new QMediaPlaylist;
    playList->addMedia(QUrl("qrc:///music/starwars.mp3"));          // 添加媒体文件
    //playList->addMedia(QUrl("qrc:///music/shoot.mp3"));
    playList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);   // 单曲循环
    //playList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);     // 只播放一次
    //playList->setPlaybackMode(QMediaPlaylist::Sequential);          // 顺序播放
    //playList->setPlaybackMode(QMediaPlaylist::Loop);                // 整体循环
    //playList->setPlaybackMode(QMediaPlaylist::Random);              // 随机播放
    player = new QMediaPlayer(this);
    player->setPlaylist(playList);
    player->play();


    //    /* 按钮音效路径 */
    //    this->startBtnVoiceFile  = "qrc:///music/getScore.wav";         // 开始按钮 音效路径
    //    this->helpBtnVoiceFile   = "qrc:///music/click4.mp3";           // 帮助按钮 音效路径
    //    this->quitBtnVoiceFile   = "qrc:///music/cancel.wav";           // 退出按钮 音效路径
    //    this->resumeBtnVoiceFile = "qrc:///music/getScore.wav";         // 继续按钮 音效路径
    //    this->retryBtnVoiceFile  = "qrc:///music/getScore.wav";         // 重试按钮 音效路径

    //    btnPlayList = new QMediaPlaylist;                               // 新建 按钮音效 媒体列表
    //    btnPlayList->addMedia(QUrl(this->startBtnVoiceFile));                 // 添加 开始按钮 媒体文件
    //    btnPlayList->addMedia(QUrl(this->helpBtnVoiceFile));                  // 添加 帮助按钮 媒体文件
    //    btnPlayList->addMedia(QUrl(this->quitBtnVoiceFile));                  // 添加 退出按钮 媒体文件
    //    btnPlayList->addMedia(QUrl(this->resumeBtnVoiceFile));                // 添加 继续按钮 媒体文件
    //    btnPlayList->addMedia(QUrl(this->retryBtnVoiceFile));                 // 添加 重试按钮 媒体文件
    //    btnPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);  // 只播放一次
    //    btnPlayer = new QMediaPlayer(this);                             // 新增 按钮音效 播放者
    //    btnPlayer->setPlaylist(btnPlayList);                            // 设置 播放者 媒体列表
    //    btnPlayer->currentMedia()
    //    btnPlayer->play();                                              // 播放媒体

    /* 遮罩面板 */
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 60)));                // R，G，B，不透明度
    mask->resize(width(), height());                                // 重置遮罩面板大小（为当前窗口大小）
    maskWidget = addWidget(mask);                                   // 添加 遮罩面板控件
    maskWidget->setPos(0, 0);                                       // 设置 遮罩面板位置
    maskWidget->setZValue(1);                                       // 设置处在z值为0的图形项上方
    maskWidget->hide();                                             // 隐藏 遮罩面板

    /* 子弹类型/技能/游戏场景 */
    isSuperBullet = false;          // 游戏开始 默认不使用超级子弹
    myBulletType = 0;               // 游戏开始 默认不使用技能
    sceneId = 0;                    // 游戏开始 默认游戏场景 为欢迎界面

    /* 得分提示(右下角) */
    score = 0;                  	// 游戏开始 得分初始化为0
    scoreText = new QGraphicsTextItem;
    scoreText->setPos(scoreTextPos);
    scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
    //    scoreText->setFont(QFont("Courier", 15));
    scoreText->setFont(QFont("Algerian", 10));
    addItem(scoreText);
    scoreText->hide();

    /* 游戏开始按钮 */
    QPushButton *startGameBtn = new QPushButton("开始游戏");
    //startGameBtn->setFont(QFont("Algerian", 18));
    startGameBtn->setFont(QFont("Font Creator Program", 18));
    //startGameBtn->setFont(QFont("方正盛世楷书简体_大", 18));
    //startGameBtn->setStyleSheet("QPushButton{background: transparent; color:white; }"
    //                            "QPushButton:hover{color:red; }");
    // 设置按钮样式：背景为透明、字体颜色white，悬浮后字体颜色变red并且字体放大为30px，按下后按钮下沉（出了点问题，字体默认大小很大，并不是设置的18号，想到一种可行的办法是在下面这行中重新设置以下大小）
    startGameBtn->setStyleSheet("QPushButton{background:transparent; color:white; }"
                                "QPushButton:hover{color:red; font-size:30px; }"
                                "QPushButton:pressed{padding-left:6px; padding-top:6px; }");
//    startGameBtn->setStyleSheet("QPushButton{background-color:rgba(0, 0, 255, 100); color:white; }"
//                                "QPushButton:hover{color:red; font-size:30px; }"
//                                "QPushButton:pressed{padding-left:6px; padding-top:6px; }");
    connect(startGameBtn, SIGNAL(clicked()), this, SLOT(startGame()));
    //    connect(startGameBtn,SIGNAL(pressed()),this,SLOT(playVoice())); // 播放音效
    startGameButton = addWidget(startGameBtn);
    startGameButton->setPos(331, 250);
    startGameButton->setZValue(2);
    startGameButton->hide();

    /* 游戏帮助 */
    QPushButton *helpGameBtn = new QPushButton(tr("游戏帮助"));
    helpGameBtn->setFont(QFont("Font Creator Program", 18));
    helpGameBtn->setStyleSheet("QPushButton{background:transparent; color:white; }"
                               "QPushButton:hover{color:yellow; font-size:30px; }"
                               "QPushButton:pressed{padding-left:6px; padding-top:6px; }");
    connect(helpGameBtn,SIGNAL(clicked()), this, SLOT(showHelpMessage()));
    helpGameButton = addWidget(helpGameBtn);
    helpGameButton->setPos(331, 300);
    helpGameButton->setZValue(2);
    helpGameButton->hide();

    /* 退出游戏 */
    QPushButton *quitGameBtn = new QPushButton(tr("退出游戏"));
    quitGameBtn->setFont(QFont("Font Creator Program", 18));
    quitGameBtn->setStyleSheet("QPushButton{background:transparent; color:white; }"
                               "QPushButton:hover{color:red; font-size:30px; }"
                               "QPushButton:pressed{padding-left:6px; padding-top:6px; }");
    connect(quitGameBtn,SIGNAL(clicked()),this,SLOT(quitGame()));
    quitGameButton = addWidget(quitGameBtn);
    quitGameButton->setPos(331, 350);
    quitGameButton->setZValue(2);
    quitGameButton->hide();

    /* 游戏暂停提示 */
    isPause = false;                                                // 游戏是否为暂停状态（默认为否）
    QPushButton *continueGameBtn = new QPushButton(tr("继续游戏"));  // 新建 继续游戏 按钮
    continueGameBtn->setFont(QFont("Font Creator Program", 18));    // 设置字体、字号
    continueGameBtn->setStyleSheet("QPushButton{background:transparent; color:cyan; }"
                                   "QPushButton:hover{font-size:30px; }"
                                   "QPushButton:pressed{padding-left:6px; padding-top:6px; }");
    connect(continueGameBtn,SIGNAL(clicked()), this, SLOT(pauseGame()));// 信号槽，点击按钮后就响应 暂停游戏方法
    continueGameButton = addWidget(continueGameBtn);                // 添加 继续游戏按钮控件
    continueGameButton->setPos(331, 250);                           // 设置 控件位置
    continueGameButton->setZValue(2);                               // 设置 ZValue值，默认为0。决定相邻项的叠加顺序，值大的绘制在上层
    continueGameButton->hide();                                     // 隐藏 继续游戏按钮

    /* 游戏标题 */
    titleText = new QGraphicsTextItem;                              // 新建 标题 文本项
    addItem(titleText);                                             // 添加 标题 文本项
    titleText->setHtml(tr("<font color=white>Air War Game</font>"));// 设置 文本Html样式
    titleText->setFont(QFont("Algerian", 30));                      // 设置 文本字体
    titleText->setPos(185, 100);                                    // 设置 文本位置
    titleText->setZValue(2);                                        // 设置 ZValue值
    titleText->hide();                                              // 隐藏 标题文本

    /* 作者信息 */
    authorText = new QGraphicsTextItem;                             // 新建 作者信息 文本项
    addItem(authorText);                                            // 添加 作者信息 文本项
    authorText->setHtml(tr("<font color=white>Copyright © 2022,Ge Feng Sheng. All Rights Reserved."));// 设置文本Html样式
    authorText->setFont(QFont("Courier"));                          // 设置 文本字体
    authorText->setPos(10, 500);                                    // 设置 文本位置
    authorText->setZValue(2);                                       // 设置 ZValue值
    authorText->hide();                                             // 隐藏 作者信息文本

    /* 游戏终止提示 */
    gameLostText = new QGraphicsTextItem;                           // 新建 游戏结束提示文本项 文本项
    addItem(gameLostText);                                          // 添加 游戏结束提示文本项 文本项
    gameLostText->setHtml(tr("<font color=white>Game Over! </font>"));// 设置文本Html样式
    gameLostText->setFont(QFont("Algerian", 22));                   // 设置 文本字体
    gameLostText->setPos(100, 130);                                 // 设置 文本位置
    gameLostText->setZValue(2);                                     // 设置 ZValue值
    gameLostText->hide();                                           // 隐藏 游戏结束提示文本

    /* 重试 */
    QPushButton *retryGameBtn = new QPushButton(tr("重新游戏"));
    retryGameBtn->setFont(QFont("Font Creator Program", 18));
    retryGameBtn->setStyleSheet("QPushButton{background:transparent; color:white; }"
                                "QPushButton:hover{color:red; font-size:30px; }"
                                "QPushButton:pressed{padding-left:6px; padding-top:6px; }");
    connect(retryGameBtn,SIGNAL(clicked()), this, SLOT(retryGame()));
    retryGameButton = addWidget(retryGameBtn);
    retryGameButton->setPos(331, 250);
    retryGameButton->setZValue(2);
    retryGameButton->hide();

    /* 进入欢迎界面，之后按Enter键或者按下开始游戏按钮开始游戏 */
    welcomeGame();
}

/* 用于接收计时器消息，实现飞机、子弹定时移动，敌机的定时生成（定时更新） */
void Control::timerEvent(QTimerEvent *event)
{
    if(event->timerId()==myPlaneMoveTimerId){
        changePlanePosition(myplane, myplane->x() + myPlaneMove.x(), myplane->y() + myPlaneMove.y());
//        changeLifeBarPosition(myPlanePos);                                          // 更新血条位置
        changeLifeBarPosition(QPointF(myplane->x(), myplane->y()));                 // 更新血条位置
        updateBar(lifeBar1, lifeFrameBar1, LifeBarPos1, 0, QBrush(Qt::red), false); // 更新血条
    }
    else if(event->timerId()==enemyBulletShootTimerId)
        shootEnemyBullets();
    else if(event->timerId()==myBulletShootTimerId)
        shootBullet();
    else if(event->timerId()==allBulletMoveTimerId)
    {
        updateMyBullets();
        updateEnemyBullets();
    }
    else if(event->timerId()==enemyPlaneMoveTimerId)
        updateEnemyPlanes();
    else if(event->timerId()==enemyPlaneGenerateTimerId)
    {
        for(int i = 0; i < 2; i++)
            generateEnemyPlane();
    }
    else if(event->timerId()==bossGenerateTimeId)
        generateBoss();
    else if(event->timerId()==skillJTimerId) {
        killTimer(skillJTimerId);
        myBulletType = 0;       // 不使用J技能（单发子弹）
    }
    else if(event->timerId()==superBulletTimerId) {
        killTimer(superBulletTimerId);
        isSuperBullet = false;  // 不使用超级子弹
        myBulletShootTimerId = startTimer(myBulletShootTimerItv);
    }
}

/* 用于接收键盘控制，实现玩家控制(键盘响应) */
void Control::keyPressEvent(QKeyEvent *event)
{
    // 按W键/上键，玩家飞机向上移动
    if((event->key()==Qt::Key_W || event->key()==Qt::Key_Up)
        && !event->isAutoRepeat() && sceneId==1)
    {
        if(myPlaneMove==QPointF(0,0))
            myPlaneMoveTimerId = startTimer(myPlaneMoveTimerItv);
        myPlaneMove = QPointF(0, -10);  // 我机上移10像素
    }
    // 按S键/下键，玩家飞机向下移动
    else if((event->key()==Qt::Key_S || event->key()==Qt::Key_Down)
            && !event->isAutoRepeat() && sceneId==1)
    {
        if(myPlaneMove==QPointF(0,0))
            myPlaneMoveTimerId = startTimer(myPlaneMoveTimerItv);
        myPlaneMove = QPointF(0, 10);  // 我机下移10像素
    }
    // 按A键/左键，玩家飞机向左移动
    else if((event->key()==Qt::Key_A || event->key()==Qt::Key_Left)
            && !event->isAutoRepeat() && sceneId==1)
    {
        if(myPlaneMove==QPointF(0,0))
            myPlaneMoveTimerId = startTimer(myPlaneMoveTimerItv);
        myPlaneMove = QPointF(-10, 0);  // 我机左移10像素
    }
    // 按D键/右键，玩家飞机向右移动
    else if((event->key()==Qt::Key_D || event->key()==Qt::Key_Right)
            && !event->isAutoRepeat() && sceneId==1)
    {
        if(myPlaneMove==QPointF(0, 0))
            myPlaneMoveTimerId = startTimer(myPlaneMoveTimerItv);
        myPlaneMove = QPointF(10, 0);  // 我机右移10像素
    }
    // 按J键，触发技能，一次发射3个子弹，但是会消耗5点技能
    else if(event->key()==Qt::Key_J && myplane->skill>=5 && myBulletType==0 && sceneId==1)
    {
        myBulletType = 1;
        myplane->skill -= 5;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -10, QBrush(Qt::blue), true);
        skillJTimerId = startTimer(5000); // 5秒使用时间
    }
    // 按K键，触发技能，可以打掉所有飞机，消耗3点技能值
    else if(event->key()==Qt::Key_K && myplane->skill>=3 && sceneId==1)
    {
        // 用正向迭代器iterator遍历容器
        for(vector<EnemyPlane*>::iterator iter=enemyplanes.begin(); iter!=enemyplanes.end(); iter++)
        {
            score++;    // 得分增加
            scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
            removeItem(*iter);  // *iter就是迭代器iter指向的元素
            delete *iter;
        }
        enemyplanes.clear();
        myplane->skill -= 3;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -6, QBrush(Qt::blue), true);
    }
    // 按L键，触发技能，可以消掉所有敌机子弹，消耗7点技能值
    else if(event->key()==Qt::Key_L && myplane->skill>=7 && sceneId==1)
    {
        for(vector<Bullet*>::iterator it = enemybullets.begin(); it!= enemybullets.end(); it++)
        {
            removeItem(*it);
            delete *it;
        }
        enemybullets.clear();
        myplane->skill -= 7;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -14, QBrush(Qt::blue), true);
    }
    // 按N键，触发技能，可以发射超级子弹，消耗6点技能值
    else if(event->key()==Qt::Key_N && myplane->skill>=6 && isSuperBullet==false && sceneId==1)
    {
        isSuperBullet = true;
        myplane->skill -= 6;
        updateBar(skillBar, skillFrameBar, SkillBarPos, -12, QBrush(Qt::blue), true);
        myBulletShootTimerId = startTimer(myBulletShootTimerItv1);// 使用超级子弹技能（发射子弹时间间隔变为20ms/发）
        superBulletTimerId = startTimer(6000); // 6秒使用时间
    }
    // 按空格键，暂停/继续游戏
    else if(event->key()==Qt::Key_Space && (sceneId==1 || sceneId==2))
        pauseGame();
}

/* 按键释放事件处理函数 */
void Control::keyReleaseEvent(QKeyEvent *event)
{
    if(((event->key()==Qt::Key_W || event->key()==Qt::Key_Up)   ||
        (event->key()==Qt::Key_S || event->key()==Qt::Key_Down) ||
        (event->key()==Qt::Key_A || event->key()==Qt::Key_Left) ||
        (event->key()==Qt::Key_D || event->key()==Qt::Key_Right)) && !event->isAutoRepeat())
    {
        myPlaneMove = QPointF(0, 0);    // 玩家飞机 移动偏移量置0
        killTimer(myPlaneMoveTimerId);  // 停止我机移动计时器
    }
}

/* 生成小敌机，返回是否成功生成 */
bool Control::generateEnemyPlane()
{
    /* 随机在第一行生成敌机 */
    srand(time(NULL));              // 初始化时间种子
    QPixmap pixmap(QPixmap(QString::fromStdString(enemyPlaneImageFile)));
    int cnt = 0;
    //    int x = rand() % (int)width();  // 敌机最左端位置
    int x = rand() % (int)(width() - pixmap.width());  //敌机最左端位置
    for(cnt = 0; cnt < 100; cnt++)
    {
        bool flag = true;           // 此位置是否合法
        // 以iter为暂存变量，循环遍历整个enemyplanes
        for(auto iter:enemyplanes)
        {
            // 如果新敌机位置和旧敌机位置相交，则位置不合法
            if(iter->sceneBoundingRect().intersects(QRectF(x, 0, pixmap.width(), pixmap.height())))
            {
                flag = false;
                break;
            }
        }

        if(flag)
            break;
        else
            //            x = rand() % (int)width();
            x = rand() % (int)(width() - pixmap.width());
    }

    /* 若生成100次随机都未能找到合适的位置则退出 */
    if(cnt >= 100)
        return false;

    /* 新增敌机 */
    EnemyPlane *enemy = new EnemyPlane(x, 0, enemyPlaneImageFile, this, ORD, enemyLife);
    enemyplanes.push_back(enemy);   // 将一个新的元素加到vector的最后面，位置为当前最后一个元素的下一个元素
    /* vector常用函数功能：
     *      pop_back()              // 移除最后一个元素
     *      clear()                 // 清空所有元素
     *      empty()                 // 判断vector是否为空，如果为空返回true
     *      erase()                 // 删除指定元素
     */
    return true;
}

/* 生成BOSS机，返回是否成功生成 */
bool Control::generateBoss()
{
    /* 随机在第一行生成敌机 */
    srand(time(NULL));              // 初始化时间种子
    QPixmap pixmap(QPixmap(QString::fromStdString(bossImageFile)));
    int cnt = 0;
    //    int x = rand() % (int)width();  //BOSS机最左端位置
    int x = rand() % (int)(width() - pixmap.width());  // BOSS机最左端位置
    for(cnt = 0; cnt < 100; cnt++)
    {
        bool flag = true;           // 此位置是否合法
        for(auto iter:enemyplanes)
        {
            // 如果新BOSS机位置和旧BOSS机位置相交，则位置不合法
            if(iter->sceneBoundingRect().intersects(QRectF(x, 0, pixmap.width(), pixmap.height())))
            {
                flag = false;
                break;
            }
        }

        if(flag)
            break;
        else
            //            x = rand() % (int)width();
            x = rand() % (int)(width()-pixmap.width());
    }

    /* 若生成100次随机都未能找到合适的位置则退出 */
    if(cnt >= 100)
        return false;

    /* 新增敌机 */
    EnemyPlane *enemy = new EnemyPlane(x, 0, bossImageFile, this, BOSS, bossLife);
    enemyplanes.push_back(enemy);
    return true;
}

/* 改变飞机位置，返回飞机是否还在战场 */
bool Control::changePlanePosition(Plane *plane, int newX, int newY)
{
    /* 检查位置是否为新位置，是，则直接返回true */
    if (plane->x() == newX && plane->y() == newY)
        return true;

    /* 检查新位置是否合法，不合法则直接返回true */
    //    if (newX<0 || newX>width() || newY<0 || newY>height())
    // 敌机只能往下移动，只检查是否出下界，玩家飞机四个方向均要检测
    if (newX<0 || newX>(width()-plane->width) || newY<0 || newY>(height()-plane->height))
    {
        if (plane->part == ME)          //玩家飞机不允许出界
            return true;
        else if (plane->part == ENEMY)  //敌机不允许出上界、左界、右界，但出下界之后将被删除
        {
            //  if (newY > height())
            if (newY > (height() - plane->height))
            {
                plane->delScreen(this);
                return false;
            }
            else
                return true;
        }
    }

    /* 若为玩家飞机，则首先检查是否与补给碰撞，遇到生命补给则血量+5 */
    if(plane->part == ME)
        for(vector<Object*>::iterator it = lifesupplys.begin(); it != lifesupplys.end(); )
        {
            // collidesWithItem()用于碰撞检测
            if(plane->collidesWithItem(*it))
            {
                // std::min(a,b); 返回给定值a,b中的较小者,a与b相等则返回第一个数a
                plane->life = min(plane->life + 10, myLife);
                updateBar(lifeBar, lifeFrameBar, LifeBarPos, +20, QBrush(Qt::green), true);
                updateBar(lifeBar1, lifeFrameBar1, LifeBarPos1, +20, QBrush(Qt::red), false);
                removeItem(*it);
                delete *it;
                // erase() 删除vector中指定的元素
                it = lifesupplys.erase(it);
            }
            else
                it++;
        }


    /* 检查新位置是否与某一飞机位置冲突 */
    /* 首先检查玩家飞机是否与敌机碰撞 */
    for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); ) //遍历敌机
    {
        if (plane == *it)   // 跳过自己
        {
            it++;
            continue;
        }

        bool alive = true;  // alive表示 it所指向的飞机是否还有生命值
        // collidesWithItem()用于碰撞检测
        if (plane->collidesWithItem(*it))
        {
            /* 若为玩家飞机，两架飞机均crash，生命值都减1 */
            if (plane->part == ME)
            {
                plane->crash(this);
                alive = (*it)->crash(this);
                updateBar(lifeBar, lifeFrameBar, LifeBarPos, -2, QBrush(Qt::green), true);
                updateBar(lifeBar1, lifeFrameBar1, LifeBarPos1, -2, QBrush(Qt::red), false);
            }
            // 这个还没搞懂，为什么敌机与敌机碰撞为什么不允许改变位置？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
            if (plane->part == ENEMY) //若同为敌机，则不允许改变位置，NOCHANGE
                return true;
//            if (plane->part == ENEMY) {//若同为敌机，则不允许改变位置，NOCHANGE
//                plane->update();
//                return true;
//            }
        }

        if (alive)
            it++;
        else // 若飞机坠毁，则将此飞机去掉
        {
            delete *it;
            it = enemyplanes.erase(it);
        }
    }

    /* 若此飞机为敌机（ENEMY：小敌机或BOSS），且与玩家飞机发生了碰撞 */
    // collidesWithItem()用于碰撞检测
    if (plane->part == ENEMY && plane->collidesWithItem(myplane))
    {
        /* 降低相碰撞飞机双方的生命值 */
        myplane->crash(this);
        plane->crash(this);
        /* 更新血条 */
        updateBar(lifeBar, lifeFrameBar, LifeBarPos, -2, QBrush(Qt::green), true);
        updateBar(lifeBar1, lifeFrameBar1, LifeBarPos1, -2, QBrush(Qt::red), false);
    }

    /* 若plane存活，则更改坐标并同步屏幕 */
    if (plane->life > 0)
    {
//        if(plane->part == ME)
//            myPlanePos = QPointF(newX, newY);                   // 获取我机当前坐标

        // moveBy()：为QGraphicsScene提供的一个移动方法，只需传入x和y的移动偏移量
        plane->moveBy(newX - plane->x(), newY - plane->y());
        plane->update();                                        // update()用于刷新屏幕
    }

    return plane->life > 0;
}

/* 根据敌机方向 更新敌机位置 返回玩家飞机是否还有生命值 */
bool Control::updateEnemyPlanes()
{
    /* 若当前敌机少于1，则自动生成敌机，数目随机但小于3 */
    if (enemyplanes.size() < 1)
    {
        int genNum = rand() % 3;
        for (int i = 0; i < genNum; i++)
            generateEnemyPlane();
    }

    /* 所有敌机移动位置 */
    for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end();)
    {
//        // 下一行是打印出当前有几个敌机，及其相应的坐标
//        qDebug() << it-enemyplanes.begin() << "  (" << (*it)->x() << "," << (*it)->y() << ")";
        pair<int, int> loc = (*it)->updatePosition();
        if (changePlanePosition(*it, loc.first, loc.second))
            it++;
        else
            it = enemyplanes.erase(it);
    }

    return myplane->life > 0;
}

/* 改变子弹位置，返回子弹是否还在战场（是否还具有杀伤力） */
bool Control::changeBulletPosition(Bullet * bullet, int newX, int newY)
{
    /* 检查位置是否有变化，无变化则返回true */
    if (bullet->x() == newX && bullet->y() == newY)
        return true;

    /* 检查子弹是否击中某一飞机 */
    /* 首先检查玩家飞机 */
    if (bullet->part==ENEMY && bullet->collidesWithItem(myplane))   // collidesWithItem()用于碰撞检测
    {
        bullet->hit(this);
        myplane->crash(this);
        updateBar(lifeBar, lifeFrameBar, LifeBarPos, -2, QBrush(Qt::green), true);
        updateBar(lifeBar1, lifeFrameBar1, LifeBarPos1, -2, QBrush(Qt::red), false);
        if(myplane->life <= 0)
            loseGame();
    }
    /* 然后检查敌机 */
    else if(bullet->part==ME)
    {
        /* 若敌机已经没有生命值，就从enemyplanes中删去 */
        for (vector<EnemyPlane*>::iterator it = enemyplanes.begin(); it != enemyplanes.end(); )
        {
            bool alive = true;
            // collidesWithItem()用于碰撞检测
            if (bullet->collidesWithItem(*it))
            {
                bullet->hit(this);
                alive = (*it)->crash(this);     // *it就是迭代器it所指向的元素
                myplane->skill++;
                updateBar(skillBar, skillFrameBar, SkillBarPos, +2, QBrush(Qt::blue), true);
                score++;
                scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));
            }

            if(alive)
                it++;
            else
            {
                delete *it;
                it = enemyplanes.erase(it);
                /* 25%的概率掉落生命补给 */
                srand(time(NULL));
                if(rand()%4==0)
                {
                    Object *lifeSupply = new Object(LIFESUPPLY, lifeSupplyImageFile);
                    lifeSupply->setPos(bullet->pos());
                    lifesupplys.push_back(lifeSupply);
                    addItem(lifeSupply);
                }
            }
        }
    }

    /* 若子弹还具有杀伤力，则更新位置并同步屏幕 */
    if (bullet->power > 0)
    {
        /* 若此时子弹试图打出边界，则销毁子弹 */
        if (newX <= 0 || newX >= width() || newY <= 0 || newY >= height())
        {
            bullet->delScreen(this);
            return false;
        }
        bullet->synScreen(this);    // 将子弹显示到屏幕上
        bullet->moveBy(newX - bullet->x(), newY - bullet->y());
        bullet->update();           // update()用于刷新屏幕
    }

    return bullet->power>0;
}

/* 更新敌机所有子弹位置 */
void Control::updateEnemyBullets()
{
    for(vector<Bullet*>::iterator it = enemybullets.begin(); it!=enemybullets.end(); )
    {
        pair<int ,int> loc = (*it)->updatePosition();
        if(changeBulletPosition(*it, loc.first, loc.second))
            it++;
        else
        {
            delete *it;
            it = enemybullets.erase(it);
        }
    }
}

/* 所有敌机发射子弹 */
void Control::shootEnemyBullets()
{
    /* 仍存活的敌机发出新子弹，新子弹在敌机炮管外的位置 */
    for (vector<EnemyPlane*>::iterator iter = enemyplanes.begin(); iter != enemyplanes.end(); iter++)
        if ((*iter)->life > 0)
        {
            // 小敌机（普通敌机）
            if((*iter)->type == ORD)
            {
                Bullet *bullet = new Bullet(ENEMY, (*iter)->x() + (*iter)->pixmap().width()/2 - 7,
                                            (*iter)->y() + (*iter)->pixmap().height() - 15,
                                            enemyBulletImageFile, QPointF(0, 1), enemyBulletPower);
                enemybullets.push_back(bullet);
                addItem(bullet);
            }
            // BOSS机
            else if((*iter)->type == BOSS)
            {
                Bullet *bullet0 = new Bullet(ENEMY, (*iter)->x() + (*iter)->pixmap().width()/2 - 7,
                                             (*iter)->y() + (*iter)->pixmap().height() - 15,
                                             bossBulletImageFile, QPointF(0, 1), bossBulletPower);
                enemybullets.push_back(bullet0);
                addItem(bullet0);

                Bullet *bullet1 = new Bullet(ENEMY, (*iter)->x() + (*iter)->pixmap().width()/2 - 7,
                                             (*iter)->y() + (*iter)->pixmap().height() - 15,
                                             bossBulletImageFile, QPointF(-1, 1), bossBulletPower);
                bullet1->setRotation(45);   // 顺时针旋转45°
                enemybullets.push_back(bullet1);
                addItem(bullet1);

                Bullet *bullet2 = new Bullet(ENEMY, (*iter)->x() + (*iter)->pixmap().width()/2 - 7,
                                             (*iter)->y() + (*iter)->pixmap().height() - 15,
                                             bossBulletImageFile, QPointF(1, 1), bossBulletPower);
                bullet2->setRotation(-45);   // 逆时针旋转45°
                enemybullets.push_back(bullet2);
                addItem(bullet2);
            }
        }
}

/* 更新玩家所有子弹位置 */
void Control::updateMyBullets()
{
    for (vector<Bullet*>::iterator iter = mybullets.begin(); iter != mybullets.end();)
    {
        pair<int, int> loc = (*iter)->updatePosition();
        if (changeBulletPosition(*iter, loc.first, loc.second))
            iter++;
        else
        {
            delete *iter;
            iter = mybullets.erase(iter);
        }
    }
}

/* 玩家飞机发射子弹 */
void Control::shootBullet()
{
    /* 玩家飞机发出新子弹，新子弹在玩家飞机炮管外的位置 */
    // 不使用J技能，单发子弹
    if(myBulletType == 0)
    {
        Bullet *bullet = new Bullet(ME, myplane->x() + 32, myplane->y() - 38,
                                    myBulletImageFile, QPointF(0, -3), myBulletPower);
        mybullets.push_back(bullet);
        addItem(bullet);
    }
    // 使用J技能，3发子弹
    else if(myBulletType == 1)
    {
        /* 左侧子弹 */
        Bullet *bullet1 = new Bullet(ME, myplane->x() + 32, myplane->y() - 38,
                                     myBulletImageFile, QPointF(-3, -3), myBulletPower);
        mybullets.push_back(bullet1);
        bullet1->setRotation(-45);  // 逆时针旋转45°
        addItem(bullet1);

        /* 中间子弹 */
        Bullet *bullet2 = new Bullet(ME, myplane->x() + 32, myplane->y() - 38,
                                     myBulletImageFile, QPointF(0, -3), myBulletPower);
        mybullets.push_back(bullet2);
        addItem(bullet2);

        /* 右侧子弹 */
        Bullet *bullet3 = new Bullet(ME, myplane->x() + 32, myplane->y() - 38,
                                     myBulletImageFile, QPointF(3, -3), myBulletPower);
        mybullets.push_back(bullet3);
        bullet3->setRotation(45);   // 顺时针旋转45°
        addItem(bullet3);
    }
}

/* 更新血条或技能条 */
void Control::updateBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var, const QBrush &barBrush, const bool isFrameBarBrush)
{
    /* var是血条/技能条增量，qreal原型是typedef double qreal; */

    qreal barWidth = bar->rect().width();       // barWidth 血条/技能条的宽度
    /* 更新血条宽度 */
    // std::max(a,b); 返回给定值a,b中的较大者,a与b相等则返回第一个数a
    if(var < 0)
        barWidth = max((qreal)0, barWidth + var);
    // std::min(a,b); 返回给定值a,b中的较小者,a与b相等则返回第一个数a
    else
        barWidth = min(frameBar->rect().width(), barWidth + var);
    /* 画轮廓 */
    frameBar->setRect(pos.x(), pos.y(), 100, frameBar->rect().height());
    if(isFrameBarBrush) {
        frameBar->setBrush(QBrush(Qt::white));  // 轮廓填充色，直接填充整个血条
    }
    else {
        frameBar->setPen(QPen(Qt::white));      // 轮廓填充色，只填充整个轮廓
    }
    frameBar->update();
    /* 画血条 */
    bar->setRect(pos.x(), pos.y(), barWidth, bar->rect().height());
    bar->setBrush(barBrush);
    bar->update();                              // update()用于刷新屏幕
}

/* 改变玩家血条位置（随机移动） */
bool Control::changeLifeBarPosition(QPointF planePos){
    /* 根据飞机当前位置计算出血条的位置 */
    LifeBarPos1 = QPointF(planePos.x() - 10, planePos.y() - 10);
    return true;
}

/* 暂停游戏 */
void Control::pauseGame()
{
    // 如果当前是 非暂停状态
    if(!isPause && sceneId==1){
        sceneId = 2;                            // 游戏场景 设为 暂停界面
        isPause = true;                         // 更改暂停状态为：暂停
        killTimer(myBulletShootTimerId);        // 关闭 玩家子弹射击 计时器
        killTimer(enemyBulletShootTimerId);     // 关闭 敌机子弹射击 计时器
        killTimer(allBulletMoveTimerId);        // 关闭 所有子弹移动 计时器
        killTimer(enemyPlaneMoveTimerId);       // 关闭 敌机移动 计时器
        killTimer(enemyPlaneGenerateTimerId);   // 关闭 生成敌机 计时器
        killTimer(bossGenerateTimeId);          // 关闭 生成BOSS机 计时器
        maskWidget->show();                     // 显示 遮罩面板
        continueGameButton->show();             // 显示 继续游戏按钮
        helpGameButton->show();                 // 显示 游戏帮助按钮
        quitGameButton->show();                 // 显示 退出游戏按钮
    }
    // 如果当前是 暂停状态
    else if(isPause && sceneId==2)
    {
        sceneId = 1;                            // 游戏场景 设为 游戏界面
        isPause = false;                        // 更改暂停状态为：非暂停
        if(false == isSuperBullet) {
            myBulletShootTimerId = startTimer(myBulletShootTimerItv);       // 开启 玩家子弹射击 计时器
        }
        else {
            myBulletShootTimerId = startTimer(myBulletShootTimerItv1);      // 开启 玩家超级子弹 计时器
        }
        enemyBulletShootTimerId = startTimer(enemyBulletShootTimerItv);     // 开启 敌机子弹射击 计时器
        allBulletMoveTimerId = startTimer(allBulletMoveTimerItv);           // 开启 所有子弹移动 计时器
        enemyPlaneMoveTimerId = startTimer(enemyPlaneMoveTimerItv);         // 开启 敌机移动 计时器
        enemyPlaneGenerateTimerId = startTimer(enemyPlaneGenerateTimerItv); // 开启 生成敌机 计时器
        bossGenerateTimeId = startTimer(bossGenerateTimeItv);               // 开启 生成BOSS机 计时器
        maskWidget->hide();                     // 隐藏 遮罩面板
        continueGameButton->hide();             // 隐藏 继续游戏按钮
        helpGameButton->hide();                 // 隐藏 游戏帮助按钮
        quitGameButton->hide();                 // 隐藏 退出游戏按钮
    }
}

/* 玩家生命值用尽 */
void Control::loseGame()
{
    sceneId = 3;                                // 游戏场景 设为 游戏结束
    killTimer(myBulletShootTimerId);            // 关闭 玩家子弹射击 计时器
    killTimer(enemyBulletShootTimerId);         // 关闭 敌机子弹射击 计时器
    killTimer(allBulletMoveTimerId);            // 关闭 所有子弹移动 计时器
    killTimer(enemyPlaneMoveTimerId);           // 关闭 敌机移动 计时器
    killTimer(enemyPlaneGenerateTimerId);       // 关闭 生成敌机 计时器
    killTimer(bossGenerateTimeId);              // 关闭 生成BOSS机 计时器
    maskWidget->show();                         // 显示 遮罩面板
    // 设置 游戏结束提示文本项 样式 好像支持html语法（比如换行就只需要加一个换行标签<br/>）
    gameLostText->setHtml(tr("<font color=white>Game Over! Your Score:</font>"
                             "<font color=red> %1</font>").arg(score));
    gameLostText->show();                       // 显示 游戏结束提示 文本项
    retryGameButton->show();                    // 显示 重新游戏 按钮
    helpGameButton->show();                     // 显示 游戏帮助 按钮
    quitGameButton->show();                     // 显示 退出游戏 按钮
}

/* 重新开始游戏 */
void Control::retryGame()
{
    // 玩家飞机已毁，故不需要再次删除

    // 清空残留敌机
    for(auto iter:enemyplanes)
    {
        removeItem(iter);
        delete iter;
    }
    enemyplanes.clear();

    // 清空玩家残留子弹
    for(auto it:mybullets)
    {
        removeItem(it);
        delete it;
    }
    mybullets.clear();

    // 清空敌机残留子弹
    for(auto it:enemybullets)
    {
        removeItem(it);
        delete it;
    }
    enemybullets.clear();

    // 清空残留补给
    for(auto it:lifesupplys)
    {
        removeItem(it);
        delete it;
    }
    lifesupplys.clear();

    gameLostText->hide();       // 隐藏  游戏失败提示 文本项
    retryGameButton->hide();    // 隐藏  重新开始游戏 按钮
    helpGameButton->hide();     // 隐藏  游戏帮助 按钮
    quitGameButton->hide();     // 隐藏  退出游戏 按钮
    startGame();                // 启动游戏（再次）
}

/* 结束游戏 */
void Control::quitGame()
{
    exit(0);                    // 直接退出
}

/* 初始界面 */
void Control::welcomeGame()
{
    sceneId = 0;                // 游戏场景 设为 欢迎界面
    hasStarted = false;         // 游戏开始标志 初始化为false
    maskWidget->show();         // 显示 遮罩面板
    titleText->show();          // 显示 游戏标题 文本项
    startGameButton->show();    // 显示 开始游戏 按钮
    helpGameButton->show();     // 显示 游戏帮助 按钮
    quitGameButton->show();     // 显示 退出游戏 按钮
    authorText->show();         // 显示 作者信息 文本项
}

/* 开始（启动）游戏 */
void Control::startGame()
{
    sceneId = 1;                                                                                // 游戏场景 设为 游戏界面
    score = 0;                                                                                  // 游戏初始得分
    scoreText->setHtml(tr("<font color=white>SCORE: %1</font>").arg(score));                    // 设置游戏得分文本项 样式

    hasStarted = true;                                                                          // 游戏开始 标志 改为true
    titleText->hide();                                                                          // 隐藏 游戏标题 文本项
    authorText->hide();                                                                         // 隐藏 作者信息 文本项
    startGameButton->hide();                                                                    // 隐藏 开始游戏 按钮
    helpGameButton->hide();                                                                     // 隐藏 游戏帮助 按钮
    quitGameButton->hide();                                                                     // 隐藏 退出游戏 按钮
    maskWidget->hide();                                                                         // 隐藏 遮罩面板

    /* 分数，血条，技能条 */
    scoreText->show();                                                                          // 显示 游戏得分 文本项

    lifeFrameBar->show();                                                                       // 显示 生命条 边框
    lifeBar->setRect(LifeBarPos.x(), LifeBarPos.y(), myLife*2, lifeBar->rect().height());       // 设置 生命条 位置、宽高
    lifeBar->setBrush(Qt::green);                                                               // 设置 生命条 填充色
    lifeBar->update();                                                                          // 更新 生命条 update()用于刷新屏幕
    lifeBar->show();                                                                            // 显示 生命条

    lifeFrameBar1->show();                                                                      // 显示 生命条1 边框
    lifeBar1->setRect(LifeBarPos1.x(), LifeBarPos1.y(), myLife*2, lifeBar1->rect().height());   // 设置 生命条1 位置、宽高
    lifeBar1->setBrush(Qt::red);                                                                // 设置 生命条1 填充色
    lifeBar1->update();                                                                         // 更新 生命条1 update()用于刷新屏幕
    lifeBar1->show();                                                                           // 显示 生命条1

    skillFrameBar->show();                                                                      // 显示 杀伤力条 边框
    skillBar->setRect(SkillBarPos.x(), SkillBarPos.y(), mySkill*2, skillBar->rect().height());  // 设置 杀伤力条 位置、宽高
    skillBar->setBrush(Qt::blue);                                                               // 设置 杀伤力条 填充色
    skillBar->update();                                                                         // 更新 杀伤力条 update()用于刷新屏幕
    skillBar->show();                                                                           // 显示 杀伤力条


    /* 设置各动作更新时钟 */
    myPlaneMove = QPointF(0, 0);
    myBulletShootTimerId = startTimer(myBulletShootTimerItv);
    enemyBulletShootTimerId = startTimer(enemyBulletShootTimerItv);
    allBulletMoveTimerId = startTimer(allBulletMoveTimerItv);
    enemyPlaneMoveTimerId = startTimer(enemyPlaneMoveTimerItv);
    enemyPlaneGenerateTimerId = startTimer(enemyPlaneGenerateTimerItv);
    bossGenerateTimeId = startTimer(bossGenerateTimeItv);

    /* 添加玩家飞机 */
    myplane = new MyPlane(myPlanePos.x(), myPlanePos.y(), myPlaneImageFile, this, myLife, mySkill); // 添加玩家飞机在屏幕中央靠下一点的位置
    myplane->synScreen(this);                                                                       // 将玩家飞机显示到屏幕上

    /* 添加敌机 */
    for (int i = 0; i < 3; i++)
        generateEnemyPlane();
}

///* 播放音效 */
//void Control::playVoice(QMediaPlayer player, const string &voiceFile){
//    player->currentMedia(voiceFile);
//    player->play();
//}

/* 显示游戏帮助信息 */
void Control::showHelpMessage()
{
    QString helpText;
    helpText += tr("欢迎来到飞机大战！\n\n");
    helpText += tr("游戏规则：\n");
    helpText += tr("    1. 当你击落敌机时，你的得分和技能值都会相应地增加\n");
    helpText += tr("    2. 若你被敌机击中或撞到，双方生命值均会相应减少\n");
    helpText += tr("    3. 当你的技能值达到一定量的时候，便可释放技能\n\n");
    helpText += tr("操作：\n");
    helpText += tr("    W：向上移动； S：向下移动；\n");
    helpText += tr("     A：向左移动； D：向右移动；\n");
    helpText += tr("    空格键：暂停或继续游戏\n\n");
    helpText += tr("技能：\n");
    helpText += tr("    J：无敌子弹技能，一次发射三个方向的子弹，消耗5点技能\n");
    helpText += tr("    K：轰炸技能，一次性炸掉所有敌机，消耗3点技能\n");
    helpText += tr("    L：子弹拦截技能，一次性拦截掉所有子弹，消耗7点技能\n");
    helpText += tr("    N：触发超级子弹，威力为普通子弹的10倍，消耗6点技能\n\n");
    helpText += tr("进度条：\n");
    helpText += tr("    1. 绿色进度条：你的生命值\n");
    helpText += tr("    2. 蓝色进度条：你的技能值\n\n");
    helpText += tr("补给：\n");
    helpText += tr("    被击毁的敌机随机（25%）生成补给，即绿色的加号，每次可补充10点生命值");
    QMessageBox * messageBox = new QMessageBox;
    messageBox->setStyleSheet("QLabel {color:blue; font-size:15px; }");
    messageBox->setWindowTitle("游戏帮助");
    messageBox->setText(helpText);
    messageBox->show();

    //messageBox->information(NULL, tr("游戏帮助"), helpText);
    // information原型：
    /* static StandardButton information(QWidget *parent, const QString &title,
         const QString &text, StandardButtons buttons = Ok,
         StandardButton defaultButton = NoButton); */
}

//void Control::showHelpMessage()
//{
//    QLabel *welcomeLabel = new QLabel("欢迎来到飞机大战！");
//    QGraphicsWidget *wel = this->addWidget(welcomeLabel);
//    wel->setPos(0, 20);
//    wel->setZValue(2);
//    wel->show();

//    QLabel *ruleLabel = new QLabel;
//    ruleLabel->setText("游戏规则：\n"
//                       "    1. 当你击落敌机时，你的得分和技能值都会相应地增加\n"
//                       "    2. 若你被敌机击中或撞到，你的生命值会相应减少\n"
//                       "    3. 当你的技能值达到一定量的时候，便可释放技能\n\n");
//    QString helpText;
// //    helpText += tr("欢迎来到飞机大战！\n\n");
// //    helpText += tr("游戏规则：\n");
// //    helpText += tr("    1. 当你击落敌机时，你的得分和技能值都会相应地增加\n");
// //    helpText += tr("    2. 若你被敌机击中或撞到，你的生命值会相应减少\n");
// //    helpText += tr("    3. 当你的技能值达到一定量的时候，便可释放技能\n\n");
//    helpText += tr("操作：\n");
//    helpText += tr("    W：向上移动； S：向下移动；\n");
//    helpText += tr("     A：向左移动； D：向右移动；\n");
//    helpText += tr("    空格键：暂停/继续游戏\n\n");
//    helpText += tr("技能：\n");
//    helpText += tr("    J：无敌子弹技能，一次发射三个方向的子弹，消耗5点技能\n");
//    helpText += tr("    K：轰炸技能，一次性炸掉所有敌机，消耗3点技能\n");
//    helpText += tr("    L：子弹拦截技能，一次性拦截掉所有子弹，消耗7点技能\n");
//    helpText += tr("    N：触发超级子弹，威力为普通子弹的10倍，消耗6点技能\n\n");
//    helpText += tr("进度条：\n");
//    helpText += tr("    1. 绿色进度条：你的生命值\n");
//    helpText += tr("    2. 蓝色进度条：你的技能值\n\n");
//    helpText += tr("补给：\n");
//    helpText += tr("    被击毁的敌机随机（25%）生成补给，即绿色的加号，每次可补充10点生命值");
//    QMessageBox * messageBox = new QMessageBox;
// //    messageBox->information(NULL,tr("游戏帮助"),helpText);


//    messageBox->setStyleSheet("QLabel {background-image:url(:/images/help.bmp);"
//                              "color:blue; font-size:15px; }");
//    messageBox->setWindowTitle("游戏帮助");
//    messageBox->setText(helpText);
//    messageBox->show();
//    messageBox->adjustSize();
//    ruleLabel->show();
//}
