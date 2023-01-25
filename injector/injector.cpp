#include "injector.h"
#include <Qstring>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>


QString process_name;
QString dll_path;
QString temp = "";

injector::injector(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton_process_refresh, &QPushButton::clicked, this, &injector::process_refresh_btn_click_handler);
    connect(ui.pushButton_file_search, &QPushButton::clicked, this, &injector::file_search_btn_click_handler);
    connect(ui.pushButton_inject, &QPushButton::clicked, this, &injector::inject_btn_click_handler);
    connect(ui.listWidget_process_list, &QListWidget::itemClicked, this, &injector::process_list_item_click_handler);
    process_refresh_btn_click_handler();
}

injector::~injector()
{}

bool injector::get_process_list(PROCESSENTRY32* proc_list, int size, int *return_size)
{
    if (size == 0)
        return FALSE;
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (hSnapShot == INVALID_HANDLE_VALUE)
        return FALSE;
    PROCESSENTRY32 pe = { 0, };
    pe.dwSize = sizeof(pe);

    if (!Process32First(hSnapShot, &pe))
    {
        CloseHandle(hSnapShot);
        return FALSE;
    }

    int cur_idx = 0;
    do
    {
        /*
        process_name = QString::fromWCharArray(pe.szExeFile);
        ui.listWidget_process_list->addItem(process_name);
        */
        proc_list[cur_idx++] = pe;
        if (cur_idx >= size)
            break;
    } while (Process32Next(hSnapShot, &pe));
    *return_size = cur_idx;
    CloseHandle(hSnapShot);
    return TRUE;
}

void injector::process_refresh_btn_click_handler()
{ 
    ui.listWidget_process_list->clear();
    //PROCESSENTRY32 pe[320];
    //int size = sizeof(pe) / sizeof(PROCESSENTRY32);

    int size = 255;
    PROCESSENTRY32* pe = (PROCESSENTRY32*)malloc(sizeof(PROCESSENTRY32) * size);

    if (pe == NULL)
        return;

    int return_size = 0;
    if (!get_process_list(pe, size, &return_size))
    {
        free(pe);
        return;
    }

    for (int idx = 0; idx < return_size ; idx++)
    {
        QString name = QString::fromWCharArray(pe[idx].szExeFile);
        ui.listWidget_process_list->addItem(name);
    } 
    free(pe);
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

bool inject_dll(DWORD dwPID, QString DllPath)
{
    HANDLE hProcess = NULL, hThread = NULL;
    HMODULE hMod = NULL;
    LPVOID pRemoteBuf = NULL;
    QByteArray bArray = DllPath.toLatin1();
    DWORD dwBufSize = (DWORD)bArray.size() * sizeof(wchar_t);
    LPTHREAD_START_ROUTINE pThreadProc;


    if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
        return FALSE;

    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);
    WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)DllPath.toStdWString().c_str(), dwBufSize, NULL);

    hMod = GetModuleHandle(L"kernel32.dll");
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");

    hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);
    return TRUE;
}

void injector::inject_btn_click_handler()
{ 
    DWORD dwPID = 0;
    //PROCESSENTRY32 pe[320];
    //int size = sizeof(pe) / sizeof(PROCESSENTRY32);

    int size = 255;
    PROCESSENTRY32* pe = (PROCESSENTRY32*)malloc(sizeof(PROCESSENTRY32) * size);

    if (pe == NULL)
        return;

    int return_size = 0;
    if (!get_process_list(pe, size, &return_size))
    {
        free(pe);
        return;
    }

    for (int idx = 0; idx < return_size; idx++)
    {
        if (process_name == QString::fromWCharArray(pe[idx].szExeFile))
        {
            dwPID = pe[idx].th32ProcessID;
            inject_dll(dwPID, dll_path);
        }
    }
    
    /*
    DWORD dwPID = 0;
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (hSnapShot == INVALID_HANDLE_VALUE)
        return;

    PROCESSENTRY32 pe = { 0, };
    pe.dwSize = sizeof(pe);

    Process32First(hSnapShot, &pe);
    
    do
    {
        if (process_name == QString::fromWCharArray(pe.szExeFile))
        {
            dwPID = pe.th32ProcessID;
            inject_dll(dwPID, dll_path);
        }
    } while (Process32Next(hSnapShot, &pe));
    CloseHandle(hSnapShot);
    */
    ui.pushButton_inject->setEnabled(FALSE);
    free(pe);
}