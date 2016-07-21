/*!
 *  \brief     Main entry point of the TLS Pool GUI.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <QApplication>
#include <QDebug>

#include <tlspool/starttls.h>
#include "tlspoolgenerated.h"
#include "tlspoolinterface.h"

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    QApplication::setQuitOnLastWindowClosed(false);

    qSetMessagePattern("[%{time yyyyMMdd h:mm:ss.zzz t} %{if-debug}D%{endif}%{if-info}I%{endif}"
                       "%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] "
                       "%{file}:%{line} %{threadid} - %{message}");

    QCoreApplication::setOrganizationName("ARPA2 Project");
    QCoreApplication::setOrganizationDomain("arpa2.net");
    QCoreApplication::setApplicationName("TLS Pool GUI");

    qDebug() << "TLS Pool GUI version:" << g_tlsPoolGuiVersion;

    TlsPoolInterface tlsPoolInterface;

    tlsPoolInterface.showSystemTray();

    return application.exec();
}
