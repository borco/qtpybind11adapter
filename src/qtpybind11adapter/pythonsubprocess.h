#pragma once

#include <QObject>

class QProcess;

namespace qtpybind11adapter {

class PythonSubprocess : public QObject
{
    Q_OBJECT

public:
    virtual ~PythonSubprocess();

    void terminate();
    void kill();

    const QByteArray& output() const {return m_output;} // all output
    const QByteArray& results() const {return m_results;} // only results, after the separator
    const QByteArray& debug() const {return m_debug;} // only debug, before the separator
    int exitCode() const {return m_exitCode;} // process exit code

protected:
    explicit PythonSubprocess(const QByteArray& separator, QObject *parent = nullptr);
    explicit PythonSubprocess(QObject *parent = nullptr);

    void startSubprocess(const QString& program, const QByteArray &input);

    virtual void processOutput() = 0;

private:
    void parseOutput();
    void printExitCode() const;

    QProcess* m_process {nullptr};
    QByteArray m_separator;
    QByteArray m_output;
    QByteArray m_results;
    QByteArray m_debug;
    int m_exitCode {0};
};

} // qtpybind11adapter
