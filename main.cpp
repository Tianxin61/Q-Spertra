//#include "MainWindow.h"
#include"LoginDialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    //程序运行的第一个界面是登录界面
    LoginDialog ld;
    ld.show();
    return a.exec();
}
