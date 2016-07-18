
#include <QWidget>

#include "ui/settingsdialog.h"
#include "uiadapter.h"

static SettingsDialog *dialog = NULL;

static QWidget* getMainWindowWidget() {
	foreach(QWidget *widget, QApplication::topLevelWidgets()) {
		if (widget->isWindow() && widget->inherits("QMainWindow") && !widget->windowTitle().isEmpty()) {
			return widget;
		}
	}
	return NULL;
}


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


void adapter_configure(void* qParentWidget) {
	// dialog = new SettingsDialog((QWidget *)qParentWidget);
}

void adapter_show() {
	char msg[1024];

	dialog = new SettingsDialog(getMainWindowWidget());
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	dialog->show();
}


#ifdef __cplusplus
}
#endif /* __cplusplus */
