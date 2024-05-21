project(Tests)

find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(doctest REQUIRED)

set(SOURCE_FILES server_test.cpp)
add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests Qt5::Core Qt5::Network doctest::doctest)
