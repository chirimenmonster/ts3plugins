#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtWidgets/QMainWindow>
#include "ui_settingsdialog.h"
#include "../storage.h"

class SettingsDialog : public QMainWindow
{
	Q_OBJECT

public:
	SettingsDialog(QWidget *parent = 0);
	~SettingsDialog();

public slots:
	void show();

private slots:
	void accept();
	void reset();

private:
	Ui::SettingsDialogClass ui;
	void readConfig(void);
	void applyConfigToCheckBox(config_t *srcConfig);
};

#endif // SETTINGSDIALOG_H
