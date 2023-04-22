#pragma once

// from https://github.com/pybind/pybind11/issues/1622

#include <QString>

#pragma push_macro("slots")
#undef slots
#include <pybind11/embed.h>
#pragma pop_macro("slots")

namespace qtpybind11adapter {

class EmbeddedPythonStdErrOutStreamRedirect
{
public:
    EmbeddedPythonStdErrOutStreamRedirect();
    ~EmbeddedPythonStdErrOutStreamRedirect();

    QString stdOut() const;
    QString stdErr() const;

private:
    pybind11::object m_stdout;
    pybind11::object m_stderr;
    pybind11::object m_stdoutBuffer;
    pybind11::object m_stderrBuffer;
};

} // qtpybind11adapter
