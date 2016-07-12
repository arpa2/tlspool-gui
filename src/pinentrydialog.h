/*!
 *  \brief     PinEntryDialog is the dialog the enables the user to enter a pin.
 *  \author    Aschwin Marsman
 *  \date      2016
 *  \copyright Copyright 2016, Aschwin Marsman, the ARPA2 project
 *  \license   SPDX-License-Identifier: GPL-2.0
 */

#ifndef PINENTRYDIALOG_H
#define PINENTRYDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class PinEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PinEntryDialog(QWidget *a_parent = Q_NULLPTR);
    ~PinEntryDialog();

    void setPrompt(const QString &a_prompt);
    void setMaxPinLength(ulong a_maxPinLength);
    void setNrOfAttemptFailures(quint32 a_nrOfAttemptFailures);
    void setTimeout(quint32 a_timeout_us);
    void setTokenManufacturer(const QString &a_setTokenManufacturer);
    void setTokenModel(const QString &a_setTokenModel);
    void setTokenSerialNumber(const QString &a_setTokenSerialNumber);
    void setTokenLabel(const QString &a_setTokenLabel);

signals:
    void pinEntered(const QString &a_pin);

protected slots:
    void onAccepted();
    void updateTimeLeft();

private:
    QLabel    *m_tokenManufacturerLabel;
    QLabel    *m_tokenModelLabel;
    QLabel    *m_tokenSerialNumberLabel;
    QLabel    *m_tokenLabel;
    QLabel    *m_attemptLabel;
    QLabel    *m_timeLeftLabel;
    QLabel    *m_infoLabel;
    QLineEdit *m_pinLineEdit;
    int        m_timeLeft_ms;
};

#endif // PINENTRYDIALOG_H
