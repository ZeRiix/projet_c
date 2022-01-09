cmake_minimum_required(VERSION 3.21)
project(projet_v3 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-Wall")
include_directories("/usr/local/Cellar/mariadb/10.6.4/include")
include_directories("/usr/local/Cellar/gtk+3/3.24.30_1/include/gtk-3.0")
include_directories("/usr/local/Cellar/curl/7.80.0_1/include/curl")

add_library(libmariadbclient SHARED IMPORTED)
add_library(libgtk-3.0 SHARED IMPORTED)
add_library(curl SHARED IMPORTED)

set_target_properties(
        libmariadbclient
        PROPERTIES LINKER_LANGUAGE C
        IMPORTED_IMPLIB "/usr/local/Cellar/mariadb/10.6.4/lib/libmariadbclient.dylib")

set_target_properties(
        libgtk-3.0
        PROPERTIES LINKER_LANGUAGE C
        IMPORTED_IMPLIB "/usr/local/Cellar/gtk+3/3.24.30_1/lib/libgtk-3.0.dylib")

set_target_properties(
        curl
        PROPERTIES LINKER_LANGUAGE C
        IMPORTED_IMPLIB "/usr/local/Cellar/curl/7.80.0_1/lib/libcurl.dylib")


link_directories("/usr/local/Cellar/mariadb/10.6.4/lib")
link_directories("/usr/local/Cellar/gtk+3/3.24.30_1/lib")
link_directories("/usr/local/Cellar/curl/7.80.0_1/lib")


add_executable(projet_v3 main.c)
target_link_libraries(projet_v3 libmariadb.dylib)
target_link_libraries(projet_v3 libgtk-3.0.dylib)
target_link_libraries(projet_v3 libcurl.dylib)