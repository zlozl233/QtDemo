#include "ForwardMain.h"
#include "ui_ForwardMain.h"

ForwardMain::ForwardMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForwardMain)
{
    ui->setupUi(this);
}

ForwardMain::~ForwardMain()
{
    delete ui;
}

