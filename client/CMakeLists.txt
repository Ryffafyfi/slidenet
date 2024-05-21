project(Client)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

set(SOURCE_FILES main.cpp client.cpp)
add_executable(client ${SOURCE_FILES})

target_link_libraries(client Qt5::Core Qt5::Widgets Qt5::Network)
