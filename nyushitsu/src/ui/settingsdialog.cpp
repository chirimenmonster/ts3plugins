
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "settingsdialog.h"
#include "../nyushitsu.h"
#include "../storage.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.checkBox_1->setChecked(config.avoidOtherRoom);
	ui.checkBox_2->setChecked(config.filterStrip);

	logMessage("constructor: checked = %d", config.avoidOtherRoom);
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
	config.avoidOtherRoom	= ui.checkBox_1->isChecked();
	config.filterStrip		= ui.checkBox_2->isChecked();

	logMessage("accept: checked = %d", config.avoidOtherRoom);
}
