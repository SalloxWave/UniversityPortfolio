cmake_minimum_required(VERSION 3.6)
project(game38)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp player.cpp player.h level.cpp level.h projectile.cpp projectile.h
        base_window.cpp base_window.h gameboard.cpp gameboard.h main_menu.cpp main_menu.h
        characterwindow.cpp characterwindow.h enum_player_action.h struct_point.h entity.cpp entity.h
        enum_direction.h enum_player_state.h enum_projectile_state.h resources game38.cpp game38.h enum_window_state.h winning_window.cpp winning_window.h enum_winning_option.h enum_menu_choice.h)
add_executable(game38 ${SOURCE_FILES})

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")
find_package(SFML REQUIRED system window graphics network audio)
if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(game38 ${SFML_LIBRARIES})
endif()
