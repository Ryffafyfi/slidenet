#ifndef FUNCS_H
#define FUNCS_H

#include "qtypes.h"
#include "qdatastream.h"
#include "qstring.h"
#include "qcontainerfwd.h"
#include "qurl.h"
#include <QMediaRecorder>
#include "qfile.h"

bool valid_receiving(QDataStream::Status indata, qint64 available);
bool chack_and_change_slide_num(quint16 * slide_num, quint16 slide_num_inf, QString mod);
void set_num_min(quint16 * real_num_min, qint64 num_min);


// ------------------------- EGOR FUNCS ------------------------

bool checking_filelist(QStringList filelist);
bool checking_path_exist(const auto v);
bool checking_valid_and_correct_status(QString recordFileName, QMediaRecorder::RecorderState stat);


#endif // FUNCS_H
