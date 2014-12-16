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


    connect(takeOff, SIGNAL(clicked()), qAtCmd, SLOT(sendTakeOff()));
    connect(landing, SIGNAL(clicked()), qAtCmd, SLOT(sendLanding()));
    connect(emergency, SIGNAL(clicked()), qAtCmd, SLOT(sendEmergency()));
    connect(goUp, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, 0, 1, 0)));
    connect(goDown, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, O, -1, 0)));
    connect(moveRight, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 1, 0, 0, 0, 0)));
    connect(moveLeft, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, -1, 0, 0, 0, 0)));
    connect(moveBW, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, -1, 0, 0)));
    connect(moveFW, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, 1, 0, 0)));
    connect(moveFW, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, 1, 0, 0)));
    connect(yawRight, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, 0, 0, 1)));
    connect(yawLeft, SIGNAL(clicked()), qAtCmd, SLOT(sendMovement(3, 0, 0, 0, -1)));

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


