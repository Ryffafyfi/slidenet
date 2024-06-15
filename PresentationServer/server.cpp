#include "server.h"

Server::Server(const QStringList &fileList, const QString& recordFileName) :
    fileList { fileList }, recordFileName { recordFileName }, slideNumber {1}, slideNumberMinimum {0}, slideNumberMaximum {0}
{
    if(this->listen(QHostAddress::Any, 2323))
    {
        qDebug() << "start\n";
    }
    else
    {
        qDebug() << "error\n";
    }
}

void Server::slideNumberIncrease()
{
    if(this->slideNumber < this->slideNumberMaximum)
    {
        ++slideNumber;
        emit slideNumberChanged(this->slideNumber);
    }
}

void Server::slideNumberDecrease()
{
    if(this->slideNumber > this->slideNumberMinimum)
    {
        --slideNumber;
        emit slideNumberChanged(this->slideNumber);
    }
}

void Server::setslideNumberMinimum(quint16 slideNumberMinimum)
{
    if(slideNumberMinimum > 0)
    {
        this->slideNumberMinimum = slideNumberMinimum;
    }
}

void Server::setslideNumberMaximum(quint16 slideNumberMaximum)
{
    this->slideNumberMaximum = slideNumberMaximum;
}

const quint16 Server::getSlideNumber()
{
    return slideNumber;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << " socketDescriptor: " <<  socketDescriptor;
    threads.emplace_back(std::make_unique<SrvThread>(socketDescriptor, fileList, recordFileName, this));
    connect(threads.back().get(), &QThread::finished, threads.back().get(), &QThread::deleteLater);
    connect(this, &Server::slideNumberChanged, threads.back().get(), &::SrvThread::slideNumberChange);
    threads.back().get()->start();
}






//****************************************TRANSLATE VOICE****************************************************************

