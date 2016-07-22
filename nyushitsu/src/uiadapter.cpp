
#include <QWidget>

#include "ui/settingsdialog.h"
#include "uiadapter.h"

#define DIALOG_TITLE	"Nyushitsu Settings"

static SettingsDialog *dialog = NULL;

static QWidget* getMainWindowWidget()
{
	foreach(QWidget *widget, QApplication::topLevelWidgets()) {
		if (widget->isWindow() && widget->inherits("QMainWindow") && !widget->windowTitle().isEmpty()) {
			return widget;
		}
	}
	return NULL;
}

static void dialog_init()
{
	if (!dialog) {
		dialog = new SettingsDialog(getMainWindowWidget());
		dialog->setWindowTitle(DIALOG_TITLE);
	}
}

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void adapter_configure(void* qParentWidget)
{
	// dialog = new SettingsDialog((QWidget *)qParentWidget);
}

void adapter_show()
{
	dialog_init();
	//dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}

void adapter_delete()
{
	delete dialog;
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
