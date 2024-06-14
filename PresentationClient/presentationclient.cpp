#include "presentationclient.h"
#include "./ui_presentationclient.h"

PresentationClient::PresentationClient(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::PresentationClient), socket{ std::make_unique<QTcpSocket>() },
    m_nNextBlockSize { 0 }, Data {0, Qt::Uninitialized}, pixmapVector{0}, isShow {true}, slideNumber {0}
{
    ui->setupUi(this);
    connect(socket.get(), &QTcpSocket::readyRead, this, &PresentationClient::slidesReadFromServer);
    connect(socket.get(), &QTcpSocket::disconnected, socket.get(), &QTcpSocket::deleteLater);
    connect(this, &PresentationClient::slideNumberHasBeenRead, &waitingForSlideNumber, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &slideCheckTimerTimeout, &QEventLoop::quit);
    //connect(&file, &QFile::readChannelFinished, &writeFileFinishidEvent, &QEventLoop::quit);
    ui->imageLabel->setScaledContents(true);
    ui->imageLabel->setAlignment(Qt::AlignCenter);
}

PresentationClient::~PresentationClient()
{
    delete ui;
}

void PresentationClient::on_connectionButton_clicked()
{
    socket->connectToHost("127.0.0.1", 2323);
    if(socket->isOpen())
    {

    }
    else
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "Connecting error");
    }
}

void PresentationClient::requestingASlideNumber()
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << quint16(commandsForServer::numberOfSlide);
    //out << str; //Записываем блок;
    //out.device()->seek(0); //Когда блок записан, переходим в начало и мерием длину блока;
    //out << quint16(Data.size() - sizeof(quint16));
    socket->write(Data);
}

void PresentationClient::slidesReadFromServer()
{
    Data.clear();
    QDataStream in(socket.get());
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok)
    {
        while(true)
        {
            if (m_nNextBlockSize == 0)
            {
                if (socket->bytesAvailable() < (qint64)sizeof(qint64))
                {
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (socket->bytesAvailable() < m_nNextBlockSize)
            {
                break;
            }

            in >> Data;
            pixmapVector.emplace_back(QPixmap());
            pixmapVector.back().loadFromData(Data);

            m_nNextBlockSize = 0;
        }      
        ui->downloadVoiceRecordButton->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "Slides read error");
    }
}

void PresentationClient::slideNumberFromServer()
{
    Data.clear();
    QDataStream in(socket.get());
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok)
    {
        if (socket->bytesAvailable() == (qint64)sizeof(quint16))
        {
            in >> slideNumber;
            emit slideNumberHasBeenRead();
        }
    }
    else
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 "Read current number slide error");
    }
}

void PresentationClient::downloadRecordFromServer()
{

    qDebug() << "TEST 0";

    Data.clear();
    QDataStream in(socket.get());
    in.setVersion(QDataStream::Qt_6_6);
    if(in.status() == QDataStream::Ok)
    {
        while(true)
        {
            if (m_nNextBlockSize == 0)
            {
                if (socket->bytesAvailable() < (qint64)sizeof(qint64))
                {
                    file.close();
                    break;
                }
                in >> m_nNextBlockSize;
            }

            if (socket->bytesAvailable() < m_nNextBlockSize)
            {
                file.close();
                break;
            }

            in >> Data;

            file.write(Data);
            //writeFileFinishidEvent.exec();
            file.close();

            m_nNextBlockSize = 0;
        }

        disconnect(socket.get(), &QTcpSocket::readyRead, this, &PresentationClient::downloadRecordFromServer);
        connect(socket.get(), &QTcpSocket::readyRead, this, &PresentationClient::slideNumberFromServer);

        ui->showAPresentationButton->setEnabled(true);
    }
    else
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "Voice download error"); 
    } 
}

void PresentationClient::on_showAPresentationButton_clicked()
{
    while(isShow && pixmapVector.size() != 0)
    {
        timer.start(std::chrono::milliseconds(1500));
        slideCheckTimerTimeout.exec();
        requestingASlideNumber();
        waitingForSlideNumber.exec();
        ui->imageLabel->setPixmap(pixmapVector[slideNumber - 1]);
    }
}


void PresentationClient::on_downloadVoiceRecordButton_clicked()
{    
    recordFileName = QFileDialog::getSaveFileName();
    file.setFileName(recordFileName + ".m4a");

    if(file.open(QIODevice::WriteOnly))
    {
        disconnect(socket.get(), &QTcpSocket::readyRead, this, &PresentationClient::slidesReadFromServer);
        connect(socket.get(), &QTcpSocket::readyRead, this, &PresentationClient::downloadRecordFromServer);

        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_6);
        out << quint16(commandsForServer::downloadVoiceRecord);
        socket->write(Data);
    }
    else
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), "The file path is not set correctly");
    }


}

