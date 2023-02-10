set(LIB_NAME QtPyBind11Adapter)

qt_add_library(${LIB_NAME} STATIC
    src/qtpybind11adapter/embeddedpython.h
    src/qtpybind11adapter/embeddedpythonstderroutstreamredirect.cpp
    src/qtpybind11adapter/embeddedpythonstderroutstreamredirect.h
    src/qtpybind11adapter/pythonsubprocess.cpp
    src/qtpybind11adapter/pythonsubprocess.h
)

target_compile_definitions(${LIB_NAME} PUBLIC
    PROJECT_PYTHON_SITELIB="${PROJECT_PYTHON_SITELIB}"
    PROJECT_PYTHON_BIN="${PROJECT_PYTHON_BIN}"
    Python3_EXECUTABLE="${Python3_EXECUTABLE}"
    Python3_SITEARCH="${Python3_SITEARCH}"
    Python3_SITELIB="${Python3_SITELIB}"
)

target_link_libraries(${LIB_NAME} PUBLIC
    Qt6::Core
    pybind11::embed
    iborcocpputils
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)

add_subdirectory(src/pybind11)
