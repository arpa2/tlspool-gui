/*!
 *  \brief     PinEntryThread is the thread to handle the pin entry.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <cstring>

#include <tlspool/commands.h>
#include <tlspool/starttls.h>

#include <QtCore>
#include <QDebug>

#include "pinentrythread.h"

static QWaitCondition s_pinEntered;
static QMutex s_pinMutex;

PinEntryThread::PinEntryThread(QObject *a_parent)
    : QThread(a_parent)
{
    qDebug() << "> PinEntryThread::PinEntryThread()";
    qDebug() << "< PinEntryThread::PinEntryThread()";
}

PinEntryThread::~PinEntryThread()
{
    qDebug() << "> PinEntryThread::~PinEntryThread()";
//    qDebug() << "> PinEntryThread::~PinEntryThread::terminate()";
//    terminate();
//    qDebug() << "< PinEntryThread::~PinEntryThread::terminate()";
//    qDebug() << "> PinEntryThread::~PinEntryThread::wait()";
//    wait();
//    qDebug() << "< PinEntryThread::~PinEntryThread::wait()";
    qDebug() << "< PinEntryThread::~PinEntryThread()";
}

void PinEntryThread::setSocketName(const QString &a_socketName)
{
    qDebug() << "> PinEntryThread::setSocketName(" << a_socketName << ")";
    m_socketName = a_socketName;
    qDebug() << "< PinEntryThread::setSocketName()";
}

static void pinServiceCallback(pinentry_t *a_pinEntry, void *a_data)
{
    qDebug() << "> PinEntryThread::pinServiceCallback()";
    PinEntryThread *pinEntryThreadPtr = static_cast<PinEntryThread *>(a_data);

    memset (a_pinEntry->pin, 0, sizeof (a_pinEntry->pin));
    emit pinEntryThreadPtr->pinRequested(a_pinEntry->prompt, sizeof (a_pinEntry->pin) - 1,
                                         a_pinEntry->attempt, a_pinEntry->timeout_us,
                                         a_pinEntry->token_manuf, a_pinEntry->token_model,
                                         a_pinEntry->token_serial, a_pinEntry->token_label);
    s_pinMutex.lock();
    s_pinEntered.wait(&s_pinMutex);
    QString pin = pinEntryThreadPtr->m_pin;
    pinEntryThreadPtr->m_pin.clear();
    s_pinMutex.unlock();
    if ((pin.length() + 1) > static_cast<int>(sizeof (a_pinEntry->pin))) {
        qWarning() << "Provided PIN is too long, maximum length=" << sizeof (a_pinEntry->pin) - 1;
    } else {
        strcpy(a_pinEntry->pin, pin.toLatin1().constData());
        pin.clear();
    }
    QString obfuscate = "*";

    qDebug() << "< PinEntryThread::pinServiceCallback(" <<
             obfuscate.repeated(QString(a_pinEntry->pin).length()) << ")";
}

void PinEntryThread::run()
{
    qDebug() << "> PinEntryThread::run()";
    uint32_t regflags = 0;
    uint32_t responsetime_usec = 60 * 1000 * 1000;

    // Service the TLS Pool
    errno = 0;
    int ret = tlspool_pin_service (m_socketName.toLatin1().data(), regflags,
                                   responsetime_usec, pinServiceCallback, this);
    if (ret == -1) {
        qWarning() << "  PinEntryThread::run::tlspool_pin_service: terminated, errno" <<
                   std::strerror(errno);
        emit tlsPoolPinServiceTermindated();
        // \todo Remove test code below:
//        pinentry_t pinEntry;
//        pinEntry.attempt = 1;
//        strcpy(pinEntry.prompt, "Please provide a PIN");
//        strcpy(pinEntry.token_manuf, "manufacturer");
//        strcpy(pinEntry.token_model, "model");
//        strcpy(pinEntry.token_serial, "serial number");
//        strcpy(pinEntry.token_label, "");
//        pinServiceCallback(&pinEntry, this);
    }
    qDebug() << "< PinEntryThread::run()";
}

void PinEntryThread::onPinEntered(const QString &a_pin)
{
    QString obfuscate = "*";

    qDebug() << "< PinEntryThread::onPinEntered(" << obfuscate.repeated(a_pin.length()) << ")";
    s_pinMutex.lock();
    m_pin = a_pin;
    s_pinEntered.wakeAll();
    s_pinMutex.unlock();
    qDebug() << "< PinEntryThread::onPinEntered()";
}

void PinEntryThread::onPinRejected()
{
    qDebug() << "< PinEntryThread::onPinRejected()";
    s_pinMutex.lock();
    m_pin.clear();
    s_pinEntered.wakeAll();
    s_pinMutex.unlock();
    qDebug() << "< PinEntryThread::onPinRejected()";
}
