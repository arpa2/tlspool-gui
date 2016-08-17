/*!
 *  \brief     TlsPoolGuiSettings contains settings for the TLS Pool GUI.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <QDebug>

#include <tlspool/starttls.h>

#include "tlspoolguisettings.h"

TlsPoolGuiSettings::TlsPoolGuiSettings(const QString &a_fileName, QObject *a_parent)
    : QObject(a_parent)
{
    qDebug() << "> TlsPoolGuiSettings::TlsPoolGuiSettings()";
    m_settings = new QSettings(a_fileName, QSettings::IniFormat, a_parent);

    if (!m_settings->contains("tlspool/configFile")) {
        m_settings->setValue("tlspool/configFile", TLSPOOL_DEFAULT_CONFIG_PATH);
    }
    qDebug() << "  tlspool/configFile" << m_settings->value("tlspool/configFile");
    qDebug() << "< TlsPoolGuiSettings::TlsPoolGuiSettings()";
}

TlsPoolGuiSettings::~TlsPoolGuiSettings()
{
    qDebug() << "> TlsPoolGuiSettings::~TlsPoolGuiSettings()";
    qDebug() << "< TlsPoolGuiSettings::~TlsPoolGuiSettings()";
}

QString TlsPoolGuiSettings::socketName()
{
    qDebug() << "> TlsPoolGuiSettings::socketName()";

    // \todo discuss if tlspool_configvar can have const for the arguments
    char *socket_name = tlspool_configvar(
                            m_settings->value("tlspool/configFile").toString().toLatin1().data(),
                            const_cast<char *>("socket_name"));
    QString returnValue;

    if (socket_name) {
        returnValue = socket_name;
        free(socket_name);
    }
    qDebug() << "< TlsPoolGuiSettings::socketName()" << returnValue;
    return returnValue;
}
