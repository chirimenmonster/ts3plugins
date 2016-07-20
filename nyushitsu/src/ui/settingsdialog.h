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
	void reset();

private:
	Ui::SettingsDialogClass ui;
	void readConfig();
};

#endif // SETTINGSDIALOG_H
