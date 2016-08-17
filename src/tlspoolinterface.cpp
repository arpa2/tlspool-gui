/*!
 *  \brief     TlsPoolInterface is the spider in the web.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <tlspool/starttls.h>

#include <QDebug>
#include <QTimer>

#include "localidselecetionthread.h"
#include "pinentrythread.h"
#include "selectlocalidentitydialog.h"
#include "tlspoolinterface.h"

static int RESTART_THREAD_TIME = 5000;

TlsPoolInterface::TlsPoolInterface(QObject *a_parent)
    : QObject(a_parent)
    , m_pinEntryDialog(nullptr)
    , m_selectLocalIdentityDialog(nullptr)
{
    qDebug() << "> TlsPoolInterface::TlsPoolInterface()";

    m_settings = new TlsPoolGuiSettings("tlspool-gui.ini", this);

    m_systemTrayItem = new SystemTrayItem(a_parent);
    connect(this, &TlsPoolInterface::localIdentityListUpdated,
            m_systemTrayItem, &SystemTrayItem::onLocalIdentityListUpdated);
    connect(m_systemTrayItem, &SystemTrayItem::defaultLocalIdentity,
            this, &TlsPoolInterface::onDefaultLocalIdentity);
    connect(m_systemTrayItem, &SystemTrayItem::deleteDefaultLocalIdentity,
            this, &TlsPoolInterface::onDeleteDefaultLocalIdentity);
    connect(this, &TlsPoolInterface::defaultLocalIdentitySelected,
            m_systemTrayItem, &SystemTrayItem::onDefaultLocalIdentitySelected);

    startPinEntryThread();
    startLocalIdSelectionThread();
    qDebug() << "< TlsPoolInterface::TlsPoolInterface()";
}

TlsPoolInterface::~TlsPoolInterface()
{
    qDebug() << "> TlsPoolInterface::~TlsPoolInterface()";
    qDebug() << "< TlsPoolInterface::~TlsPoolInterface()";
}

void TlsPoolInterface::showSystemTray()
{
    qDebug() << "> TlsPoolInterface::showSystemTray()";
    m_systemTrayItem->show();
    qDebug() << "< TlsPoolInterface::showSystemTray()";
}

void TlsPoolInterface::startPinEntryThread()
{
    qDebug() << "> TlsPoolInterface::startPinEntryThread()";
    PinEntryThread *pinEntryThread = new PinEntryThread(this);

    connect(pinEntryThread, &PinEntryThread::finished, pinEntryThread, &QObject::deleteLater);
    connect(pinEntryThread, &PinEntryThread::pinRequested, this, &TlsPoolInterface::onPinRequested);
    connect(pinEntryThread, &PinEntryThread::tlsPoolPinServiceTermindated, this,
            &TlsPoolInterface::onTlsPoolPinServiceTermindated);
    connect(this, &TlsPoolInterface::pinEntered, pinEntryThread, &PinEntryThread::onPinEntered);
    connect(this, &TlsPoolInterface::pinRejected, pinEntryThread, &PinEntryThread::onPinRejected);

    pinEntryThread->setSocketName(m_settings->socketName());
    pinEntryThread->start();
    qDebug() << "< TlsPoolInterface::startPinEntryThread()";
}

void TlsPoolInterface::startLocalIdSelectionThread()
{
    qDebug() << "> TlsPoolInterface::startLocalIdSelectionThread()";
    LocalIdSelectionThread *localIdSelectionThread = new LocalIdSelectionThread(this);

    connect(localIdSelectionThread, &LocalIdSelectionThread::finished, localIdSelectionThread,
            &QObject::deleteLater);
    connect(localIdSelectionThread, &LocalIdSelectionThread::newEntry, this,
            &TlsPoolInterface::onNewEntry);
    connect(localIdSelectionThread, &LocalIdSelectionThread::remoteId, this,
            &TlsPoolInterface::onRemoteId);
    connect(localIdSelectionThread, &LocalIdSelectionThread::tlsPoolLocalIdSelectionTermindated,
            this, &TlsPoolInterface::onTlsPoolLocalIdSelectionTermindated);
    connect(this, &TlsPoolInterface::localIdentitySelected, localIdSelectionThread,
            &LocalIdSelectionThread::onLocalIdentitySelected);
    connect(this, &TlsPoolInterface::localIdentityRejected, localIdSelectionThread,
            &LocalIdSelectionThread::onLocalIdentityRejected);

    localIdSelectionThread->setSocketName(m_settings->socketName());
    localIdSelectionThread->start();
    qDebug() << "< TlsPoolInterface::startLocalIdSelectionThread()";
}

void TlsPoolInterface::onPinRequested(const QString &a_prompt, ulong a_maxPinLength,
                                      quint32 a_nrOfAttemptFailures, quint32 a_timeout_us,
                                      const QString &a_tokenManufacturer,
                                      const QString &a_tokenModel,
                                      const QString &a_tokenSerialNumber,
                                      const QString &a_tokenLabel)
{
    qDebug() << "> TlsPoolInterface::onPinRequested(prompt=" << a_prompt <<
             ", pinSize=" << a_maxPinLength <<
             ", nrOfAttemptFailures=" << a_nrOfAttemptFailures <<
             ", timeout_us=" << a_timeout_us <<
             ", token manufacturer=" << a_tokenManufacturer <<
             ", token model=" << a_tokenModel <<
             ", token serial number=" << a_tokenSerialNumber <<
             ", token label=" << a_tokenLabel << ")";

    delete m_pinEntryDialog;
    m_pinEntryDialog = new PinEntryDialog;

    if (!a_prompt.isEmpty()) {
        // Only overwrite the prompt if there is a replacement text
        m_pinEntryDialog->setPrompt(a_prompt);
    }
    m_pinEntryDialog->setMaxPinLength(a_maxPinLength);
    m_pinEntryDialog->setNrOfAttemptFailures(a_nrOfAttemptFailures);
    m_pinEntryDialog->setTimeout(a_timeout_us);
    m_pinEntryDialog->setTokenManufacturer(a_tokenManufacturer);
    m_pinEntryDialog->setTokenModel(a_tokenModel);
    m_pinEntryDialog->setTokenSerialNumber(a_tokenSerialNumber);
    m_pinEntryDialog->setTokenLabel(a_tokenLabel);

    connect(m_pinEntryDialog, &QDialog::rejected,
            this, &TlsPoolInterface::onPinEntryDialogRejected);
    connect(m_pinEntryDialog, &PinEntryDialog::pinEntered, this,
            &TlsPoolInterface::onPinEntryDialogAccepted);
    m_pinEntryDialog->show();
    qDebug() << "< TlsPoolInterface::onPinRequested()";
}

void TlsPoolInterface::onPinEntryDialogRejected()
{
    qDebug() << "> TlsPoolInterface::onPinEntryDialogRejected()";
    emit pinRejected();
    qDebug() << "< TlsPoolInterface::onPinEntryDialogRejected()";
}

void TlsPoolInterface::onPinEntryDialogAccepted(const QString &a_pin)
{
    QString obfuscate = "*";

    qDebug() << "> TlsPoolInterface::onPinEntryDialogAccepted(" <<
             obfuscate.repeated(a_pin.length()) << ")";
    emit pinEntered(a_pin);
    qDebug() << "< TlsPoolInterface::onPinEntryDialogAccepted()";
}

void TlsPoolInterface::onTlsPoolPinServiceTermindated()
{
    qDebug() << "> TlsPoolInterface::onTlsPoolPinServiceTermindated()";
    delete m_pinEntryDialog;
    m_pinEntryDialog = nullptr;
    QTimer::singleShot(RESTART_THREAD_TIME, this, &TlsPoolInterface::startPinEntryThread);
    qDebug() << "< TlsPoolInterface::onTlsPoolPinServiceTermindated()";
}

void TlsPoolInterface::onNewEntry(const QString &a_newEntry)
{
    qDebug() << "> TlsPoolInterface::newEntry(" << a_newEntry << ")";
    if (!m_localIdentityList.contains(a_newEntry)) {
        m_localIdentityList.append(a_newEntry);
        emit localIdentityListUpdated(m_localIdentityList);
    }
    qDebug() << "< TlsPoolInterface::newEntry()";
}

void TlsPoolInterface::onRemoteId(const QString &a_remoteId)
{
    qDebug() << "> TlsPoolInterface::remoteId(" << a_remoteId << ")";
    m_remoteId = a_remoteId;

    if (!m_defaultLocalIdentity.isEmpty()) {
        // There is already a local identity selected, don't show the dialog
        emit localIdentitySelected(m_defaultLocalIdentity, false);
    } else {
        delete m_selectLocalIdentityDialog;
        m_selectLocalIdentityDialog = new SelectLocalIdentityDialog;

        m_selectLocalIdentityDialog->setLocalIdentityList(m_localIdentityList);
        m_selectLocalIdentityDialog->setRemoteIdentity(m_remoteId);

        connect(m_selectLocalIdentityDialog, &QDialog::rejected, this,
                &TlsPoolInterface::onLocalIdentityRejected);
        connect(m_selectLocalIdentityDialog, &SelectLocalIdentityDialog::localIdentitySelected, this,
                &TlsPoolInterface::onLocalIdentitySelected);

        m_selectLocalIdentityDialog->show();
    }
    qDebug() << "< TlsPoolInterface::remoteId()";
}

void TlsPoolInterface::onLocalIdentityRejected()
{
    qDebug() << "> TlsPoolInterface::onLocalIdentityRejected()";
    emit localIdentityRejected();
    qDebug() << "< TlsPoolInterface::onLocalIdentityRejected()";
}

void TlsPoolInterface::onLocalIdentitySelected(const QString &a_localIdentity, bool a_remember)
{
    qDebug() << "> TlsPoolInterface::onLocalIdentitySelected(" <<
             a_localIdentity << a_remember << ")";
    emit localIdentitySelected(a_localIdentity, a_remember);
    qDebug() << "< TlsPoolInterface::onLocalIdentitySelected()";
}

void TlsPoolInterface::onTlsPoolLocalIdSelectionTermindated()
{
    qDebug() << "> TlsPoolInterface::tlsPoolLocalIdSelectionTermindated()";
    delete m_selectLocalIdentityDialog;
    m_selectLocalIdentityDialog = nullptr;
    QTimer::singleShot(RESTART_THREAD_TIME, this, &TlsPoolInterface::startLocalIdSelectionThread);
    qDebug() << "< TlsPoolInterface::tlsPoolLocalIdSelectionTermindated()";
}

void TlsPoolInterface::onDefaultLocalIdentity(const QString &a_identity)
{
    qDebug() << "> TlsPoolInterface::onDefaultLocalIdentity(" << a_identity << ")";
    m_defaultLocalIdentity = a_identity;
    emit defaultLocalIdentitySelected(m_defaultLocalIdentity);
    qDebug() << "< TlsPoolInterface::onDefaultLocalIdentity()";
}

void TlsPoolInterface::onDeleteDefaultLocalIdentity()
{
    qDebug() << "> TlsPoolInterface::onDeleteDefaultLocalIdentity()";
    m_defaultLocalIdentity.clear();
    emit defaultLocalIdentitySelected("");
    qDebug() << "< TlsPoolInterface::onDeleteDefaultLocalIdentity(): default local identity deleted";
}
