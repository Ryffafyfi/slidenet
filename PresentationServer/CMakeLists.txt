project(Server)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

set(SOURCE_FILES main.cpp server.cpp)
add_executable(server ${SOURCE_FILES})

target_link_libraries(server Qt5::Core Qt5::Network)
