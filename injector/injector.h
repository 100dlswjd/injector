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
    void process_list_item_click_handler();
    void file_search_btn_click_handler();
    void inject_btn_click_handler();

private:
    Ui::injectorClass ui;
};
