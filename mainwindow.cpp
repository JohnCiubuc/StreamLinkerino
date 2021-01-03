#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Run external tool checker
    _Submodules = new SubmodulesDialog;
    connect(_Submodules, &SubmodulesDialog::submodulesFinished, this, &MainWindow::initialize);

    _Submodules->initialize();
}

// Kill StreamLink and Chatterino on exit
MainWindow::~MainWindow()
{
    _pStreamlinkProcess->terminate();
    _pChatterinoProcess->terminate();
    delete ui;
}

// Run until Chatterino is launched
// Get winID from Chatterino Process
// Embed Chatterino into App
// Monitor /tmp for chat changes (if patched)
void MainWindow::setupChatterinoEmbed()
{
    QTimer * findChatterino = new QTimer;
    connect(findChatterino, &QTimer::timeout, this, [=]()
    {
        Window result = _WMP.getWID(_pChatterinoProcess->processId());
        if(result != 0)
        {
            QWindow *window = QWindow::fromWinId(result);
            _chatContainer = createWindowContainer(window);
            _chatContainer->setParent(ui->widget);
            _chatContainer->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
            _chatContainer->show();

            resizeEmbeds();

            _bChatterinoEmbedded = true;
            _tChatChannelMonitor->start(500);
            findChatterino->deleteLater();
        }
    });
    findChatterino->start(10);
}

// Requires patch to chatterino to monitor channel chages
// Would use QFileSystemWatcher but it is bugged for linux
void MainWindow::chatChannelMonitor()
{
    QFile channel("/tmp/chatterino_chan");
    if(channel.open(QFile::ReadOnly))
    {
        QList<QByteArray> sChans = channel.readAll().split(':');

        // Initiate and save Chatterino UUID unique to this client
        if(_chatterinoUUID.isEmpty())
            _chatterinoUUID = sChans[1];

        channel.close();
        // sChans
        //      @Arg1: New Channel
        //      @Arg2: Unique UUID for each program (if running multiple)
        if(sChans[0] != _cChatChannel && sChans[1] == _chatterinoUUID)
        {
            _pStreamlinkProcess->terminate();
            _mpvContainer->setParent(NULL);
            _cChatChannel = sChans[0];
            ui->statusbar->show();

            // Wait for current streamlink to die before restarting
            QTimer * restart = new QTimer(this);
            connect(restart, &QTimer::timeout, this, [=]()
            {
                if(_pStreamlinkProcess->state() == 0)
                {
                    _pStreamlinkProcess->setArguments(createStreamLinkArgs(_cChatChannel));
                    _pStreamlinkProcess->start();
                    restart->deleteLater();
                }
            });
            restart->start(10);
        }
    }
}

// Read streamlink output
// show on QPlainTextEdit debug
// Update Statusbar
// Load MPV when ready
void MainWindow::readStreamLink()
{
    QString s = _pStreamlinkProcess->readAll();
    ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText().append(s).append("\n"));
    ui->statusbar->showMessage(s);
    if(s.contains("player: mpv"))
    {
        _mpvContainer->setParent(ui->widget_2);
        _mpvContainer->show();
        ui->statusbar->hide();
        resizeEmbeds();
    }
}

void MainWindow::initialize()
{
    // Timer to check for chat channel switches
    _tChatChannelMonitor = new QTimer;
    connect(_tChatChannelMonitor, &QTimer::timeout, this, &MainWindow::chatChannelMonitor);

    // Setup StreamLink and Chatterino Processes
    _pChatterinoProcess = new QProcess(ui->widget);
    _pChatterinoProcess->setProgram(_Submodules->chatterinoPath());
    _pChatterinoProcess->start();

    _pStreamlinkProcess = new QProcess(ui->widget_2);
    _pStreamlinkProcess->setProgram(_Submodules->streamlinkPath());
    connect(_pStreamlinkProcess, &QProcess::readyRead, this, &MainWindow::readStreamLink);

    // Setup mpv container and wid
    QWindow * mpv_window = new QWindow;
    _mpvContainerWID = mpv_window->winId();
    _mpvContainer = createWindowContainer(mpv_window);
    _mpvContainer->setSizePolicy(QSizePolicy::Policy::Expanding,QSizePolicy::Policy::Expanding);
    _mpvContainer->hide();

    // Settings
    connect(ui->actionSettings, &QAction::triggered, _Submodules, &SubmodulesDialog::showDialog);

    // Run Chatterino and Embed it
    setupChatterinoEmbed();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(_bChatterinoEmbedded)
        resizeEmbeds();
    event->accept();
}

void MainWindow::resizeEmbeds()
{
    // Delay needed for X
    QTimer::singleShot(10, this, [=]()
    {
        _chatContainer->setGeometry(0,0,ui->widget->geometry().width(), ui->widget->geometry().height());
        _mpvContainer->setGeometry(0,0,ui->widget_2->geometry().width(), ui->widget_2->geometry().height());
    });
}

QStringList MainWindow::createStreamLinkArgs(QString channel)
{
    QStringList args;
    args << "--twitch-low-latency";
    args << "--twitch-disable-ads";
    args<< "--player";
    args << "mpv --wid="+QString::number(_mpvContainerWID);
    args << "https://www.twitch.tv/"+channel;
    args << "best";
    return args;
}

