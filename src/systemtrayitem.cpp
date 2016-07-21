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

#include "tlspoolgenerated.h"
#include "systemtrayitem.h"

SystemTrayItem::SystemTrayItem(QObject *a_parent)
    : QObject(a_parent)
    , m_aboutDialog(nullptr)
{
    qDebug() << "> SystemTrayItem::SystemTrayItem()";
    Q_INIT_RESOURCE(resources);

    QMenu *systemTrayIconMenu;
    QAction *aboutAction;
    QAction *quitAction;

    m_systemTrayIcon = new QSystemTrayIcon(a_parent);
    systemTrayIconMenu = new QMenu();

    aboutAction = new QAction(tr("&About"), a_parent);
    connect(aboutAction, &QAction::triggered, this, &SystemTrayItem::showAboutDialog);

    quitAction = new QAction(tr("&Quit"), a_parent);
    connect(quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

    systemTrayIconMenu->addSeparator();
    systemTrayIconMenu->addAction(aboutAction);
    systemTrayIconMenu->addAction(quitAction);

    m_systemTrayIcon->setContextMenu(systemTrayIconMenu);
    // \todo use svg icon
    m_systemTrayIcon->setIcon(QIcon(":/icon.png"));

    connect(m_systemTrayIcon, &QSystemTrayIcon::activated, this, &SystemTrayItem::iconActivated);
    qDebug() << "< SystemTrayItem::SystemTrayItem()";
}

SystemTrayItem::~SystemTrayItem()
{
    qDebug() << "> SystemTrayItem::~SystemTrayItem()";
    delete m_aboutDialog;
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

void SystemTrayItem::showAboutDialog()
{
    qDebug() << "> SystemTrayItem::aboutDialog()";
    if (!m_aboutDialog)
    {
        m_aboutDialog = new QMessageBox;
        m_aboutDialog->setText("<center><b>TLS Pool GUI</b></center>");
        m_aboutDialog->setInformativeText("<center>version " + g_tlsPoolGuiVersion +
                         "<br><br>Copyright 2016<br>Aschwin Marsman<br>the ARPA2 project</center>");
        m_aboutDialog->setDetailedText("SPDX-License-Identifier: GPL-2.0");
        // the about dialog shouldn't prevent using another dialog that pops up
        m_aboutDialog->setModal(false);
    }
    m_aboutDialog->show();
    qDebug() << "< SystemTrayItem::aboutDialog()";
}
