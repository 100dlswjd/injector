#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_injector.h"
#include <Windows.h>
#include <TlHelp32.h>

class injector : public QMainWindow
{
    Q_OBJECT

public:
    injector(QWidget *parent = nullptr);
    ~injector();

    QString process_name;
    QString dll_path;
    QString temp = "";
    bool get_process_list(PROCESSENTRY32* proc_list, int size, int *return_size);

public slots:
    void process_refresh_btn_click_handler();
    void process_list_item_click_handler();
    void file_search_btn_click_handler();
    void inject_btn_click_handler();

private:
    Ui::injectorClass ui;
};
