/*!
 *  \brief     SystemTrayItem is the item that shows an icon to interact with the TLS Pool GUI.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef SYSTEMTRAYITEM_H
#define SYSTEMTRAYITEM_H

#include <QMenu>
#include <QMessageBox>
#include <QSystemTrayIcon>

class SystemTrayItem : public QObject
{
    Q_OBJECT

public:
    explicit SystemTrayItem(QObject *a_parent = Q_NULLPTR);
    ~SystemTrayItem();

    void show();

    void showLocalIdentityMenu();

signals:
    void defaultLocalIdentity(const QString &a_identity);
    void deleteDefaultLocalIdentity();

public slots:
    void onLocalIdentityListUpdated(const QStringList &a_localIdentityList);
    void onDefaultLocalIdentitySelected(const QString &a_identity);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason a_reason);
    void showAboutDialog();
    void localIdentityTriggered(QAction *a_action);

protected:
    bool event(QEvent *e);

private:
    QSystemTrayIcon *m_systemTrayIcon;
    QMessageBox     *m_aboutDialog;
    QStringList      m_localIdentityList;
    QMenu           *m_localIdentityMenu;
    QString          m_defaultLocalIdentity;
};

#endif // SYSTEMTRAYITEM_H
