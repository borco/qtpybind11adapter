#pragma once

#pragma push_macro("slots")
#undef slots
#include <pybind11/embed.h> // everything needed for embedding
#pragma pop_macro("slots")

#include "iborcocpputils/singleton.h"

#include <QDebug>

namespace qtpybind11adapter {

template <class T>
class EmbeddedPython : public iborcocpputils::Singleton<T>
{
public:
    ~EmbeddedPython() {
        pybind11::finalize_interpreter();
        qDebug() << "Deleted embedded python";
    }

protected:
    EmbeddedPython() {
        qDebug() << "Created new embedded python";
        pybind11::initialize_interpreter();

        // configure python import path
        pybind11::object sys = pybind11::module::import("sys");
        pybind11::object path = sys.attr("path");
        path.attr("insert")(0, Python3_SITEARCH);
        path.attr("insert")(0, Python3_SITELIB);
        path.attr("insert")(0, PROJECT_PYTHON_SITELIB);
    }
};

} // qtpybind11adapter
