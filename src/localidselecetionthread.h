/*!
 *  \brief     LocalIdSelectionThread is the thread to handle the location identity selection.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef LOCALIDSELECTIONTHREAD_h
#define LOCALIDSELECTIONTHREAD_h

#include <QString>
#include <QThread>

class LocalIdSelectionThread : public QThread
{
    Q_OBJECT

public:
    explicit LocalIdSelectionThread(QObject *a_parent = Q_NULLPTR);
    ~LocalIdSelectionThread();

    void setSocketName(const QString &a_socketName);
    void run() Q_DECL_OVERRIDE;


signals:
    void newEntry(const QString &a_newEntry);
    void remoteId(const QString &a_remoteId);
    void tlsPoolLocalIdSelectionTermindated();

public slots:
    void onLocalIdentitySelected(const QString &a_localIdentitySelected);
    void onLocalIdentityRejected();

public:
    QString m_localId;
    QString m_socketName;
};

#endif // LOCALIDSELECTIONTHREAD_h
