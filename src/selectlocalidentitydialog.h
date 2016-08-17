/*!
 *  \brief     SelectLocalIdentityDialog is the dialog the enables the user to select any identity.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef SELECTLOCALIDENTITYDIALOG_H
#define SELECTLOCALIDENTITYDIALOG_H

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QStringList>

class SelectLocalIdentityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectLocalIdentityDialog(QWidget *a_parent = Q_NULLPTR);
    ~SelectLocalIdentityDialog();

    void setLocalIdentityList(const QStringList &a_localIdentityList);
    void setRemoteIdentity(const QString &a_remoteId);

signals:
    void localIdentitySelected(const QString &a_localIdentity, bool a_remember);
    void localIdentityRejected();

protected slots:
    void onAccepted();
    void onRejected();

private:
    QComboBox *m_remotePeerComboBox;
    QComboBox *m_localIdentityComboBox;
    QCheckBox *m_rememberChoiceCheckBox;

};

#endif // SELECTLOCALIDENTITYDIALOG_H
