#include "submodulesdialog.h"
#include "ui_submodulesdialog.h"

using namespace Submodules;

SubmodulesDialog::SubmodulesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubmodulesDialog)
{
    ui->setupUi(this);
    this->hide();
    _SL = new Streamlink;

    hideAlerts();

    setupConnections();
}

SubmodulesDialog::~SubmodulesDialog()
{
    delete ui;
}

void SubmodulesDialog::showMessageBox()
{
    QMessageBox alert;
    QString text = "Warning! The following external tools have not been found:\n\n";
    text += _streamlinkPath.isEmpty() ? "streamlink" : "chatterino";
    text += "\n\nPlease select the folder where the missing tools can be found.";
    alert.setText(text);
    alert.exec();

    showDialog();
}

QString SubmodulesDialog::streamlinkPath()
{
    return _streamlinkPath;
}

QString SubmodulesDialog::chatterinoPath()
{
    return _chatterinoPath;
}

QString SubmodulesDialog::streamlinkArguments()
{
    return _streamlinkArgs;
}

QString SubmodulesDialog::streamlinkQuality()
{
    return  ui->comboBox_StreamlinkQuality->currentText();
}

QStringList SubmodulesDialog::getStreamLinkArguments(QString channel, unsigned long mpvContainer)
{
    return _SL->getArgs(channel, mpvContainer);
}

void SubmodulesDialog::saveSettings()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope,"streamlinkerino", "streamlinkerino");
    settings.beginGroup("preferences");
    settings.setValue("streamlink", _streamlinkPath);
    settings.setValue("chatterino", _chatterinoPath);
    settings.setValue("streamlink_args", _streamlinkArgs);
    settings.setValue("streamlink_quality", ui->comboBox_StreamlinkQuality->currentIndex());
    settings.endGroup();

    _SL->setQuality(ui->comboBox_StreamlinkQuality->currentIndex());
}

void SubmodulesDialog::closeEvent(QCloseEvent *e)
{
    hideAlerts();
    emit submodulesFinished();
    e->accept();
}

void SubmodulesDialog::loadSettings()
{
    bool bMissing = false;
    QStringList paths = QStringList() << "/usr/bin/" << "/bin/" << "/usr/local/bin/";
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope,"streamlinkerino", "streamlinkerino");
    settings.beginGroup("preferences");

    ui->comboBox_StreamlinkQuality->setCurrentIndex(settings.value("streamlink_quality").toInt());
    _streamlinkPath = settings.value("streamlink", "").toString();
    _streamlinkArgs = settings.value("streamlink_args").toString();
    _chatterinoPath = settings.value("chatterino", "").toString();

    if(_streamlinkPath.isEmpty())
    {
        bMissing=true;
        foreach(QString path, paths)
        {
            QFileInfo check_file(path + "streamlink");
            if (check_file.exists() && check_file.isFile())
            {
                _streamlinkPath = path + "streamlink";
                break;
            }
        }
    }
    if(_chatterinoPath.isEmpty())
    {
        bMissing=true;
        foreach(QString path, paths)
        {
            QFileInfo check_file(path + "chatterino");
            if (check_file.exists() && check_file.isFile())
            {
                _streamlinkPath = path + "chatterino";
                break;
            }
        }
    }
    if(bMissing)
    {
        settings.setValue("streamlink", _streamlinkPath);
        settings.setValue("chatterino", _chatterinoPath);
    }
    settings.endGroup();

    _SL->setQuality(ui->comboBox_StreamlinkQuality->currentIndex());
    if (_streamlinkPath.isEmpty() || _chatterinoPath.isEmpty())
        showMessageBox();
    else
        emit submodulesFinished();
}

void SubmodulesDialog::showDialog()
{
    ui->lineEdit_chatterinoPath->setText(_chatterinoPath);
    ui->lineEdit_streamLinkPath->setText(_streamlinkPath);
    ui->lineEdit_streamLinkOptions->setText(_streamlinkArgs);
    _changes = 0;
    this->show();
}

void SubmodulesDialog::initialize()
{
    loadSettings();
}

void SubmodulesDialog::setupConnections()
{
    connect(ui->lineEdit_chatterinoPath, &QLineEdit::editingFinished, this, [=]()
    {
        QString loc =  ui->lineEdit_chatterinoPath->text().simplified();
        QStringList paths =QStringList() << loc
                           << loc + "/chatterino";
        foreach(QString path, paths)
        {
            if(!path.contains("chatterino")) continue;
            QFileInfo check_file(path);
            if (check_file.exists() && check_file.isFile())
            {
                ui->label_chatterinoAlert->setText("Chatterino Found");
                ui->label_chatterinoAlert->setStyleSheet(_styleGreen);
                ui->label_chatterinoAlert->show();
                _chatterinoPath = path;
                // If there's a difference
                if(!_chatterinoPath.isEmpty() && _chatterinoPath != loc)
                {
                    if(!(_changes & ChangeFlags::Chatterino))
                        _changes ^= ChangeFlags::Chatterino;
                }
                saveSettings();
                return;
            }
        }

//        db_f(not found);
        ui->label_chatterinoAlert->setText("Chatterino Not Found");
        ui->label_chatterinoAlert->setStyleSheet(_styleRed);
        ui->label_chatterinoAlert->show();
    });
    connect(ui->lineEdit_streamLinkPath, &QLineEdit::editingFinished, this, [=]()
    {
        QString loc =  ui->lineEdit_streamLinkPath->text().simplified();
        QStringList paths =QStringList() << loc
                           << loc + "/streamlink";
        foreach(QString path, paths)
        {
            if(!path.contains("streamlink")) continue;
            QFileInfo check_file(path);
            if (check_file.exists() && check_file.isFile())
            {
                ui->label_streamlinkAlert->setText("Streamlink Found");
                ui->label_streamlinkAlert->setStyleSheet(_styleGreen);
                ui->label_streamlinkAlert->show();
                _streamlinkPath = path;
                // If there's no difference
                if(!_streamlinkPath.isEmpty() && _streamlinkPath != loc)
                {
                    if(!(_changes & ChangeFlags::StreamLink))
                        _changes ^= ChangeFlags::StreamLink;
                }
                saveSettings();
                return;
            }
        }

        ui->label_streamlinkAlert->setText("Streamlink Not Found");
        ui->label_streamlinkAlert->setStyleSheet(_styleRed);
        ui->label_streamlinkAlert->show();
    });


    connect(ui->lineEdit_streamLinkOptions, &QLineEdit::editingFinished, this, &SubmodulesDialog::saveSettings);
    // Man, this connect is ugly af
    connect(ui->comboBox_StreamlinkQuality, static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged), this, [=]()
    {
        // Queue up streamlink reset
        if(!(_changes & ChangeFlags::StreamLink))
            _changes ^= ChangeFlags::StreamLink;

        saveSettings();
    });
}

void SubmodulesDialog::hideAlerts()
{
    ui->label_chatterinoAlert->hide();
    ui->label_streamlinkAlert->hide();
}

int Submodules::SubmodulesDialog::getChanges()
{
    return _changes;
}

QStringList Streamlink::getArgs(QString channel, unsigned long mpvContainer)
{
    QStringList args;
    args << "--twitch-low-latency";
    args << "--twitch-disable-ads";
    args<< "--player";
    args << "mpv --wid="+QString::number(mpvContainer);
    args << "https://www.twitch.tv/"+channel;
    args << _quality;
    return args;
}

void Streamlink::setQuality(int qual)
{
    if(qual >8)
    {
        db "setQuality OOR";
        return;
    }
    const char * qualityList[] = {"best", "1080p60", "720p60", "720p", "480p", "360p", "160p", "worst", "audio_only"};

    _quality = QString(qualityList[qual]);
}
