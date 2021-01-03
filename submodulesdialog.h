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

class SubmodulesDialog : public QDialog
{
    Q_OBJECT
signals:
    void submodulesFinished();
public:
    explicit SubmodulesDialog(QWidget *parent = nullptr);
    ~SubmodulesDialog();

    void showMessageBox();

    QString streamlinkPath();
    QString chatterinoPath();
    QString streamlinkArguments();
    QString streamlinkQuality();
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


    Ui::SubmodulesDialog *ui;
    QString _streamlinkPath;
    QString _streamlinkArgs;
    QString _chatterinoPath;

    QString _styleGreen = "color: rgb(44, 145, 7);";
    QString _styleRed = "color: rgb(156, 12, 21);";
};

#endif // SUBMODULESDIALOG_H
