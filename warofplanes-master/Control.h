#ifndef CONTROL_H
#define CONTROL_H

#include <ctime>
#include <vector>
#include <cassert>
#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <windows.h>

#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "Plane.h"
#include "MyPlane.h"
#include "EnemyPlane.h"
using namespace std;

// Control类继承自 QGraphicsScene
class Control: public QGraphicsScene
{
    Q_OBJECT
public:
    /* Control类 的构造函数 */
	Control();

protected:
    /* 事件处理 */
    void timerEvent(QTimerEvent *event);    // 用于接收计时器消息，实现飞机、子弹定时移动，敌机的定时生成（定时更新）
    void keyPressEvent(QKeyEvent *event);   // 用于接收键盘控制，实现玩家控制（键盘响应）
    void keyReleaseEvent(QKeyEvent *event); // 按钮释放事件（重写）

private:
    string myPlaneImageFile;                // 我方飞机 资源路径
    QPixmap m_pixmap;                       // 玩家飞机 图片类
//    QPointF myPlanePos;                     // 玩家飞机位置
    int myLife;                             // 我方飞机 生命值（血条）
    int mySkill;                            // 我方飞机 杀伤力（技能条）

    string myBulletImageFile;               // 我方子弹 资源路径
    int myBulletPower;                      // 我方子弹 杀伤力值

    string enemyPlaneImageFile;             // 敌方飞机 资源路径
    int enemyLife;                          // 敌方飞机 生命值

    string enemyBulletImageFile;            // 敌方子弹 资源路径
    int enemyBulletPower;                   // 敌方子弹 杀伤力值

    string bossImageFile;                   // BOSS机 资源路径
    int bossLife;                           // BOSS机 生命值

    string bossBulletImageFile;             // BOSS子弹 资源路径
    int bossBulletPower;                    // BOSS子弹 杀伤力

    int myBulletShootTimerId;               // 我机发射子弹 计时器ID？
    int enemyBulletShootTimerId;            // 敌机发射子弹 计时器ID？
    int allBulletMoveTimerId;               // 所有子弹移动 计时器ID？

    QPointF myPlaneMove;                    // 我机移动 偏移量
    int myPlaneMoveTimerId;                 // 我机移动 计时器ID？  时间？
    int enemyPlaneMoveTimerId;              // 敌机移动 计时器ID？  时间？
    int enemyPlaneGenerateTimerId;          // 生成敌机 计时器ID？  时间？
    int bossGenerateTimeId;                 // 生成BOSS 计时器ID？ 时间？

    string lifeSupplyImageFile;             // 生命补给 资源路径

    MyPlane *myplane;                       // 我方飞机
    vector<Bullet*> mybullets;              // 我方子弹
    vector<EnemyPlane *> enemyplanes;       // 敌方飞机
    vector<Bullet*> enemybullets;           // 敌方子弹
    vector<Object*> lifesupplys;            // 生命补给

    /* 血条、技能条 */
    QGraphicsRectItem *lifeFrameBar;        // 生命边框条
    QGraphicsRectItem *lifeBar;             // 生命条
    QGraphicsRectItem *lifeFrameBar1;       // 生命边框条（跟随玩家飞机移动）
    QGraphicsRectItem *lifeBar1;            // 生命条（跟随玩家飞机移动）
    QGraphicsRectItem *skillFrameBar;       // 杀伤力边框条
    QGraphicsRectItem *skillBar;            // 杀伤力条

    /* 媒体、音效 */
    QMediaPlayer *player;                   // QMediaPlayer 类允许播放媒体源
    QMediaPlaylist *playList;               // QMediaPlaylist 类提供了要播放的媒体内容列表

    QMediaPlayer *btnPlayer;               // 播放按钮音效
    QMediaPlaylist *btnPlayList;           // 播放按钮音效

    string startBtnVoiceFile;               // 开始按钮 音效路径
    string helpBtnVoiceFile;                // 帮助按钮 音效路径
    string quitBtnVoiceFile;                // 退出按钮 音效路径
    string resumeBtnVoiceFile;              // 继续按钮 音效路径
    string retryBtnVoiceFile;               // 重试按钮 音效路径

    bool generateEnemyPlane();              // 生成一架敌机
    bool generateBoss();                    // 生成Boss
	
    bool changePlanePosition(Plane *plane, int newX, int newY);// 改变飞机位置，返回飞机是否还在战场？
    bool updateEnemyPlanes();               // 根据敌机方向 更新敌机位置
	
    bool changeBulletPosition(Bullet *bullet, int newX, int newY); // 改变子弹位置，返回子弹是否还在战场
    void shootEnemyBullets();               // 所有敌机 发射子弹
    void updateMyBullets();                 // 更新玩家 所有子弹
    void updateEnemyBullets();              // 更新敌机 所有子弹
    void shootBullet();                     // 玩家飞机 发射子弹

    bool changeLifeBarPosition(QPointF planePos);    // 改变玩家血条位置（随机移动）
    void updateBar(QGraphicsRectItem *bar, QGraphicsRectItem *frameBar, const QPointF &pos, qreal var, const QBrush &barBrush, const bool isFrameBarBrush);    // 更新血条或技能条

    bool hasStarted;                        // 是否已经开始游戏
    bool isPause;                           // 游戏是否暂停
    bool isSuperBullet;                     // 是否使用超级子弹
    int superBulletTimerId;                 // 超级子弹使用时间
    int myBulletType;                       // 是否使用J技能，0不使用/1使用
    int skillJTimerId;                      // 技能J使用时间
    int sceneId;                            // 游戏场景，0欢迎界面/1游戏界面/2暂停界面/3游戏结束
    int score;                              // 游戏得分（打掉的小敌机数，或者说敌机中弹次数）

	// 游戏标题、提示信息等
    QGraphicsTextItem *titleText;           // 游戏标题
    QGraphicsTextItem *authorText;          // 作者信息
    QGraphicsTextItem *scoreText;           // 玩家当前打掉的飞机数 文本项
    QGraphicsTextItem *gameLostText;        // 玩家死亡提示 文本项
    QGraphicsTextItem *gameHelpText;        // 游戏帮助 文本项
    void welcomeGame();                     // 初始界面

	// 游戏按钮、遮罩面板（用于在游戏暂停时达到场景变暗的效果）
    QGraphicsWidget *maskWidget;            // 遮罩面板
    QGraphicsWidget *startGameButton;       // 游戏开始按钮
    QGraphicsWidget *helpGameButton;        // 游戏帮助按钮
    QGraphicsWidget *continueGameButton;    // 继续游戏按钮
    QGraphicsWidget *retryGameButton;       // 重新游戏按钮
    QGraphicsWidget *quitGameButton;        // 退出游戏按钮

/* Control类 的槽函数 */
protected slots:
    void startGame();                       // 开始游戏
    void showHelpMessage();                 // 显示帮助信息
    void pauseGame();                       // 暂停游戏
    void loseGame();                        // 玩家生命值用尽
    void retryGame();                       // 重新开始
    void quitGame();                        // 结束游戏
//    void playVoice(QMediaPlayer player, const string &voiceFile);// 播放音效
};

#endif // !CONTROL_H
