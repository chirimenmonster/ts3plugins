#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets/QMainWindow>
#include "ui_settingsdialog.h"

class SettingsDialog : public QMainWindow
{
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

private slots:
	void accept();

private:
	Ui::SettingsDialogClass ui;
};

#endif // SETTINGSDIALOG_H
