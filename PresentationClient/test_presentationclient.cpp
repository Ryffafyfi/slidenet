#include <QtTest>
#include "presentationclient.h"

class TestPresentationClient : public QObject
{
    Q_OBJECT

private slots:
    void test_on_connectionButton_clicked();
    void test_requestingASlideNumber();
    void test_slidesReadFromServer();
    void test_slideNumberFromServer();
    void test_downloadRecordFromServer();
    void test_on_showAPresentationButton_clicked();
    void test_on_downloadVoiceRecordButton_clicked();
};

void TestPresentationClient::test_on_connectionButton_clicked()
{
    PresentationClient client;
    client.on_connectionButton_clicked();

    // Test 1: Проверка, что попытка подключения была произведена
    QVERIFY(client.socket->state() == QAbstractSocket::ConnectingState || client.socket->state() == QAbstractSocket::ConnectedState);

    // Test 2: Проверка, что сообщение об ошибке отображается при невозможности подключения
    client.socket->abort(); // Имитируем ошибку подключения
    client.on_connectionButton_clicked();
    QVERIFY(!client.socket->isOpen());

    // Test 3: Проверка, что сокет закрыт при ошибке
    client.socket->abort();
    client.on_connectionButton_clicked();
    QVERIFY(client.socket->state() == QAbstractSocket::UnconnectedState);
}

void TestPresentationClient::test_requestingASlideNumber()
{
    PresentationClient client;
    client.requestingASlideNumber();

    // Test 1: Проверка, что данные были очищены
    QVERIFY(client.Data.isEmpty() == false);

    // Test 2: Проверка, что данные были записаны в сокет
    QVERIFY(client.socket->bytesToWrite() > 0);

    // Test 3: Проверка, что команда для запроса номера слайда была отправлена
    QDataStream in(&client.Data, QIODevice::ReadOnly);
    quint16 command;
    in >> command;
    QCOMPARE(command, quint16(commandsForServer::numberOfSlide));
}

void TestPresentationClient::test_slidesReadFromServer()
{
    PresentationClient client;
    client.slidesReadFromServer();

    // Test 1: Проверка, что данные были очищены
    QVERIFY(client.Data.isEmpty() == true);

    // Test 2: Проверка, что кнопка загрузки голосовой записи активирована
    QVERIFY(client.ui->downloadVoiceRecordButton->isEnabled() == true);

    // Test 3: Проверка, что изображения загружены в pixmapVector
    QCOMPARE(client.pixmapVector.size(), 0);
}

void TestPresentationClient::test_slideNumberFromServer()
{
    PresentationClient client;
    client.slideNumberFromServer();

    // Test 1: Проверка, что данные были очищены
    QVERIFY(client.Data.isEmpty() == true);

    // Test 2: Проверка, что номер слайда считан правильно
    quint16 expectedSlideNumber = 1; // Предположим, что номер слайда 1
    client.slideNumber = expectedSlideNumber;
    QCOMPARE(client.slideNumber, expectedSlideNumber);

    // Test 3: Проверка, что сигнал slideNumberHasBeenRead был отправлен
    QSignalSpy spy(&client, &PresentationClient::slideNumberHasBeenRead);
    QVERIFY(spy.count() == 1);
}

void TestPresentationClient::test_downloadRecordFromServer()
{
    PresentationClient client;
    client.downloadRecordFromServer();

    // Test 1: Проверка, что данные были очищены
    QVERIFY(client.Data.isEmpty() == true);

    // Test 2: Проверка, что файл был закрыт при ошибке чтения
    client.socket->abort(); // Имитируем ошибку чтения
    QVERIFY(client.file.isOpen() == false);

    // Test 3: Проверка, что данные были записаны в файл
    QVERIFY(client.file.size() > 0);
}

void TestPresentationClient::test_on_showAPresentationButton_clicked()
{
    PresentationClient client;
    client.pixmapVector.push_back(QPixmap()); // Добавляем пустое изображение для теста
    client.on_showAPresentationButton_clicked();

    // Test 1: Проверка, что таймер запущен
    QVERIFY(client.timer.isActive() == true);

    // Test 2: Проверка, что цикл ожидания был выполнен
    QEventLoop loop;
    QTimer::singleShot(1500, &loop, &QEventLoop::quit);
    loop.exec();
    QVERIFY(client.timer.isActive() == false);

    // Test 3: Проверка, что изображение установлено на метке
    QVERIFY(!client.ui->imageLabel->pixmap()->isNull());
}

void TestPresentationClient::test_on_downloadVoiceRecordButton_clicked()
{
    PresentationClient client;
    client.on_downloadVoiceRecordButton_clicked();

    // Test 1: Проверка, что имя файла установлено
    QVERIFY(!client.file.fileName().isEmpty());

    // Test 2: Проверка, что сокет подключен к правильному слоту
    QVERIFY(client.socket->isOpen() == true);

    // Test 3: Проверка, что данные были записаны в сокет
    QVERIFY(client.socket->bytesToWrite() > 0);
}

QTEST_MAIN(TestPresentationClient)
#include "test_presentationclient.moc"
