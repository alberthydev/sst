//#include "principal.h"
#include <QApplication>
#include <QDebug>
#include "principal.h"
#include "databasemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager::getInstance();

    Principal w;
    w.show();

    return a.exec();
}
