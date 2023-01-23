#include "injector.h"
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>
#include <Windows.h>
#include <TlHelp32.h>

QString process_name;
QString dll_path;
QString temp = "";

injector::injector(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_process_refresh, &QPushButton::clicked, this, &injector::process_refresh_btn_click_handler);
    connect(ui.pushButton_file_search, &QPushButton::clicked, this, &injector::file_search_btn_click_handler);
    connect(ui.listWidget_process_list, &QListWidget::itemClicked, this, &injector::process_list_item_click_handler);
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

void injector::process_list_item_click_handler()
{
    QListWidgetItem* item = ui.listWidget_process_list->currentItem();
    ui.lineEdit_process_name->setText(item->text());
    process_name = item->text();

    if (dll_path != temp)
    {
        ui.pushButton_inject->setEnabled(TRUE);
    }
}

void injector::file_search_btn_click_handler()
{ 
    QFileDialog dialog(this);
    QStringList fileNames;
    dialog.setNameFilter(tr("dll file (*.dll)"));
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
        dll_path = fileNames.join("");
        ui.lineEdit_dll_path->setText(dll_path);

        if (process_name != temp)
        {
            ui.pushButton_inject->setEnabled(TRUE);
        }
    }
}

void injector::inject_btn_click_handler()
{ }