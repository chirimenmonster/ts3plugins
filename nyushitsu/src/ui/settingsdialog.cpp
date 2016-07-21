
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

// checkbox の内容をデフォルト設定にする
void SettingsDialog::reset()
{
	config_t defaultConfig;

	config_getDefault(&defaultConfig);
	applyConfigToCheckBox(&defaultConfig);
}

// 現在の config 変数を UIに反映させる
void SettingsDialog::readConfig()
{
	applyConfigToCheckBox(&config);
}

// 指定の config 変数を UIに反映させる
void SettingsDialog::applyConfigToCheckBox(config_t *srcConfig)
{
	ui.checkBox_enableVoiceOnMove		->setChecked(srcConfig->enableVoiceOnMove);
	ui.checkBox_enableVoiceOnChat		->setChecked(srcConfig->enableVoiceOnChat);
	ui.checkBox_enableNicknameFilter	->setChecked(srcConfig->enableNicknameFilter);
	ui.checkBox_enableWatchOtherRoom	->setChecked(srcConfig->enableWatchOtherRoom);
}
