#include "embeddedpythonstderroutstreamredirect.h"

namespace py = pybind11;

namespace qtpybind11adapter {

EmbeddedPythonStdErrOutStreamRedirect::EmbeddedPythonStdErrOutStreamRedirect()
{
    auto sysm = py::module::import("sys");
    m_stdout = sysm.attr("stdout");
    m_stderr = sysm.attr("stderr");

    auto stringio = py::module::import("io").attr("StringIO");
    m_stdoutBuffer = stringio();  // Other filelike object can be used here as well, such as objects created by pybind11
    m_stderrBuffer = stringio();

    sysm.attr("stdout") = m_stdoutBuffer;
    sysm.attr("stderr") = m_stderrBuffer;
}

EmbeddedPythonStdErrOutStreamRedirect::~EmbeddedPythonStdErrOutStreamRedirect()
{
    auto sysm = py::module::import("sys");
    sysm.attr("stdout") = m_stdout;
    sysm.attr("stderr") = m_stderr;
}

QString EmbeddedPythonStdErrOutStreamRedirect::stdOut() const
{
    m_stdoutBuffer.attr("seek")(0);
    return QString::fromStdString(m_stdoutBuffer.attr("read")().cast<std::string>());
}

QString EmbeddedPythonStdErrOutStreamRedirect::stdErr() const
{
    m_stderrBuffer.attr("seek")(0);
    return QString::fromStdString(m_stderrBuffer.attr("read")().cast<std::string>());
}

} // qtpybind11adapter
