#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "funcs.h"

TEST_CASE("Valid_receiving - Case 1")
{
    CHECK(valid_receiving(QDataStream::Ok, (qint64)sizeof(quint16)) == true);
}
TEST_CASE("Valid_receiving - Case 2")
{
    CHECK(valid_receiving(QDataStream::ReadPastEnd, (qint64)sizeof(quint16)) == false);
}
TEST_CASE("Valid_receiving - Case 3")
{
    CHECK(valid_receiving(QDataStream::ReadCorruptData, (qint64)sizeof(quint16)) == false);
}
TEST_CASE("Valid_receiving - Case 4")
{
    CHECK(valid_receiving(QDataStream::Ok, (quint16)sizeof(qint64)) == false);
}


TEST_CASE("Chack_and_change_slide_num - Case 1")
{
    quint16 slide_num = quint16(14);
    CHECK(chack_and_change_slide_num(&slide_num, quint16(15), "incr") == true);
    CHECK(slide_num == 15);
}
TEST_CASE("Chack_and_change_slide_num - Case 2")
{
    quint16 slide_num = quint16(16);
    CHECK(chack_and_change_slide_num(&slide_num, quint16(15), "incr") == false);
    CHECK(slide_num == 16);
}
TEST_CASE("Chack_and_change_slide_num - Case 3")
{
    quint16 slide_num = quint16(7);
    CHECK(chack_and_change_slide_num(&slide_num, quint16(2), "decr") == true);
    CHECK(slide_num == 6);
}
TEST_CASE("Chack_and_change_slide_num - Case 4")
{
    quint16 slide_num = quint16(2);
    CHECK(chack_and_change_slide_num(&slide_num, quint16(3), "decr") == false);
    CHECK(slide_num == 2);
}


TEST_CASE("Set_num_min - Case 1")
{
    quint16 real_num_min = 4;
    set_num_min(&real_num_min, 5);
    CHECK(real_num_min == 5);
}
TEST_CASE("Set_num_min - Case 2")
{
    quint16 real_num_min = 6;
    set_num_min(&real_num_min, -2);
    CHECK(real_num_min == 6);
}
TEST_CASE("Set_num_min - Case 3")
{
    quint16 real_num_min = 1;
    set_num_min(&real_num_min, 11);
    CHECK(real_num_min == 11);
}
TEST_CASE("Set_num_min - Case 4")
{
    quint16 real_num_min = 3;
    set_num_min(&real_num_min, 0);
    CHECK(real_num_min == 3);
}


// ---------------- EGOR TESTS ------------------------------

TEST_CASE("checking_filelist - Case 1")
{
    QStringList test_filelist = {"Qwe", "Rty"};
    CHECK(checking_filelist(test_filelist) == false);
}
TEST_CASE("checking_filelist - Case 2")
{
    QStringList test_filelist = {"Qwe", "Rty", "said"};
    CHECK(checking_filelist(test_filelist) == false);
}
TEST_CASE("checking_filelist - Case 3")
{
    QStringList test_filelist = {};
    CHECK(checking_filelist(test_filelist) == true);
}
TEST_CASE("checking_filelist - Case 4")
{
    QStringList test_filelist = {"123", "090"};
    CHECK(checking_filelist(test_filelist) == false);
}


TEST_CASE("checking_path_exist - Case 1")
{
    const auto test_v = ""; // вставить существующий путь
    CHECK(checking_path_exist(test_v) == true);
}
TEST_CASE("checking_path_exist - Case 2")
{
    const auto test_v = ""; // вставить несуществующий путь
    CHECK(checking_path_exist(test_v) == false);
}
TEST_CASE("checking_path_exist - Case 3")
{
    const auto test_v = ""; // вставить существующий путь
    CHECK(checking_path_exist(test_v) == true);
}
TEST_CASE("checking_path_exist - Case 4")
{
    const auto test_v = ""; // вставить несуществующий путь
    CHECK(checking_path_exist(test_v) == false);
}


TEST_CASE("checking_valid_and_correct_status - Case 1")
{
    QString test_recordFileName = ""; // вставить существующий путь
    QMediaRecorder::RecorderState test_stat = QMediaRecorder::StoppedState;
    CHECK(checking_valid_and_correct_status(test_recordFileName, test_stat) == true);
}
TEST_CASE("checking_valid_and_correct_status - Case 1")
{
    QString test_recordFileName = ""; // вставить несуществующий путь
    QMediaRecorder::RecorderState test_stat = QMediaRecorder::StoppedState;
    CHECK(checking_valid_and_correct_status(test_recordFileName, test_stat) == false);
}
TEST_CASE("checking_valid_and_correct_status - Case 1")
{
    QString test_recordFileName = ""; // вставить несуществующий путь
    QMediaRecorder::RecorderState test_stat = QMediaRecorder::RecordingState;
    CHECK(checking_valid_and_correct_status(test_recordFileName, test_stat) == false);
}
TEST_CASE("checking_valid_and_correct_status - Case 1")
{
    QString test_recordFileName = ""; // вставить существующий путь
    QMediaRecorder::RecorderState test_stat = QMediaRecorder::PausedState;
    CHECK(checking_valid_and_correct_status(test_recordFileName, test_stat) == false);
}
