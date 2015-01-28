#include "interface.hpp"
#include "../drone/QAtCmd.hpp"

MyWidget::MyWidget(QWidget *parent) : QWidget(parent) {
    resize(800, 700);

    QAtCmd *qAtCmd = new QAtCmd;

    /*QPushButton *config = new QPushButton("Config");
    connect(config, SIGNAL(clicked()), qAtCmd, SLOT(AtCmd::sendConfig("video:video_channel", "0")));*/

    QPushButton *takeOff = new QPushButton("TakeOff");
    QPushButton *landing = new QPushButton("Land");
    QPushButton *emergency = new QPushButton("Emergency");
    QPushButton *goUp = new QPushButton("Go Up");
    QPushButton *goDown = new QPushButton("Go Down");
    QPushButton *moveRight = new QPushButton("Move Right");
    QPushButton *moveLeft = new QPushButton("Move Left");
    QPushButton *moveBW = new QPushButton("Move Backwards");
    QPushButton *moveFW = new QPushButton("Move Forward");
    QPushButton *fTrim = new QPushButton("FTrim");
    QPushButton *yawRight = new QPushButton("yawRight");
    QPushButton *yawLeft = new QPushButton("yawLeft");

    QShortcut *takeOffsc = new QShortcut(QKeySequence(tr("Return")), takeOff);
    QObject::connect(takeOffsc, SIGNAL(activated()), qAtCmd, SLOT(sendTakeOff()));
    QShortcut *landingsc = new QShortcut(QKeySequence(tr("Shift+Return")), landing);
    QObject::connect(landingsc, SIGNAL(activated()), qAtCmd, SLOT(sendLanding()));
    QShortcut *emergencysc = new QShortcut(QKeySequence(tr("Escape")), emergency);
    QObject::connect(emergencysc, SIGNAL(activated()), qAtCmd, SLOT(sendEmergency()));
    QShortcut *goUpsc = new QShortcut(QKeySequence(tr("Space")), goUp);
    QObject::connect(goUpsc, SIGNAL(activated()), qAtCmd, SLOT(goUp()));
    QShortcut *goDownsc = new QShortcut(QKeySequence(tr("Shift+Space")), goDown);
    QObject::connect(goDownsc, SIGNAL(activated()), qAtCmd, SLOT(goDown()));
    QShortcut *moveRightsc = new QShortcut(QKeySequence("d"), moveRight);
    QObject::connect(moveRightsc, SIGNAL(activated()), qAtCmd, SLOT(moveRight()));
    QShortcut *moveLeftsc = new QShortcut(QKeySequence(tr("q")), moveLeft);
    QObject::connect(moveLeftsc, SIGNAL(activated()), qAtCmd, SLOT(moveLeft()));
    QShortcut *moveBWsc = new QShortcut(QKeySequence(tr("s")), moveBW);
    QObject::connect(moveBWsc, SIGNAL(activated()), qAtCmd, SLOT(moveBW()));
    QShortcut *moveFWsc = new QShortcut(QKeySequence(tr("z")), moveFW);
    QObject::connect(moveFWsc, SIGNAL(activated()), qAtCmd, SLOT(moveFW()));
    QShortcut *ftrimsc = new QShortcut(QKeySequence(tr("t")), fTrim);
    QObject::connect(ftrimsc, SIGNAL(activated()), qAtCmd, SLOT(sendFTrim()));
    QShortcut *yawRightsc = new QShortcut(QKeySequence(tr("a")), yawRight);
    QObject::connect(yawRightsc, SIGNAL(activated()), qAtCmd, SLOT(yawRight()));
    QShortcut *yawLeftsc = new QShortcut(QKeySequence(tr("e")), yawLeft);
    QObject::connect(yawLeftsc, SIGNAL(activated()), qAtCmd, SLOT(yawLeft()));


    connect(takeOff, SIGNAL(clicked()), qAtCmd, SLOT(sendTakeOff()));
    connect(landing, SIGNAL(clicked()), qAtCmd, SLOT(sendLanding()));
    connect(emergency, SIGNAL(clicked()), qAtCmd, SLOT(sendEmergency()));
    connect(goUp, SIGNAL(clicked()), qAtCmd, SLOT(goUp()));
    connect(goDown, SIGNAL(clicked()), qAtCmd, SLOT(goDown()));
    connect(moveRight, SIGNAL(clicked()), qAtCmd, SLOT(moveRight()));
    connect(moveLeft, SIGNAL(clicked()), qAtCmd, SLOT(moveLeft()));
    connect(moveBW, SIGNAL(clicked()), qAtCmd, SLOT(moveBW()));
    connect(moveFW, SIGNAL(clicked()), qAtCmd, SLOT(moveFW()));
    connect(yawRight, SIGNAL(clicked()), qAtCmd, SLOT(yawRight()));
    connect(yawLeft, SIGNAL(clicked()), qAtCmd, SLOT(yawLeft()));
    connect(fTrim, SIGNAL(clicked()), qAtCmd, SLOT(sendFTrim())); 

    takeOff->setMinimumWidth(80);
    landing->setMinimumWidth(80);
    emergency->setMinimumWidth(80);
    goUp->setMinimumWidth(80);
    goDown->setMinimumWidth(80);
    moveRight->setMinimumWidth(80);
    moveLeft->setMinimumWidth(80);
    moveBW->setMinimumWidth(80);
    moveFW->setMinimumWidth(80);
    fTrim->setMinimumWidth(80);
    yawRight->setMinimumWidth(80);
    yawLeft->setMinimumWidth(80);

    takeOff->setMinimumHeight(80);
    landing->setMinimumHeight(80);
    emergency->setMinimumHeight(80);
    goUp->setMinimumHeight(80);
    goDown->setMinimumHeight(80);
    moveRight->setMinimumHeight(80);
    moveLeft->setMinimumHeight(80);
    moveBW->setMinimumHeight(80);
    moveFW->setMinimumHeight(80);
    fTrim->setMinimumHeight(80);
    yawRight->setMinimumHeight(80);
    yawLeft->setMinimumHeight(80);

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(takeOff, 0, 0, 1, 2);
    grid->addWidget(goUp, 1, 0, 1, 2);
    grid->addWidget(moveFW, 2, 0, 1, 2);
    grid->addWidget(moveLeft, 3, 0);
    grid->addWidget(moveRight, 3, 1);
    grid->addWidget(moveBW, 4, 0, 1, 2);
    grid->addWidget(goDown, 5, 0, 1, 2);
    grid->addWidget(yawRight, 6, 0);
    grid->addWidget(yawLeft, 6, 1);
    grid->addWidget(landing, 8, 0);
    grid->addWidget(emergency, 8, 1);
    grid->addWidget(fTrim, 9, 0, 1, 2);
                                                           
    QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget();
    layout->addLayout(grid);                                                             
    setLayout(layout);
}


