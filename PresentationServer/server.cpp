#include "server.h"
#include "funcs.h"

/**
 * @brief Конструктор Server.
 * 
 * Инициализирует сервер с заданным списком файлов и именем файла записи.
 * Запускает сервер на порту 2323 и выводит сообщение об успешном запуске или ошибке.
 * 
 * @param fileList Список файлов.
 * @param recordFileName Имя файла записи.
 */
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

/**
 * @brief Увеличивает номер слайда.
 * 
 * Если номер слайда меньше максимального, увеличивает номер слайда на единицу и испускает сигнал slideNumberChanged.
 */
void Server::slideNumberIncrease()
{
    if (chack_and_change_slide_num(&(this->slideNumber), this->slideNumberMaximum, QString("incr")))
        emit slideNumberChanged(this->slideNumber);
}

/**
 * @brief Уменьшает номер слайда.
 * 
 * Если номер слайда больше минимального, уменьшает номер слайда на единицу и испускает сигнал slideNumberChanged.
 */
void Server::slideNumberDecrease()
{
    if (chack_and_change_slide_num(&(this->slideNumber), this->slideNumberMinimum, QString("decr")))
        emit slideNumberChanged(this->slideNumber);
}

/**
 * @brief Устанавливает минимальный номер слайда.
 * 
 * Устанавливает минимальный номер слайда, если переданное значение больше нуля.
 * 
 * @param slideNumberMinimum Минимальный номер слайда.
 */
void Server::setslideNumberMinimum(qint64 slideNumberMinimum)
{
    set_num_min(&(this->slideNumberMinimum), slideNumberMinimum);
}

/**
 * @brief Устанавливает максимальный номер слайда.
 * 
 * Устанавливает максимальный номер слайда.
 * 
 * @param slideNumberMaximum Максимальный номер слайда.
 */
void Server::setslideNumberMaximum(quint16 slideNumberMaximum)
{
    this->slideNumberMaximum = slideNumberMaximum;
}

/**
 * @brief Возвращает текущий номер слайда.
 * 
 * @return Текущий номер слайда.
 */
const quint16 Server::getSlideNumber()
{
    return slideNumber;
}

/**
 * @brief Обработчик входящих подключений.
 * 
 * Принимает входящее подключение, создает новый поток SrvThread для обработки подключения.
 * Устанавливает соединения для обработки завершения потока и изменения номера слайда.
 * 
 * @param socketDescriptor Дескриптор сокета.
 */
void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << " socketDescriptor: " <<  socketDescriptor;
    threads.emplace_back(std::make_unique<SrvThread>(socketDescriptor, fileList, recordFileName, this));
    connect(threads.back().get(), &QThread::finished, threads.back().get(), &QThread::deleteLater);
    connect(this, &Server::slideNumberChanged, threads.back().get(), &::SrvThread::slideNumberChange);
    threads.back().get()->start();
}
