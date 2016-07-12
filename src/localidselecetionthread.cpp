/*!
 *  \brief     LocalIdSelectionThread is the thread to handle the location identity selection.
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

#include "localidselecetionthread.h"

static QWaitCondition s_localSelected;
static QMutex s_localIdMutex;

LocalIdSelectionThread::LocalIdSelectionThread(QObject *a_parent)
    : QThread(a_parent)
{
    qDebug() << "> LocalIdSelectionThread::LocalIdSelectionThread()";
    qDebug() << "< LocalIdSelectionThread::LocalIdSelectionThread()";
}

LocalIdSelectionThread::~LocalIdSelectionThread()
{
    qDebug() << "> LocalIdSelectionThread::~LocalIdSelectionThread()";
//    qDebug() << "> LocalIdSelectionThread::~LocalIdSelectionThread::terminate()";
//    terminate();
//    qDebug() << "< LocalIdSelectionThread::~LocalIdSelectionThread::terminate()";
//    qDebug() << "> LocalIdSelectionThread::~LocalIdSelectionThread::wait()";
//    wait();
//    qDebug() << "< LocalIdSelectionThread::~LocalIdSelectionThread::wait()";
    qDebug() << "< LocalIdSelectionThread::~LocalIdSelectionThread()";
}

void LocalIdSelectionThread::setSocketName(const QString &a_socketName)
{
    qDebug() << "> LocalIdSelectionThread::setSocketName(" << a_socketName << ")";
    m_socketName = a_socketName;
    qDebug() << "< LocalIdSelectionThread::setSocketName()";
}

static char *localIdSelectionCallback (lidentry_t *a_entry, void *a_data)
{
    qDebug() << "> LocalIdSelectionThread::localIdSelectionCallback()";
    LocalIdSelectionThread *localIdSelectionThreadPtr =
        static_cast<LocalIdSelectionThread *>(a_data);

    if (a_entry->flags & PIOF_LIDENTRY_DBENTRY) {
        a_entry->localid[127] = '\0';
        qDebug() << "newEntry:" << a_entry->localid;
        emit localIdSelectionThreadPtr->newEntry(a_entry->localid);
        return nullptr;
    }
    a_entry->remoteid[127] = '\0';
    qDebug() << "remoteId:" << a_entry->remoteid;
    emit localIdSelectionThreadPtr->remoteId(a_entry->remoteid);

    s_localIdMutex.lock();
    s_localSelected.wait(&s_localIdMutex);
    QString localId = localIdSelectionThreadPtr->m_localId;
    localIdSelectionThreadPtr->m_localId.clear();
    s_localIdMutex.unlock();

    if ((localId.length() + 1) > static_cast<int>(sizeof (a_entry->localid))) {
        qWarning() << "Provided localId is too long, maximum length=" <<
                   sizeof (a_entry->localid) - 1;
    } else {
        strcpy(a_entry->localid, localId.toLatin1().constData());
        localId.clear();
    }

    qDebug() << "< LocalIdSelectionThread::localIdSelectionCallback()";
    return nullptr;
}

void LocalIdSelectionThread::run()
{
    qDebug() << "> LocalIdSelectionThread::run()";
    uint32_t regflags = PIOF_LIDENTRY_WANT_DBENTRY;
    int responsetime_sec = 300;

    // Service the TLS Pool
    errno = 0;
    int ret = tlspool_localid_service (m_socketName.toLatin1().data(),
                                       regflags, responsetime_sec, localIdSelectionCallback, this);
    if (ret == -1) {
        qWarning() << "  LocalIdSelectionThread::run::tlspool_localid_service: terminated" <<
                   std::strerror(errno);
        emit tlsPoolLocalIdSelectionTermindated();
        // \todo Remove test code below:
//        lidentry_t lidentry;
//        lidentry.flags = 0 | PIOF_LIDENTRY_DBENTRY;
//        strcpy(lidentry.localid, "aap");
//        localIdSelectionCallback(&lidentry, this);
//        strcpy(lidentry.localid, "noot");
//        localIdSelectionCallback(&lidentry, this);
//        strcpy(lidentry.localid, "mies");
//        localIdSelectionCallback(&lidentry, this);
//        strcpy(lidentry.remoteid, "juf");
//        lidentry.flags = 0;
//        localIdSelectionCallback(&lidentry, this);
    }
    qDebug() << "< LocalIdSelectionThread::run()";
}

void LocalIdSelectionThread::onLocalIdentitySelected(const QString &a_localIdentitySelected)
{
    qDebug() << "< LocalIdSelectionThread::onLocalIdentitySelected(" << a_localIdentitySelected <<
             ")";
    s_localIdMutex.lock();
    m_localId = a_localIdentitySelected;
    s_localSelected.wakeAll();
    s_localIdMutex.unlock();
    qDebug() << "< LocalIdSelectionThread::onLocalIdentitySelected()";
}

void LocalIdSelectionThread::onLocalIdentityRejected()
{
    qDebug() << "< LocalIdSelectionThread::onLocalIdentityRejected()";
    s_localIdMutex.lock();
    m_localId.clear();
    s_localSelected.wakeAll();
    s_localIdMutex.unlock();
    qDebug() << "< LocalIdSelectionThread::onLocalIdentityRejected()";
}
