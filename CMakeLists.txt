cmake_minimum_required(VERSION 3.8)
project(untitled1)

set(CMAKE_C_STANDARD 99)
set(gcc Wall Werror Wextra pedantic-errors)

set(SOURCE_FILES main.c SPGame.c SPMiniMax.c SPMiniMaxNode.c SPParser.c SPSettings.c SPGameFlow.c SPQueue.c SPMoves.h SPMoves.c MiniMax.c)
add_executable(untitled1 ${SOURCE_FILES})