#ifndef FUNCS_H
#define FUNCS_H

#include "qtypes.h"
#include "qdatastream.h"
#include "qstring.h"

bool valid_receiving(QDataStream::Status indata, qint64 available);
bool chack_and_change_slide_num(quint16 * slide_num, quint16 slide_num_inf, QString mod);
void set_num_min(quint16 * real_num_min, qint64 num_min);

#endif // FUNCS_H
