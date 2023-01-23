#include "injector.h"
#include <Windows.h>
#include <TlHelp32.h>

injector::injector(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_process_refresh, &QPushButton::clicked, this, &injector::process_refresh_btn_click_handler);
}

injector::~injector()
{}

void injector::process_refresh_btn_click_handler()
{ 
    ui.listWidget_process_list->clear();
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hSnapShot == INVALID_HANDLE_VALUE)
        return;
    PROCESSENTRY32 pe = { 0, };
    pe.dwSize = sizeof(pe);

    if (!Process32First(hSnapShot, &pe))
    {
        CloseHandle(hSnapShot);
        return;
    }

    do
    {
        QString process_name = QString::fromWCharArray(pe.szExeFile);
        ui.listWidget_process_list->addItem(process_name);
    } while (Process32Next(hSnapShot, &pe));
    CloseHandle(hSnapShot);
}


void injector::fild_search_btn_click_handler()
{ }

void injector::inject_btn_click_handler()
{ }