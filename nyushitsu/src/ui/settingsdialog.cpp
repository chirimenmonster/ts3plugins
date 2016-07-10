
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settingsdialog.h"
#include "ts3_functions.h"
#include "nyushitsu.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QMainWindow(parent)
{
	char msg[1024];

	ui.setupUi(this);
	ui.checkBox_1->setChecked(config.avoidOtherRoom);
	ui.checkBox_2->setChecked(config.filterNumber);

	snprintf(msg, sizeof(msg), "constructor: checked = %d", config.avoidOtherRoom);
	logMessage(msg);
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
	char msg[1024];

	config.avoidOtherRoom	= ui.checkBox_1->isChecked();
	config.filterNumber		= ui.checkBox_2->isChecked();

	snprintf(msg, sizeof(msg), "accept: checked = %d", config.avoidOtherRoom);
	logMessage(msg);
}
