#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QMessageBox>
#include <QPushButton>
#include <QEventLoop>
#include <QTimer>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class PresentationClient;
}

QT_END_NAMESPACE

class PresentationClient : public QMainWindow
{
    Q_OBJECT

public:
    PresentationClient(QWidget *parent = nullptr);
    ~PresentationClient();

private slots:
    void on_connectionButton_clicked();
    void on_showAPresentationButton_clicked();

    void on_downloadVoiceRecordButton_clicked();

signals:
    void slideNumberHasBeenRead();

private:
    Ui::PresentationClient *ui;
    std::unique_ptr<QTcpSocket> socket;
    qint64 m_nNextBlockSize;
    QByteArray Data;
    std::vector<QPixmap> pixmapVector;
    bool isShow;
    quint16 slideNumber;

    enum class commandsForServer
    {
        numberOfSlide = 5001,
        downloadVoiceRecord = 5002
    };

    QEventLoop waitingForSlideNumber;
    QEventLoop slideCheckTimerTimeout;
    QEventLoop writeFileFinishidEvent;
    QTimer timer;
    QString recordFileName;
    QFile file;

private:
    void requestingASlideNumber();

public slots:
    void slidesReadFromServer();
    void slideNumberFromServer();
    void downloadRecordFromServer();
};
