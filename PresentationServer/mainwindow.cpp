#include "mainwindow.h"
#include "./ui_mainwindow.h"

/**
 * @brief Конструктор класса MainWindow.
 * 
 * Инициализирует элементы интерфейса и настраивает параметры для записи аудио.
 * 
 * @param parent Родительский виджет, если есть.
 */
MainWindow::MainWindow(QWidget *parent)  
    : QMainWindow(parent), ui(new Ui::MainWindow), fdialog { this },
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

/**
 * @brief Деструктор класса MainWindow.
 * 
 * Освобождает память, выделенную для интерфейса.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Слот для обработки нажатия кнопки старта сервера.
 * 
 * Если список файлов пуст, отображает сообщение. Иначе, создает и настраивает объект сервера.
 */
void MainWindow::on_startServerButton_clicked()
{
    if(checking_filelist(fileList))
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
/**
 * @brief Слот для обработки нажатия кнопки загрузки изображений.
 * 
 * Открывает диалог выбора файлов изображений и загружает выбранные файлы.
 */
void MainWindow::on_uploadButton_clicked()
{
    fileList = std::move(fdialog.getOpenFileNames(this, tr("Open Image"),
                                                  "/home/jana", tr("Image Files (*.png *.jpg *.jpeg *.bmp)")));

    if(!checking_filelist(fileList))
    {

       for(const auto &v : fileList)
       {
            if(checking_path_exist(v))
            {
               pixmapVector.emplace_back(QPixmap(v));
               ui->imageLabel->setPixmap(pixmapVector[0]);
            }
            else
            {
                QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "Error open file: "
                                                                                              + fileList.back());
            }
       }
    }
}

/**
 * @brief Слот для обработки нажатия кнопки переключения на предыдущий слайд.
 * 
 * Уменьшает номер текущего слайда на сервере и отображает его.
 */
void MainWindow::on_leftSlideButton_clicked()
{
    if(tcpServer.get() != nullptr)
    {
        tcpServer->slideNumberDecrease();
        showSlide(tcpServer->getSlideNumber());
    }
}

/**
 * @brief Слот для обработки нажатия кнопки переключения на следующий слайд.
 * 
 * Увеличивает номер текущего слайда на сервере и отображает его.
 */
void MainWindow::on_rightSlideButton_clicked()
{
    if(tcpServer.get() != nullptr)
    {
        tcpServer->slideNumberIncrease();
        showSlide(tcpServer->getSlideNumber());
    }
}

/**
 * @brief Отображает слайд по заданному номеру.
 * 
 * @param slideNumber Номер слайда для отображения.
 */
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

/**
 * @brief Слот для обработки нажатия кнопки начала записи голоса.
 * 
 * Открывает диалог выбора места сохранения файла записи и начинает запись.
 */
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

/**
 * @brief Слот для обработки нажатия кнопки остановки записи голоса.
 * 
 * Останавливает запись и обновляет текст метки о состоянии записи.
 */
void MainWindow::on_stopVoiceRecordingButton_clicked()
{
    ui->recordInfolabel->setText("Audio recording has been stopped");
    audioRecorder->stop();
}
