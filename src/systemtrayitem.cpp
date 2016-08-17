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

#include "tlspoolgenerated.h"
#include "systemtrayitem.h"

static const int LOCAL_IDENTITY_MENU_TITLE = -1;
static const int LOCAL_IDENTITY_MENU_RESET = -2;

SystemTrayItem::SystemTrayItem(QObject *a_parent)
    : QObject(a_parent)
    , m_aboutDialog(nullptr)
    , m_localIdentityMenu(nullptr)
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
    delete m_localIdentityMenu;
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
    qDebug() << "> SystemTrayItem::iconActivated(" << a_reason << ")";
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
    QAction *localIdentityAction;

    m_localIdentityMenu = new QMenu();
    connect(m_localIdentityMenu, &QMenu::triggered, this, &SystemTrayItem::localIdentityTriggered);
    QAction *titleAction = new QAction(tr("Current Identity"), m_localIdentityMenu);
    titleAction->setData(QVariant(LOCAL_IDENTITY_MENU_TITLE));
    titleAction->setEnabled(false);
    m_localIdentityMenu->addAction(titleAction);
    m_localIdentityMenu->addSeparator();

    qDebug() << "m_defaultLocalIdentity:" << m_defaultLocalIdentity;
    for (int i = 0; i < m_localIdentityList.size(); ++i) {
        localIdentityAction = new QAction(m_localIdentityList.at(i), m_localIdentityMenu);
        localIdentityAction->setData(i);
        qDebug() << i << m_localIdentityList.at(i);
        if (m_defaultLocalIdentity == m_localIdentityList.at(i)) {
            localIdentityAction->setCheckable(true);
            localIdentityAction->setChecked(true);
            qDebug() << i << "setChecked(true)";
        }
        m_localIdentityMenu->addAction(localIdentityAction);
    }

    m_localIdentityMenu->addSeparator();

    QAction *resetAction = new QAction(tr("No default identity"), m_localIdentityMenu);
    resetAction->setData(QVariant(LOCAL_IDENTITY_MENU_RESET));
    m_localIdentityMenu->addAction(resetAction);

    // \todo in qdbustrayicon_p.h the geometry function is implemented as:
    //       QRect geometry() const Q_DECL_OVERRIDE { return QRect(); }
    //       so it never contains a correct position :-(
    //       Need to find another way to find the icon position so the local identity menu can be
    //       shown at the correct location
    //    m_localIdentityMenu->exec(m_systemTrayIcon->geometry().center());
    qDebug() << m_systemTrayIcon->geometry();
    m_localIdentityMenu->exec(QPoint(1000, 600));
    qDebug() << "< SystemTrayItem::showLocalIdentityMenu()";
}

void SystemTrayItem::showAboutDialog()
{
    qDebug() << "> SystemTrayItem::aboutDialog()";
    if (!m_aboutDialog) {
        m_aboutDialog = new QMessageBox;
        m_aboutDialog->setText("<center><b>TLS Pool GUI</b></center>");
        m_aboutDialog->setInformativeText("<center>version " + g_tlsPoolGuiVersion +
                                          "<br><br>Copyright 2016<br>Aschwin Marsman<br>"
                                          "the ARPA2 project</center>");
        m_aboutDialog->setDetailedText("SPDX-License-Identifier: GPL-2.0");
        // the about dialog shouldn't prevent using another dialog that pops up
        m_aboutDialog->setModal(false);
    }
    m_aboutDialog->show();
    qDebug() << "< SystemTrayItem::aboutDialog()";
}

void SystemTrayItem::onLocalIdentityListUpdated(const QStringList &a_localIdentityList)
{
    qDebug() << "> SystemTrayItem::onLocalIdentityListUpdated(" << a_localIdentityList << ")";
    m_localIdentityList = a_localIdentityList;
    qDebug() << "< SystemTrayItem::onLocalIdentityListUpdated()";
}

void SystemTrayItem::localIdentityTriggered(QAction *a_action)
{
    qDebug() << "> SystemTrayItem::localIdentityTriggered(" << a_action->text() << ")";
    switch (a_action->data().toInt()) {
    case LOCAL_IDENTITY_MENU_TITLE:
        break;
    case LOCAL_IDENTITY_MENU_RESET:
        emit deleteDefaultLocalIdentity();
        break;
    default:
        emit defaultLocalIdentity(m_localIdentityList.at(a_action->data().toInt()));
        break;
    }

    qDebug() << "< SystemTrayItem::localIdentityTriggered()";
}

void SystemTrayItem::onDefaultLocalIdentitySelected(const QString &a_identity)
{
    qDebug() << "> SystemTrayItem::onDefaultLocalIdentitySelected(" << a_identity << ")";
    m_defaultLocalIdentity = a_identity;
    qDebug() << "< SystemTrayItem::onDefaultLocalIdentitySelected()";
}

bool SystemTrayItem::event(QEvent *a_event)
{
    qDebug() << "> SystemTrayItem::event(" << a_event << ")";
    return QObject::event(a_event);
    qDebug() << "< SystemTrayItem::event()";
}
