#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_injector.h"

class injector : public QMainWindow
{
    Q_OBJECT

public:
    injector(QWidget *parent = nullptr);
    ~injector();

public slots:
    void process_refresh_btn_click_handler();
    void fild_search_btn_click_handler();
    void inject_btn_click_handler();
private:
    Ui::injectorClass ui;
};
