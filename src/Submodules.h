#ifndef SUBMODULESDIALOG_H
#define SUBMODULESDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QMessageBox>
#include <QSettings>
#include <QFileInfo>
#include <QCloseEvent>
//#include <QComboBox>

#define db qDebug() << this <<
namespace Ui
{
class SubmodulesDialog;
}

namespace Submodules
{
enum ChangeFlags
{
    StreamLink = 0x01,
    Chatterino = 0x02
};

class Streamlink
{
public:
    Streamlink()
    {

    }
    ~Streamlink();
    QStringList getArgs(QString channel, unsigned long mpvContainer);

    void setQuality(int qual);
private:
    QString _quality;
};

class SubmodulesDialog : public QDialog
{
    Q_OBJECT
signals:
    void submodulesFinished();
    void refreshStream();
public:
    explicit SubmodulesDialog(QWidget *parent = nullptr);
    ~SubmodulesDialog();

    void showMessageBox();

    QString streamlinkPath();
    QString chatterinoPath();
    QString streamlinkArguments();
    QString streamlinkQuality();

    QStringList getStreamLinkArguments(QString channel, unsigned long mpvContainer);

    int getChanges();;
public slots:
    void showDialog();
    void initialize();
private slots:
    void saveSettings();

protected:
    virtual void closeEvent(QCloseEvent *e) override;
private:
    void loadSettings();
    void setupConnections();
    void hideAlerts();


    Ui::SubmodulesDialog *ui;
    Streamlink * _SL;
    QString _streamlinkPath;
    QString _streamlinkArgs;
    QString _chatterinoPath;

    QString _styleGreen = "color: rgb(44, 145, 7);";
    QString _styleRed = "color: rgb(156, 12, 21);";

    int _changes = 0;
};
}
#endif // SUBMODULESDIALOG_H
