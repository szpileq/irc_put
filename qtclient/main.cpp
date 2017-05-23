#include "client.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    client ircClient;
    ircClient.show();

    return a.exec();
}
