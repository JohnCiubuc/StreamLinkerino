#include "submodulesdialog.h"
#include "ui_submodulesdialog.h"

SubmodulesDialog::SubmodulesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SubmodulesDialog)
{
    ui->setupUi(this);
    this->hide();
    ui->label_chatterinoAlert->hide();
    ui->label_streamlinkAlert->hide();

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

void SubmodulesDialog::saveSettings()
{
    QSettings settings(QSettings::NativeFormat, QSettings::UserScope,"streamlinkerino", "streamlinkerino");
    settings.beginGroup("preferences");
    settings.setValue("streamlink", _streamlinkPath);
    settings.setValue("chatterino", _chatterinoPath);
    settings.setValue("streamlink_args", _streamlinkArgs);
    settings.setValue("streamlink_quality", ui->comboBox_StreamlinkQuality->currentIndex());
    settings.endGroup();
}

void SubmodulesDialog::closeEvent(QCloseEvent *e)
{
    emit submodulesFinished();
//    e->ignore();
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
            qDebug() << this << path;
            QFileInfo check_file(path);
            if (check_file.exists() && check_file.isFile())
            {
                ui->label_chatterinoAlert->setText("Chatterino Found");
                ui->label_chatterinoAlert->setStyleSheet(_styleGreen);
                ui->label_chatterinoAlert->show();
                _chatterinoPath = path;
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
            db path;
            QFileInfo check_file(path);
            if (check_file.exists() && check_file.isFile())
            {
                ui->label_streamlinkAlert->setText("Streamlink Found");
                ui->label_streamlinkAlert->setStyleSheet(_styleGreen);
                ui->label_streamlinkAlert->show();
                _chatterinoPath = path;
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
    connect(ui->comboBox_StreamlinkQuality, static_cast<void (QComboBox::*)(const int)>(&QComboBox::currentIndexChanged), this, &SubmodulesDialog::saveSettings);
}
