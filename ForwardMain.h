#ifndef FORWARDMAIN_H
#define FORWARDMAIN_H

#include <QWidget>

#include "zlnetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ForwardMain; }
QT_END_NAMESPACE

class ForwardMain : public QWidget
{
    Q_OBJECT

public:
    ForwardMain(QWidget *parent = nullptr);
    ~ForwardMain();

private:
    Ui::ForwardMain *ui;
};
#endif // FORWARDMAIN_H
