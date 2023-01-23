#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_injector.h"

class injector : public QMainWindow
{
    Q_OBJECT

public:
    injector(QWidget *parent = nullptr);
    ~injector();

private:
    Ui::injectorClass ui;
};
