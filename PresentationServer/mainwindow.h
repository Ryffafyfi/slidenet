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

/**
 * @brief Класс MainWindow предоставляет главное окно приложения.
 * 
 * Этот класс наследуется от QMainWindow и предоставляет функциональность, такую как запуск/остановка сервера, 
 * загрузка изображений, навигация по слайдам и запись аудио.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор MainWindow.
     * 
     * Инициализирует компоненты пользовательского интерфейса и настраивает параметры аудиозаписи.
     * 
     * @param parent Родительский виджет, если есть.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Деструктор MainWindow.
     * 
     * Очищает выделенные ресурсы.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Слот для обработки события нажатия кнопки запуска сервера.
     * 
     * Запускает сервер, если файлы изображений были загружены.
     */
    void on_startServerButton_clicked();

    /**
     * @brief Слот для обработки события нажатия кнопки остановки сервера.
     * 
     * Останавливает сервер.
     */
    void on_stopServerButton_clicked();

    /**
     * @brief Слот для обработки события нажатия кнопки загрузки.
     * 
     * Открывает диалоговое окно для загрузки файлов изображений.
     */
    void on_uploadButton_clicked();

    /**
     * @brief Слот для обработки события нажатия кнопки переключения на предыдущий слайд.
     * 
     * Переходит к предыдущему слайду.
     */
    void on_leftSlideButton_clicked();

    /**
     * @brief Слот для обработки события нажатия кнопки переключения на следующий слайд.
     * 
     * Переходит к следующему слайду.
     */
    void on_rightSlideButton_clicked();

    /**
     * @brief Слот для обработки события нажатия кнопки начала записи голоса.
     * 
     * Начинает запись аудио.
     */
    void on_voiceRecordingButton_clicked();

    /**
     * @brief Слот для обработки события нажатия кнопки остановки записи голоса.
     * 
     * Останавливает запись аудио.
     */
    void on_stopVoiceRecordingButton_clicked();

private:
    /**
     * @brief Отображает слайд, соответствующий указанному номеру слайда.
     * 
     * @param slideNumber Номер слайда для отображения.
     */
    void showSlide(quint16 slideNumber);

private:
    QFileDialog fdialog;                         ///< Диалоговое окно для выбора файлов изображений.
    std::vector<QPixmap> pixmapVector;           ///< Вектор для хранения загруженных изображений.
    std::unique_ptr<Server> tcpServer;           ///< Указатель на экземпляр сервера.
    QStringList fileList;                        ///< Список путей к файлам изображений.
    std::unique_ptr<QMediaRecorder> audioRecorder; ///< Указатель на экземпляр аудиорекордера.
    QMediaFormat format;                         ///< Формат медиа для аудиозаписи.
    QMediaCaptureSession captureSession;         ///< Сессия захвата для медиа записи.
    QString recordFileName;                      ///< Имя файла для аудиозаписи.

    Ui::MainWindow *ui;                          ///< Указатель на компоненты пользовательского интерфейса.
};
