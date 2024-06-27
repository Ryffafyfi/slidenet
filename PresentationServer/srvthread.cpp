#include "srvthread.h"
#include "funcs.h"

/**
 * @brief Конструктор SrvThread.
 * 
 * Инициализирует объект SrvThread с заданным дескриптором сокета, списком файлов и именем файла записи.
 * 
 * @param ID Дескриптор сокета.
 * @param fileList Список файлов.
 * @param recordFileName Имя файла записи.
 * @param parent Указатель на родительский объект.
 */
SrvThread::SrvThread(qintptr ID, QStringList fileList, const QString& recordFileName, QObject *parent) : QThread(parent),
    socketDescriptor {ID}, fileList {fileList}, recordFileName { recordFileName }, slideNumber { 1 }
{

}

/**
 * @brief Запуск потока.
 * 
 * Инициализирует сокет и устанавливает соединения для обработки событий. Отправляет изображения клиенту и запускает цикл обработки событий.
 */
void SrvThread::run()
{
    qDebug() << " Thread started: " << this;

    socket = std::make_unique<QTcpSocket>();

    if(!socket.get()->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket.get(), &QTcpSocket::disconnected, this, &SrvThread::disconnected);
    connect(socket.get(), &QTcpSocket::readyRead, this, &SrvThread::receivingCommandFromClient, Qt::DirectConnection);

    SendImagesToClient();

    exec();
}

/**
 * @brief Обработчик отключения.
 * 
 * Вызывается при отключении сокета. Удаляет сокет и завершает поток.
 */
void SrvThread::disconnected()
{
    qDebug() << socketDescriptor << " Disconnected";

    socket->deleteLater();
    exit(0);
}

/**
 * @brief Изменение номера слайда.
 * 
 * Изменяет текущий номер слайда.
 * 
 * @param slideNumber Новый номер слайда.
 */
void SrvThread::slideNumberChange(quint16 slideNumber)
{
    this->slideNumber = slideNumber;
}

/**
 * @brief Прием команды от клиента.
 * 
 * Очищает данные, читает команду от клиента и вызывает выполнение команды.
 */
void SrvThread::receivingCommandFromClient()
{
    Data.clear();
    QDataStream in(socket.get());
    in.setVersion(QDataStream::Qt_6_6);
    if (valid_receiving(in.status(), socket->bytesAvailable()))
         in >> this->commandFromClient;
    executeClientCommand();
}

/**
 * @brief Выполнение команды клиента.
 * 
 * Выполняет команду, полученную от клиента.
 */
void SrvThread::executeClientCommand()
{
    qDebug() << "COMMAND: " << this->commandFromClient;

    switch (this->commandFromClient)
    {
        case 5001:
        {
            sendSlideNumberToClient();
            break;
        }
        case 5002:
        {
            sendRecordToClient();
            break;
        }
        default:
            break;
    }
}

/**
 * @brief Отправка номера слайда клиенту.
 * 
 * Отправляет текущий номер слайда клиенту через сокет.
 */
void SrvThread::sendSlideNumberToClient()
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << quint16(this->slideNumber);
    socket->write(Data);
}

/**
 * @brief Отправка записи клиенту.
 * 
 * Отправляет файл записи клиенту через сокет.
 */
void SrvThread::sendRecordToClient()
{
    file.setFileName(recordFileName + ".m4a");
    if(!file.open(QIODevice::ReadOnly)) {return; }

    qDebug() << "sendRecordToClient() 2" << file.size();

    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_6);
    out << file.size();
    out << file.readAll();
    socket->write(Data);
    Data.clear();
    file.close();
}

/**
 * @brief Отправка изображений клиенту.
 * 
 * Отправляет все изображения из списка файлов клиенту через сокет.
 */
void SrvThread::SendImagesToClient()
{
    while(!fileList.isEmpty())
    {
        file.setFileName(fileList.front());
        fileList.pop_front();
        if(!file.open(QIODevice::ReadOnly)) { continue; }

        Data.clear();
        QDataStream out(&Data, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_6_6);
        out << file.size();
        out << file.readAll();
        socket->write(Data);
        Data.clear();
        file.close();
    }
}
