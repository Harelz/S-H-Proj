cmake_minimum_required(VERSION 3.8)
project(untitled1)

set(SDL2_INCLUDE_DIR C:/SDL2-2.0.5/x86_64-w64-mingw32/include/SDL2)
include_directories(${SDL2_INCLUDE_DIR})

set(prefix "C:/MinGW/x86_64-w64-mingw32")
set(exec_prefix "${prefix}")
set(libdir "${exec_prefix}/lib")
set(SDL2_PREFIX "C:/SDL2-2.0.5/x86_64-w64-mingw32")
set(SDL2_EXEC_PREFIX "C:/SDL2-2.0.5/x86_64-w64-mingw32")
set(SDL2_LIBDIR "${exec_prefix}/lib")
set(SDL2_INCLUDE_DIRS "${prefix}/include/SDL2")

add_library(SDL2 STATIC IMPORTED)
set_property(TARGET SDL2 PROPERTY IMPORTED_LOCATION C:/SDL2-2.0.5/x86_64-w64-mingw32/lib/libSDL2.a)
add_library(SDL2main STATIC IMPORTED)
set_property(TARGET SDL2main PROPERTY IMPORTED_LOCATION C:/SDL2-2.0.5/x86_64-w64-mingw32/lib/libSDL2main.a)


set(SDL2_LIBRARIES "-L${SDL2_LIBDIR} -lmingw32")
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)



configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\back.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\back.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_bishop.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_bishop.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_king.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_king.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_knight.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_knight.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_pawn.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_pawn.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_player.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_player.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_queen.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_queen.bmp COPYONLY)
configure_file(C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_rook.bmp C:\\Users\\hoshri\\Documents\\Harelz\\S-H-Proj\\Graphics\\images\\black_rook .bmp COPYONLY)

set(CMAKE_C_STANDARD 99)
set(gcc Wall Werror Wextra pedantic-errors)
set(SOURCE_FILES main.c SPGame.c SPMiniMax.c SPParser.c SPSettings.c SPQueue.c SPMoves.h SPMoves.c
        Graphics/guimode.c Graphics/SPCHESSGUIButton.c Graphics/SPCHESSGUICommon.c Graphics/SPCHESSGUIGameWin.c Graphics/SPCHESSGUILoadWin.c
        Graphics/SPCHESSGUIMainWin.c Graphics/SPCHESSGUIManager.c Graphics/SPCHESSGUISetWin.c
        )
add_executable(untitled1 ${SOURCE_FILES})
target_link_libraries(untitled1 ${SDL2_LIBRARIES} SDL2main SDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc)
