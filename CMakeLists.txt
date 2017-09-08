cmake_minimum_required(VERSION 3.8)
project(untitled1)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c SPArrayList.c SPGame.c SPMainAux.c SPMiniMax.c SPMiniMaxNode.c SPParser.c SPSettings.c)
add_executable(untitled1 ${SOURCE_FILES})