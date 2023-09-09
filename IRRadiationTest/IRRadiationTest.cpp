#include "IRRadiationTest.h"

IRRadiationTest::IRRadiationTest(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// 连接信号槽
	GUIConnect();
	InitUi();
}


IRRadiationTest::~IRRadiationTest()
{
}

void IRRadiationTest::ShowInStatusBar(QString text)
{
	ui.statusBar->showMessage(text);
}


void IRRadiationTest::GUIConnect()
{
	// enable关闭标签页按钮
	ui.tabWidget->setTabsClosable(true);

	// enable拖动标签页
	ui.tabWidget->setMovable(true);

	// 连接关闭按钮到removeTab()
	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseTab(int)));
	connect(ui.exportRadData, SIGNAL(triggered()), this, SLOT(ExportRadData()));
	connect(ui.exportRadChart, SIGNAL(triggered()), this, SLOT(ExportRadChart()));
	connect(ui.clearTable, SIGNAL(triggered()), this, SLOT(ClearTable()));
	connect(ui.newRadTab, SIGNAL(triggered()), this, SLOT(NewRadTab()));
	connect(ui.newPlankTab, SIGNAL(triggered()), this, SLOT(NewPlankTab()));
	connect(ui.importSunData, SIGNAL(triggered()), this, SLOT(ImportSunData()));
}

// 导入太阳特征数据
void IRRadiationTest::ImportSunData()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// 判断当前tab类型，调用相应的实例函数
	if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			((PlankForm*)curTab)->ImportSunData();
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ClearTableErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"错误", u8"当前tab的类型未能在ClearTable()中识别", QMessageBox::Ok);
		goto ClearTableErr;
	}

	return;

ClearTableErr:
	ui.statusBar->showMessage(u8"发生错误，已终止导入操作");
	return;
}

// 初始化UI
void IRRadiationTest::InitUi()
{

}

// 关闭标签页
void IRRadiationTest::CloseTab(int index)
{
	ui.tabWidget->removeTab(index);
}

// 新建红外辐射亮度分析标签页
void IRRadiationTest::NewRadTab()
{
	int idx = ui.tabWidget->count();
	bool ok = true;
	QString tabName = QInputDialog::getText(
		this, u8"新建标签页",
		u8"输入新标签页名称",
		QLineEdit::Normal,
		u8"红外辐射亮度分析" + QString::number(ui.tabWidget->count()),
		&ok);

	if (ok && tabName != u8"") {
		ui.tabWidget->addTab(new RadiationForm, tabName);
	}
	else {
		QMessageBox::critical(NULL, u8"错误", u8"输入名称无效或取消新建操作", QMessageBox::Ok);
		ui.statusBar->showMessage(u8"已终止新建操作");
	}

	// 转到新建的标签页
	ui.tabWidget->setCurrentIndex(idx);

	return;
}

// 新建普朗克定律模拟标签页
void IRRadiationTest::NewPlankTab()
{

	int idx = ui.tabWidget->count();
	bool ok = true;
	QString tabName = QInputDialog::getText(
		this, u8"新建标签页",
		u8"输入新标签页名称",
		QLineEdit::Normal,
		u8"普朗克定律模拟" + QString::number(ui.tabWidget->count()),
		&ok);

	if (ok && tabName != u8"") {
		ui.tabWidget->addTab(new PlankForm, tabName);
	}
	else {
		QMessageBox::critical(NULL, u8"错误", u8"输入名称无效或取消新建操作", QMessageBox::Ok);
		ui.statusBar->showMessage(u8"已终止新建操作");
	}

	// 转到新建的标签页
	ui.tabWidget->setCurrentIndex(idx);

	return;
}

// 清空表格数据
void IRRadiationTest::ClearTable()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// 判断当前tab类型，调用相应的实例函数
	if (curTabName == RAD_FORM_CLASS_NAME)
	{
		try
		{
			((RadiationForm*)curTab)->ClearTabTable();
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ClearTableErr;
		}

	}
	else if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			((PlankForm*)curTab)->ClearTabTable();
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ClearTableErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"错误", u8"当前tab的类型未能在ClearTable()中识别", QMessageBox::Ok);
		goto ClearTableErr;
	}

	return;

ClearTableErr:
	ui.statusBar->showMessage(u8"发生错误，已终止清空操作");
	return;
}

// 导出当前数据到文件中
void IRRadiationTest::ExportRadData()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// 判断当前tab类型，调用相应的实例函数
	if (curTabName == RAD_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((RadiationForm*)curTab)->ExportTabRadData();
			ui.statusBar->showMessage(u8"已将结果导出到" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}

	}
	else if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((PlankForm*)curTab)->ExportTabRadData();
			ui.statusBar->showMessage(u8"已将结果导出到" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"错误", u8"当前tab的类型未能在ClearTable()中识别", QMessageBox::Ok);
		goto ExportRadDataErr;
	}

	return;

ExportRadDataErr:
	ui.statusBar->showMessage(u8"发生错误，已终止导出操作");
	return;
}

// 导出当前曲线为图片
void IRRadiationTest::ExportRadChart()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// 判断当前tab类型，调用相应的实例函数
	if (curTabName == RAD_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((RadiationForm*)curTab)->ExportTabRadChart();
			ui.statusBar->showMessage(u8"已将图像导出到" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}

	}
	else if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((PlankForm*)curTab)->ExportTabRadChart();
			ui.statusBar->showMessage(u8"已将图像导出到" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"错误", u8"当前tab的类型未能在ClearTable()中识别", QMessageBox::Ok);
		goto ExportRadDataErr;
	}

	return;

ExportRadDataErr:
	ui.statusBar->showMessage(u8"发生错误，已终止导出操作");
	return;
}
