cmake_minimum_required(VERSION "3.25.0")
project("a11ytest" VERSION 1.0.0 LANGUAGES C)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED IMPORTED_TARGET gtk+-3.0 atk atspi-2)

add_executable(a11ytest main.c item.h item.c button.h button.c customcontainer.c customcontainer.h)
target_link_libraries(a11ytest PRIVATE PkgConfig::GTK)