#pragma once

#include "server.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>
#include <QAudioInput>
#include <QAudioDevice>
#include <QMediaCaptureSession>
#include <QMediaRecorder>
#include <QMediaFormat>
#include <QMediaDevices>
#include <QMimeType>
#include <QUrl>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startServerButton_clicked();

    void on_stopServerButton_clicked();

    void on_uploadButton_clicked();

    void on_leftSlideButton_clicked();

    void on_rightSlideButton_clicked();

    void on_voiceRecordingButton_clicked();

    void on_stopVoiceRecordingButton_clicked();

private:
    void showSlide(quint16 slideNumber);

private:
    QFileDialog fdialog;
    std::vector<QPixmap> pixmapVector;
    std::unique_ptr<Server> tcpServer;
    QStringList fileList;
    std::unique_ptr<QMediaRecorder> audioRecorder;
    QMediaFormat format;
    QMediaCaptureSession captureSession;
    QString recordFileName;

    Ui::MainWindow *ui;
};
