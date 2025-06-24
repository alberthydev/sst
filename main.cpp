#include <QApplication>
#include <QDebug>
#include "principal.h"
#include "databasemanager.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Garante que inicializa o banco
    DatabaseManager::getInstance();

    Principal w;
    w.show();

    return a.exec();
}
