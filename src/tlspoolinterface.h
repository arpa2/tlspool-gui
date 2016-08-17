/*!
 *  \brief     TlsPoolInterface is the spider in the web.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef TLSPOOLINTERFACE_H
#define TLSPOOLINTERFACE_H

#include <QObject>
#include <QSettings>
#include <QStringListModel>

#include "pinentrydialog.h"
#include "selectlocalidentitydialog.h"
#include "systemtrayitem.h"
#include "tlspoolguisettings.h"

class TlsPoolInterface : public QObject
{
    Q_OBJECT

public:
    explicit TlsPoolInterface(QObject *a_parent = Q_NULLPTR);
    ~TlsPoolInterface();

    void showSystemTray();

signals:
    void pinEntered(const QString &a_pin);
    void pinRejected();

    void localIdentityListUpdated(const QStringList &a_localIdentityList);
    void localIdentitySelected(const QString &a_localIdentity, bool a_remember);
    void localIdentityRejected();
    void defaultLocalIdentitySelected(const QString &a_identity);

protected slots:
    void onPinRequested(const QString &a_prompt, ulong a_maxPinLength,
                        quint32 a_nrOfAttemptFailures, quint32 a_timeout_us,
                        const QString &a_tokenManufacturer,
                        const QString &a_tokenModel,
                        const QString &a_tokenSerialNumber,
                        const QString &a_tokenLabel);
    void onPinEntryDialogRejected();
    void onPinEntryDialogAccepted(const QString &a_pin);
    void onTlsPoolPinServiceTermindated();

    void onNewEntry(const QString &a_newEntry);
    void onRemoteId(const QString &a_remoteId);
    void onLocalIdentityRejected();
    void onLocalIdentitySelected(const QString &a_localIdentity, bool a_remember);
    void onTlsPoolLocalIdSelectionTermindated();

    void onDefaultLocalIdentity(const QString &a_identity);
    void onDeleteDefaultLocalIdentity();

private:
    void startPinEntryThread();
    void startLocalIdSelectionThread();

    SystemTrayItem            *m_systemTrayItem;
    PinEntryDialog            *m_pinEntryDialog;
    SelectLocalIdentityDialog *m_selectLocalIdentityDialog;
    TlsPoolGuiSettings        *m_settings;
    QStringList                m_localIdentityList;
    QString                    m_remoteId;
    QString                    m_defaultLocalIdentity;
};

#endif // TLSPOOLINTERFACE_H
