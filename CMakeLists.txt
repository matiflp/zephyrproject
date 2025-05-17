add_subdirectory(
    ${CMAKE_CURRENT_LIST_DIR}/modules/hal_nordic/nrfx
    ${CMAKE_CURRENT_BINARY_DIR}/zephyr_course_nrfx
)

zephyr_syscall_header(${ZEPHYR_MYCOURSEMODULE_MODULE_DIR}/include/blinker.h)

zephyr_library()

zephyr_library_sources(drivers/blinker.c)
zephyr_include_directories(${ZEPHYR_MYCOURSEMODULE_MODULE_DIR}/include)


