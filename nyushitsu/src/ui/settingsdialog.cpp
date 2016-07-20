
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
	readConfig();
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::accept()
{
	config.enableVoiceOnMove		= ui.checkBox_enableVoiceOnMove		->isChecked();
	config.enableVoiceOnChat		= ui.checkBox_enableVoiceOnChat		->isChecked();
	config.enableNicknameFilter		= ui.checkBox_enableNicknameFilter	->isChecked();
	config.enableWatchOtherRoom		= ui.checkBox_enableWatchOtherRoom	->isChecked();
}

void SettingsDialog::reset()
{
	config_default();
	readConfig();
}

// config •Ï”‚ð UI‚É”½‰f‚³‚¹‚é
void SettingsDialog::readConfig()
{
	ui.checkBox_enableVoiceOnMove		->setChecked(config.enableVoiceOnMove);
	ui.checkBox_enableVoiceOnChat		->setChecked(config.enableVoiceOnChat);
	ui.checkBox_enableNicknameFilter	->setChecked(config.enableNicknameFilter);
	ui.checkBox_enableWatchOtherRoom	->setChecked(config.enableWatchOtherRoom);
}