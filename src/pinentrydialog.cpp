/*!
 *  \brief     PinEntryDialog is the dialog the enables the user to enter a pin.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#include <QDebug>
#include <QDialogButtonBox>
#include <QTimer>
#include <QVBoxLayout>

#include "pinentrydialog.h"

PinEntryDialog::PinEntryDialog(QWidget *a_parent)
    : QDialog(a_parent)
    , m_timeLeft_ms(0)
{
    qDebug() << "> PinEntryDialog::PinEntryDialog()";
    setWindowTitle(tr("TLS Pool PIN request"));

    m_infoLabel = new QLabel(this);

    m_infoLabel->setText(tr("Please enter your PIN:"));

    m_pinLineEdit = new QLineEdit(this);
    m_pinLineEdit->setEchoMode(QLineEdit::Password);

    m_tokenManufacturerLabel = new QLabel(this);
    m_tokenModelLabel = new QLabel(this);
    m_tokenSerialNumberLabel = new QLabel(this);
    m_tokenLabel = new QLabel(this);
    m_attemptLabel = new QLabel(this);
    m_timeLeftLabel = new QLabel(this);
    QLabel *statusLabel = new QLabel(this);

    statusLabel->setText(tr("Always check the balloon on my Systray icon"));

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &PinEntryDialog::onAccepted);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    QVBoxLayout *verticalLayout = new QVBoxLayout;

    verticalLayout->addWidget(m_infoLabel);
    verticalLayout->addWidget(m_pinLineEdit);
    verticalLayout->addWidget(m_tokenManufacturerLabel);
    verticalLayout->addWidget(m_tokenModelLabel);
    verticalLayout->addWidget(m_tokenSerialNumberLabel);
    verticalLayout->addWidget(m_tokenLabel);
    verticalLayout->addWidget(m_attemptLabel);
    verticalLayout->addWidget(m_timeLeftLabel);
    verticalLayout->addWidget(statusLabel);
    verticalLayout->addWidget(buttonBox);
    setLayout(verticalLayout);
    qDebug() << "< PinEntryDialog::PinEntryDialog()";
}

void PinEntryDialog::setPrompt(const QString &a_prompt)
{
    qDebug() << "> PinEntryDialog::setPrompt(" << a_prompt << ")";
    m_infoLabel->setText(a_prompt);
    qDebug() << "< PinEntryDialog::setPrompt()";
}

void PinEntryDialog::setMaxPinLength(ulong a_maxPinLength)
{
    qDebug() << "> PinEntryDialog::setMaxPinLength(" << a_maxPinLength << ")";
    m_pinLineEdit->setMaxLength(a_maxPinLength);
    qDebug() << "< PinEntryDialog::setMaxPinLength()";
}

void PinEntryDialog::setNrOfAttemptFailures(quint32 a_nrOfAttemptFailures)
{
    qDebug() << "> PinEntryDialog::setNrOfAttemptFailures(" << a_nrOfAttemptFailures << ")";
    QString nrOfAttemptFailuresText = tr("Number of attempts already done: %1");
    m_attemptLabel->setText(nrOfAttemptFailuresText.arg(a_nrOfAttemptFailures));
    qDebug() << "< PinEntryDialog::setNrOfAttemptFailures()";
}

void PinEntryDialog::setTimeout(quint32 a_timeout_us)
{
    qDebug() << "> PinEntryDialog::setTimeout(" << a_timeout_us << ")";
    m_timeLeft_ms = a_timeout_us / 1000;

    if (m_timeLeft_ms > 0) {
        QTimer *timer = new QTimer(this);

        connect(timer, SIGNAL(timeout()), this, SLOT(updateTimeLeft()));
        timer->start(1000);
    }
    QString timeLeftText = tr("Time left: %1 seconds");
    m_timeLeftLabel->setText(timeLeftText.arg(a_timeout_us / (1000 * 1000)));
    qDebug() << "< PinEntryDialog::setTimeout()";
}

void PinEntryDialog::updateTimeLeft()
{
    qDebug() << "> PinEntryDialog::updateTimeLeft()";
    m_timeLeft_ms -= 1000;

    if (m_timeLeft_ms > 0) {
        QString timeLeftText = tr("Time left: %1 seconds");
        m_timeLeftLabel->setText(timeLeftText.arg(m_timeLeft_ms / 1000));
    } else {
        emit reject();
    }
    qDebug() << "< PinEntryDialog::updateTimeLeft()";
}

void PinEntryDialog::setTokenManufacturer(const QString &a_setTokenManufacturer)
{
    qDebug() << "> PinEntryDialog::setTokenManufacturer(" << a_setTokenManufacturer << ")";
    m_tokenManufacturerLabel->setText(a_setTokenManufacturer);
    if (a_setTokenManufacturer.isEmpty()) {
        m_tokenManufacturerLabel->hide();
    } else {
        m_tokenManufacturerLabel->show();
    }
    qDebug() << "< PinEntryDialog::setTokenManufacturer()";
}

void PinEntryDialog::setTokenModel(const QString &a_setTokenModel)
{
    qDebug() << "> PinEntryDialog::setTokenModel(" << a_setTokenModel << ")";
    m_tokenModelLabel->setText(a_setTokenModel);
    if (a_setTokenModel.isEmpty()) {
        m_tokenModelLabel->hide();
    } else {
        m_tokenModelLabel->show();
    }
    qDebug() << "< PinEntryDialog::setTokenModel()";
}

void PinEntryDialog::setTokenSerialNumber(const QString &a_setTokenSerialNumber)
{
    qDebug() << "> PinEntryDialog::setTokenSerialNumber(" << a_setTokenSerialNumber << ")";
    m_tokenSerialNumberLabel->setText(a_setTokenSerialNumber);
    if (a_setTokenSerialNumber.isEmpty()) {
        m_tokenSerialNumberLabel->hide();
    } else {
        m_tokenSerialNumberLabel->show();
    }
    qDebug() << "< PinEntryDialog::setTokenSerialNumber()";
}

void PinEntryDialog::setTokenLabel(const QString &a_setTokenLabel)
{
    qDebug() << "> PinEntryDialog::setTokenLabel(" << a_setTokenLabel << ")";
    m_tokenLabel->setText(a_setTokenLabel);
    if (a_setTokenLabel.isEmpty()) {
        m_tokenLabel->hide();
    } else {
        m_tokenLabel->show();
    }
    qDebug() << "< PinEntryDialog::setTokenLabel()";
}

PinEntryDialog::~PinEntryDialog()
{
    qDebug() << "> PinEntryDialog::~PinEntryDialog()";
    qDebug() << "> PinEntryDialog::~PinEntryDialog()";
}

void PinEntryDialog::onAccepted()
{
    qDebug() << "> PinEntryDialog::onAccepted()";
    emit pinEntered(m_pinLineEdit->text());
    accept();
    qDebug() << "> PinEntryDialog::onAccepted()";
}
