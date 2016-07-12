/*!
 *  \brief     SystemTrayItem is the item that shows an icon to interact with the TLS Pool GUI.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <QDebug>
#include <QAction>
#include <QCoreApplication>
#include <QMenu>

#include "systemtrayitem.h"

SystemTrayItem::SystemTrayItem(QObject *a_parent)
    : QObject(a_parent)
{
    qDebug() << "> SystemTrayItem::SystemTrayItem()";
    Q_INIT_RESOURCE(resources);

    QMenu *systemTrayIconMenu;
//    QAction *minimizeAction;
//    QAction *maximizeAction;
//    QAction *restoreAction;
    QAction *quitAction;

    m_systemTrayIcon = new QSystemTrayIcon(a_parent);
    systemTrayIconMenu = new QMenu();
//    minimizeAction = new QAction(tr("Mi&nimize"), a_parent);
//    connect(minimizeAction, &QAction::triggered, a_parent, &QWidget::hide);

//    maximizeAction = new QAction(tr("Ma&ximize"), a_widget);
//    connect(maximizeAction, &QAction::triggered, a_widget, &QWidget::showMaximized);

//    restoreAction = new QAction(tr("&Restore"), a_widget);
//    connect(restoreAction, &QAction::triggered, a_widget, &QWidget::showNormal);

    quitAction = new QAction(tr("&Quit"), a_parent);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

//    systemTrayIconMenu->addAction(minimizeAction);
//    systemTrayIconMenu->addAction(maximizeAction);
//    systemTrayIconMenu->addAction(restoreAction);
    systemTrayIconMenu->addSeparator();
    systemTrayIconMenu->addAction(quitAction);

    m_systemTrayIcon->setContextMenu(systemTrayIconMenu);
    // \todo use svg icon
    m_systemTrayIcon->setIcon(QIcon(":/icon.png"));
//    m_systemTrayIcon->setIcon(QIcon(":/icon.svg"));

    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &SystemTrayItem::iconActivated);
    qDebug() << "< SystemTrayItem::SystemTrayItem()";
}

SystemTrayItem::~SystemTrayItem()
{
    qDebug() << "> SystemTrayItem::~SystemTrayItem()";
    qDebug() << "< SystemTrayItem::~SystemTrayItem()";
}

void SystemTrayItem::show()
{
    qDebug() << "> SystemTrayItem::show()";
    m_systemTrayIcon->show();
    qDebug() << "< SystemTrayItem::show()";
}

void SystemTrayItem::iconActivated(QSystemTrayIcon::ActivationReason a_reason)
{
    qDebug() << "> SystemTrayItem::iconActivated()";
    switch (a_reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        showLocalIdentityMenu();
        break;
    case QSystemTrayIcon::Unknown:
    case QSystemTrayIcon::Context:
        break;
    }
    qDebug() << "< SystemTrayItem::iconActivated()";
}

void SystemTrayItem::showLocalIdentityMenu()
{
    qDebug() << "> SystemTrayItem::showLocalIdentityMenu()";
    qDebug() << "< SystemTrayItem::showLocalIdentityMenu()";
}
