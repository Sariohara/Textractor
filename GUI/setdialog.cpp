#include "setdialog.h"
#include "ui_setdialog.h"
#include "defs.h"
#include "text.h"
#include "host/host.h"

SetDialog::SetDialog(QWidget* parent) :
	QDialog(parent, Qt::WindowCloseButtonHint),
	ui(new Ui::SetDialog)
{
	ui->setupUi(this);
	for (auto[spinBox, value, label] : Array<std::tuple<QSpinBox*&, int, const char*>>{
		{ flushDelay, TextThread::flushDelay, FLUSH_DELAY },
		{ maxBufferSize, TextThread::maxBufferSize, MAX_BUFFER_SIZE },
		{ defaultCodepage, TextThread::defaultCodepage, DEFAULT_CODEPAGE }
	})
	{
		spinBox = new QSpinBox(this);
		spinBox->setMaximum(INT_MAX);
		spinBox->setValue(value);
		ui->layout->insertRow(0, label, spinBox);
	}

	connect(ui->buttonBox, &QDialogButtonBox::accepted, [this] { edited = true; });
}

SetDialog::~SetDialog()
{
	if (edited)
	{
		QSettings settings(CONFIG_FILE, QSettings::IniFormat);
		settings.setValue(FLUSH_DELAY, TextThread::flushDelay = flushDelay->value());
		settings.setValue(MAX_BUFFER_SIZE, TextThread::maxBufferSize = maxBufferSize->value());
		settings.setValue(DEFAULT_CODEPAGE, TextThread::defaultCodepage = defaultCodepage->value());
		settings.sync();
	}
	delete ui;
}