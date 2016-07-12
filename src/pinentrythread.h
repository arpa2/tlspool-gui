/*!
 *  \brief     PinEntryThread is the thread to handle the pin entry.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef PINENTRYTHREAD_H
#define PINENTRYTHREAD_H

#include <QString>
#include <QThread>

class PinEntryThread : public QThread
{
    Q_OBJECT

public:
    explicit PinEntryThread(QObject *a_parent = Q_NULLPTR);
    ~PinEntryThread();

    void setSocketName(const QString &a_socketName);
    void run() Q_DECL_OVERRIDE;

signals:
    void pinRequested(const QString &prompt, ulong pinSize, quint32 nrOfAttemptFailures,
                      quint32 a_timeout_us,
                      const QString &a_tokenManufacturer, const QString &a_tokenModel,
                      const QString &a_tokenSerialNumber, const QString &a_tokenLabel);
    void tlsPoolPinServiceTermindated();
public slots:
    void onPinEntered(const QString &a_pin);
    void onPinRejected();

public:
    QString m_pin;
    QString m_socketName;
};

#endif // PINENTRYTHREAD_H
