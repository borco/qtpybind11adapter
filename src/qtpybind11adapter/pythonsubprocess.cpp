#include "pythonsubprocess.h"

#include <QDebug>
#include <QFileInfo>
#include <QProcess>

namespace qtpybind11adapter {

PythonSubprocess::PythonSubprocess(const QByteArray &separator, QObject *parent)
    : QObject{parent}
    , m_separator(separator)
{
}

PythonSubprocess::PythonSubprocess(QObject *parent)
    : PythonSubprocess("----------", parent)
{
}

PythonSubprocess::~PythonSubprocess()
{
    qDebug() << "Deleted python subprocess";
}

void PythonSubprocess::startSubprocess(const QString &program, const QByteArray &input)
{
    if (m_process) {
        throw QString("Another process already running");
    }

    m_process = new QProcess(this);
    m_process->setProcessChannelMode(QProcess::MergedChannels);

    connect(m_process, &QProcess::finished, this, [this]() {
        m_output.append(m_process->readAllStandardOutput());
        m_exitCode = m_process->exitCode();
        m_process->deleteLater();
        m_process = nullptr;
        parseOutput();
        processOutput();
        printExitCode();
    });

    connect(m_process, &QProcess::errorOccurred, this, [this](QProcess::ProcessError error){
        m_output.append(m_process->readAllStandardOutput());
        m_process->deleteLater();
        m_process = nullptr;
        parseOutput();
        processOutput();
        qDebug() << "Python subprocess failed:" << error;
    });

    connect(m_process, &QProcess::started, this, [this, input]() {
        m_process->write(input);
        m_process->write("\n" + m_separator + "\n");
    });

    connect(m_process, &QProcess::readyReadStandardOutput, this, [this](){
       m_output.append(m_process->readAllStandardOutput());
    });

    m_process->start(Python3_EXECUTABLE, QStringList() << m_separator);
}

void PythonSubprocess::printExitCode() const
{
    if (exitCode() == 0) {
        qDebug().noquote() << "Python subprocess exited cleanly";
    } else {
        qWarning().noquote() << "Python subprocess failed\nExit code:\n" << exitCode() << "\n" << output();
    }
}

void PythonSubprocess::parseOutput()
{
    auto pos = m_output.indexOf(m_separator);
    if (pos >= 0) {
        m_results = m_output.mid(pos + m_separator.size());
        m_debug = m_output.first(pos);
    } else {
        m_results = QByteArray();
        m_debug = m_output;
    }
}

void PythonSubprocess::terminate()
{
    if (m_process) {
        m_process->terminate();
    }
}

void PythonSubprocess::kill()
{
    if (m_process) {
        m_process->kill();
    }
}

} // qtpybind11adapter
