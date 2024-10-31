#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2024.10
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2024-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(yaya_systime_test)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST
    main.c
)

set(INC_LIST
)

add_executable(
    ${PROJECT_NAME}
    ${SRC_LIST}
    ${INC_LIST}
)

target_link_libraries(${PROJECT_NAME} PUBLIC yaya_systime)
