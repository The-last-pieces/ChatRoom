cmake_minimum_required(VERSION 3.10.2)
project(chat_room)

set(CMAKE_CXX_STANDARD 17)

set(SERVER_NAME server)
set(CLIENT_NAME client)

add_executable(
    ${SERVER_NAME} 
    src/server.cpp
)

add_executable(
    ${CLIENT_NAME} 
    src/client.cpp
)

target_link_libraries(${SERVER_NAME} -lmysqlclient -pthread)