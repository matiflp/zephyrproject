cmake_minimum_required(VERSION 3.20.0)

zephyr_library_amend()
zephyr_library_sources(zephyr_course_nrfx_temp.c)

set_source_files_properties("${ZEPHYR_BASE}/../modules/hal/nordic/nrfx/drivers/src/nrfx_temp.c" TARGET_DIRECTORY ${lib_name} PROPERTIES HEADER_FILE_ONLY ON)
