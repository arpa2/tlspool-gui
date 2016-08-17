/*!
 *  \brief     SelectLocalIdentityDialog is the dialog the enables the user to select any identity.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <QDebug>
#include <QDialogButtonBox>
#include <QLabel>
#include <QVBoxLayout>

#include "selectlocalidentitydialog.h"

SelectLocalIdentityDialog::SelectLocalIdentityDialog(QWidget *a_parent)
    : QDialog(a_parent)
{
    qDebug() << "> SelectLocalIdentityDialog::SelectLocalIdentityDialog()";
    setWindowTitle(tr("Select local identity disclosure"));
    QLabel *remotePeerLabel = new QLabel;

    remotePeerLabel->setText(tr("Remote peer to disclose to:"));

    m_remotePeerComboBox = new QComboBox;

    m_remotePeerComboBox->setEditable(false);

    QLabel *localIdentityLabel = new QLabel;

    localIdentityLabel->setText(tr("Local identity to reveal:"));

    m_localIdentityComboBox = new QComboBox;

    m_localIdentityComboBox->setEditable(true);

    m_rememberChoiceCheckBox = new QCheckBox(tr("&Remember my choice"));;

    QVBoxLayout *verticalLayout = new QVBoxLayout;

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectLocalIdentityDialog::onAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    verticalLayout->addWidget(remotePeerLabel);
    verticalLayout->addWidget(m_remotePeerComboBox);
    verticalLayout->addWidget(localIdentityLabel);
    verticalLayout->addWidget(m_localIdentityComboBox);
    verticalLayout->addWidget(m_rememberChoiceCheckBox);
    verticalLayout->addWidget(buttonBox);
    setLayout(verticalLayout);
    qDebug() << "< SelectLocalIdentityDialog::SelectLocalIdentityDialog()";
}

SelectLocalIdentityDialog::~SelectLocalIdentityDialog()
{
    qDebug() << "> SelectLocalIdentityDialog::~SelectLocalIdentityDialog()";
    qDebug() << "< SelectLocalIdentityDialog::~SelectLocalIdentityDialog()";
}

void SelectLocalIdentityDialog::setLocalIdentityList(const QStringList &a_localIdentityList)
{
    qDebug() << "> SelectLocalIdentityDialog::setLocalIdentityList()";

    m_localIdentityComboBox->clear();
    m_localIdentityComboBox->addItems(a_localIdentityList);

    qDebug() << "< SelectLocalIdentityDialog::setLocalIdentityList()";
}

void SelectLocalIdentityDialog::setRemoteIdentity(const QString &a_remoteId)
{
    qDebug() << "> SelectLocalIdentityDialog::setRemoteIdentity()";

    m_remotePeerComboBox->clear();
    m_remotePeerComboBox->addItem(a_remoteId);

    qDebug() << "< SelectLocalIdentityDialog::setRemoteIdentity()";
}

void SelectLocalIdentityDialog::onAccepted()
{
    qDebug() << "> SelectLocalIdentityDialog::onAccepted()";
    emit localIdentitySelected(m_localIdentityComboBox->currentText(),
                               m_rememberChoiceCheckBox->isChecked());
    accept();
    qDebug() << "> SelectLocalIdentityDialog::~onAccepted()";
}

void SelectLocalIdentityDialog::onRejected()
{
    qDebug() << "> SelectLocalIdentityDialog::onRejected()";
    emit localIdentityRejected();
    accept();
    qDebug() << "> SelectLocalIdentityDialog::~onRejected()";
}

