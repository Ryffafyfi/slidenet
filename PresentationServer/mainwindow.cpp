#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)  : QMainWindow(parent), ui(new Ui::MainWindow), fdialog { this },
    pixmapVector { 0 }
{
    ui->setupUi(this);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setAlignment(Qt::AlignCenter);
    ui->recordInfolabel->setAlignment(Qt::AlignCenter);

    audioRecorder = std::make_unique<QMediaRecorder>(this);
    captureSession.setRecorder(audioRecorder.get());
    captureSession.setAudioInput(new QAudioInput(this));
    captureSession.audioInput()->setDevice(QMediaDevices::defaultAudioInput());

    format.setFileFormat(QMediaFormat::FileFormat::MP3);
    format.setAudioCodec(QMediaFormat::AudioCodec::MP3);
    audioRecorder->setMediaFormat(format);
    audioRecorder->setAudioSampleRate(44100);
    audioRecorder->setAudioBitRate(32000);
    audioRecorder->setAudioChannelCount(QMediaDevices::defaultAudioInput().maximumChannelCount());
    audioRecorder->setQuality(QMediaRecorder::Quality::NormalQuality);
    audioRecorder->setEncodingMode(QMediaRecorder::EncodingMode::ConstantQualityEncoding);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startServerButton_clicked()
{
    if(fileList.isEmpty())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "No image files selected");
    }
    else
    {
        tcpServer = std::make_unique<Server>(fileList, recordFileName);
        tcpServer->setslideNumberMinimum(1);
        tcpServer->setslideNumberMaximum(fileList.size());
    }
}


void MainWindow::on_stopServerButton_clicked()
{

}

void MainWindow::on_uploadButton_clicked()
{
    fileList = std::move(fdialog.getOpenFileNames(this, tr("Open Image"),
                                                  "/home/jana", tr("Image Files (*.png *.jpg *.jpeg *.bmp)")));

    if(!fileList.isEmpty())
    {

       for(const auto &v : fileList)
       {
            if(!QFile::exists(v))
            {
                QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "Error open file: "
                                                                                              + fileList.back());
            }
            else
            {
                pixmapVector.emplace_back(QPixmap(v));
                ui->imageLabel->setPixmap(pixmapVector[0]);
            }
       }
    }
}


void MainWindow::on_leftSlideButton_clicked()
{
    if(tcpServer.get() != nullptr)
    {
        tcpServer->slideNumberDecrease();
        showSlide(tcpServer->getSlideNumber());
    }
}


void MainWindow::on_rightSlideButton_clicked()
{
    if(tcpServer.get() != nullptr)
    {
        tcpServer->slideNumberIncrease();
        showSlide(tcpServer->getSlideNumber());
    }
}

void MainWindow::showSlide(quint16 slideNumber)
{
    if (pixmapVector[slideNumber - 1].isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "No image files selected");
        return;
    }
    else
    {
        ui->imageLabel->setPixmap(pixmapVector[slideNumber - 1]);
    }
}


void MainWindow::on_voiceRecordingButton_clicked()
{
    recordFileName = QFileDialog::getSaveFileName();

    if(QUrl::fromLocalFile(recordFileName).isValid() && audioRecorder->recorderState() == QMediaRecorder::StoppedState)
    {
        ui->recordInfolabel->setText("Audio recording is in progress");
        audioRecorder->setOutputLocation(QUrl::fromLocalFile(recordFileName));
        audioRecorder->record();
    }
    else
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "The file path is not set correctly");
    }
}

void MainWindow::on_stopVoiceRecordingButton_clicked()
{
    ui->recordInfolabel->setText("Audio recording has been stopped");
    audioRecorder->stop();
}


