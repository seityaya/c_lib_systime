#Author                 : Seityagiya Terlekchi
#Contacts               : terlekchiseityaya@gmail.com
#Creation Date          : 2024.10
#License Link           : https://spdx.org/licenses/LGPL-2.1-or-later.html
#SPDX-License-Identifier: LGPL-2.1-or-later
#Copyright © 2024-2024 Seityagiya Terlekchi. All rights reserved.

cmake_minimum_required(VERSION 3.0)

project(yaya_systime_project)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

add_subdirectory(core)
add_subdirectory(test)
add_subdirectory(using)
