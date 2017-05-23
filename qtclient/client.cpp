#include "client.h"
#include "ui_client.h"
client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    socket = -1;
    ui->setupUi(this);
    ui->portNumber->setValidator(new QIntValidator(1024,65535,this));

    ui->nickEdit->setMaxLength(POORIRC_NICKNAME_MAX_LEN);
    ui->sendMessage->setMaxLength(POORIRC_MSG_MAX_LEN-1);

    //IPv4 validation
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    ui->ipEdit->setValidator(ipValidator);
}


client::~client()
{
    if(socket != -1)
#ifdef _WIN32
        closesocket(socket);
#elif __linux__
        close(socket);
#endif
    delete ui;
}

void client::on_connectButton_clicked()
{
    //Validate nick
    if(ui->nickEdit->text().length() == 0){
        ui->connectErrorLabel->setText("Set nick!");
        return;
    }

    //Validate port and host
    if(!ui->portNumber->hasAcceptableInput()
            && !ui->ipEdit->hasAcceptableInput()){
        ui->connectErrorLabel->setText("Invalid port!");
        return;
    }
    //just to be readible
    char host[17];
    strcpy(host,ui->ipEdit->text().toLocal8Bit().data());
    char port[6];
    strcpy(port,ui->portNumber->text().toLocal8Bit().data());

    printf("PORT %s\n", port);
    strcpy(nick,ui->nickEdit->text().toLocal8Bit().data());
    strcat(nick, "\0");

    if(-1 == (socket = get_tcp_socket(port, host, SOCKET_CONN))){
        ui->connectErrorLabel->setText("Wrong IP");
        return;
    }

    QSocketNotifier *SocketMonitor;
    SocketMonitor = new QSocketNotifier(socket, QSocketNotifier::Read, parent());
    connect(SocketMonitor,SIGNAL(activated(int)),this ,SLOT(dataReceived()) );

    char *set_nick = (char *)calloc(7+POORIRC_MSG_MAX_LEN, sizeof(char));
    strcat(set_nick, "/nick ");
    strcat(set_nick, nick);

    if(-1 == send(socket, (char *)set_nick, POORIRC_MSG_MAX_LEN, 0)){
        free(set_nick);
        return;
    }else {
        ui->connectErrorLabel->setText("");
        ui->sendButton->setEnabled(true);
        ui->sendMessage->setEnabled(true);
        ui->connectButton->setText("CONNECTED!");

        ui->nickEdit->setEnabled(false);
        ui->connectButton->setEnabled(false);
        ui->portNumber->setEnabled(false);
        ui->ipEdit->setEnabled(false);

    }
    free(set_nick);

    printf("Socket obtained!: %d\n", socket);

}

void client::on_sendButton_clicked()
{  
    char *message = ui->sendMessage->text().toLocal8Bit().data();
    strcat(message,"\0");

    send(socket, (char *)message, POORIRC_MSG_MAX_LEN, 0);

    ui->sendMessage->clear();
}

void client::dataReceived(){
   char *res = (char *)calloc(POORIRC_MSG_MAX_LEN + POORIRC_NICKNAME_MAX_LEN + 1, sizeof(char));
   recv(socket, res, POORIRC_MSG_MAX_LEN + POORIRC_NICKNAME_MAX_LEN + 1,0);
   ui->serverMessages->append(res);
   free(res);

}
