/*!
 *  \brief     TlsPoolGuiSettings contains settings for the TLS Pool GUI.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef TLSPOOLGUISETTINGS_H
#define TLSPOOLGUISETTINGS_H

#include <QSettings>

class TlsPoolGuiSettings : public QObject
{
    Q_OBJECT

public:
    explicit TlsPoolGuiSettings(const QString &a_fileName, QObject *a_parent = nullptr);
    ~TlsPoolGuiSettings();

    QString socketName();

private:
    QSettings *m_settings;
};

#endif // TLSPOOLGUISETTINGS_H
