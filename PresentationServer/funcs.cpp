#include "funcs.h"



/**
 * @brief Проверяет валидность полученных данных.
 *
 * Функция проверяет, находится ли поток данных в нормальном состоянии и
 * доступны ли данные необходимого размера.
 *
 * @param indata Текущий статус потока данных.
 * @param available Количество байт, доступных для чтения.
 * @return true если поток данных в нормальном состоянии и доступно ровно sizeof(quint16) байт, иначе false.
 */
bool valid_receiving(QDataStream::Status indata, qint64 available)
{
    if(indata == QDataStream::Ok)
    {
        if (available == (qint64)sizeof(quint16))
            return true;
        else
            return false;
    }
    else
        return false;
}




/**
 * @brief Проверяет и изменяет номер слайда.
 *
 * Функция изменяет номер слайда в зависимости от заданного режима.
 * Если режим "incr", номер слайда увеличивается, если текущий номер меньше указанного предела.
 * Если режим "decr", номер слайда уменьшается, если текущий номер больше указанного предела.
 *
 * @param slide_num Указатель на текущий номер слайда.
 * @param slide_num_inf Предел для изменения номера слайда.
 * @param mod Режим изменения: "incr" для увеличения, "decr" для уменьшения.
 * @return true если номер слайда изменен, иначе false.
 */
bool chack_and_change_slide_num(quint16 * slide_num, quint16 slide_num_inf, QString mod)
{
    if (mod == QString("incr"))
    {
        if(*slide_num < slide_num_inf)
        {
            ++(*slide_num);
            return true;
        }
        else
            return false;
    }
    else if (mod == QString("decr"))
    {
        if (*slide_num > slide_num_inf)
        {
            --(*slide_num);
            return true;
        }
        else
            return false;
    }
    else
        return false;
}




/**
 * @brief Устанавливает минимальный номер.
 *
 * Функция устанавливает минимальный номер, если он больше нуля.
 *
 * @param real_num_min Указатель на переменную, хранящую реальный минимальный номер.
 * @param num_min Значение, которое нужно установить в качестве минимального номера.
 */
void set_num_min(quint16 * real_num_min, qint64 num_min)
{
    if(num_min > 0)
        *real_num_min = num_min;
}

bool checking_filelist(QStringList filelist)
{
    return filelist.isEmpty();
}


bool checking_path_exist(const auto v)
{
    return QFile::exists(v);
}


bool checking_valid_and_correct_status(QString recordFileName, QMediaRecorder::RecorderState stat)
{
    return (QUrl::fromLocalFile(recordFileName).isValid() && stat == QMediaRecorder::StoppedState);
}

