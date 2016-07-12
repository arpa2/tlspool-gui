/*!
 *  \brief     SystemTrayItem is the item that shows an icon to interact with the TLS Pool GUI.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef SYSTEMTRAYITEM_H
#define SYSTEMTRAYITEM_H

#include <QSystemTrayIcon>
#include <QObject>

class SystemTrayItem : public QObject
{
    Q_OBJECT

public:
    explicit SystemTrayItem(QObject *a_parent = Q_NULLPTR);
    ~SystemTrayItem();

    void show();

    void showLocalIdentityMenu();

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason a_reason);

private:
    QSystemTrayIcon *m_systemTrayIcon;
};

#endif // SYSTEMTRAYITEM_H
