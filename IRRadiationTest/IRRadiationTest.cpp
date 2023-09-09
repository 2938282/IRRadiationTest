#include "IRRadiationTest.h"

IRRadiationTest::IRRadiationTest(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	// �����źŲ�
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
	// enable�رձ�ǩҳ��ť
	ui.tabWidget->setTabsClosable(true);

	// enable�϶���ǩҳ
	ui.tabWidget->setMovable(true);

	// ���ӹرհ�ť��removeTab()
	connect(ui.tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseTab(int)));
	connect(ui.exportRadData, SIGNAL(triggered()), this, SLOT(ExportRadData()));
	connect(ui.exportRadChart, SIGNAL(triggered()), this, SLOT(ExportRadChart()));
	connect(ui.clearTable, SIGNAL(triggered()), this, SLOT(ClearTable()));
	connect(ui.newRadTab, SIGNAL(triggered()), this, SLOT(NewRadTab()));
	connect(ui.newPlankTab, SIGNAL(triggered()), this, SLOT(NewPlankTab()));
	connect(ui.importSunData, SIGNAL(triggered()), this, SLOT(ImportSunData()));
}

// ����̫����������
void IRRadiationTest::ImportSunData()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// �жϵ�ǰtab���ͣ�������Ӧ��ʵ������
	if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			((PlankForm*)curTab)->ImportSunData();
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
			goto ClearTableErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"����", u8"��ǰtab������δ����ClearTable()��ʶ��", QMessageBox::Ok);
		goto ClearTableErr;
	}

	return;

ClearTableErr:
	ui.statusBar->showMessage(u8"������������ֹ�������");
	return;
}

// ��ʼ��UI
void IRRadiationTest::InitUi()
{

}

// �رձ�ǩҳ
void IRRadiationTest::CloseTab(int index)
{
	ui.tabWidget->removeTab(index);
}

// �½�����������ȷ�����ǩҳ
void IRRadiationTest::NewRadTab()
{
	int idx = ui.tabWidget->count();
	bool ok = true;
	QString tabName = QInputDialog::getText(
		this, u8"�½���ǩҳ",
		u8"�����±�ǩҳ����",
		QLineEdit::Normal,
		u8"����������ȷ���" + QString::number(ui.tabWidget->count()),
		&ok);

	if (ok && tabName != u8"") {
		ui.tabWidget->addTab(new RadiationForm, tabName);
	}
	else {
		QMessageBox::critical(NULL, u8"����", u8"����������Ч��ȡ���½�����", QMessageBox::Ok);
		ui.statusBar->showMessage(u8"����ֹ�½�����");
	}

	// ת���½��ı�ǩҳ
	ui.tabWidget->setCurrentIndex(idx);

	return;
}

// �½����ʿ˶���ģ���ǩҳ
void IRRadiationTest::NewPlankTab()
{

	int idx = ui.tabWidget->count();
	bool ok = true;
	QString tabName = QInputDialog::getText(
		this, u8"�½���ǩҳ",
		u8"�����±�ǩҳ����",
		QLineEdit::Normal,
		u8"���ʿ˶���ģ��" + QString::number(ui.tabWidget->count()),
		&ok);

	if (ok && tabName != u8"") {
		ui.tabWidget->addTab(new PlankForm, tabName);
	}
	else {
		QMessageBox::critical(NULL, u8"����", u8"����������Ч��ȡ���½�����", QMessageBox::Ok);
		ui.statusBar->showMessage(u8"����ֹ�½�����");
	}

	// ת���½��ı�ǩҳ
	ui.tabWidget->setCurrentIndex(idx);

	return;
}

// ��ձ������
void IRRadiationTest::ClearTable()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// �жϵ�ǰtab���ͣ�������Ӧ��ʵ������
	if (curTabName == RAD_FORM_CLASS_NAME)
	{
		try
		{
			((RadiationForm*)curTab)->ClearTabTable();
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
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
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
			goto ClearTableErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"����", u8"��ǰtab������δ����ClearTable()��ʶ��", QMessageBox::Ok);
		goto ClearTableErr;
	}

	return;

ClearTableErr:
	ui.statusBar->showMessage(u8"������������ֹ��ղ���");
	return;
}

// ������ǰ���ݵ��ļ���
void IRRadiationTest::ExportRadData()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// �жϵ�ǰtab���ͣ�������Ӧ��ʵ������
	if (curTabName == RAD_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((RadiationForm*)curTab)->ExportTabRadData();
			ui.statusBar->showMessage(u8"�ѽ����������" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}

	}
	else if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((PlankForm*)curTab)->ExportTabRadData();
			ui.statusBar->showMessage(u8"�ѽ����������" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"����", u8"��ǰtab������δ����ClearTable()��ʶ��", QMessageBox::Ok);
		goto ExportRadDataErr;
	}

	return;

ExportRadDataErr:
	ui.statusBar->showMessage(u8"������������ֹ��������");
	return;
}

// ������ǰ����ΪͼƬ
void IRRadiationTest::ExportRadChart()
{
	QString curTabName = ui.tabWidget->currentWidget()->metaObject()->className();

	QWidget* curTab = ui.tabWidget->currentWidget();

	// �жϵ�ǰtab���ͣ�������Ӧ��ʵ������
	if (curTabName == RAD_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((RadiationForm*)curTab)->ExportTabRadChart();
			ui.statusBar->showMessage(u8"�ѽ�ͼ�񵼳���" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}

	}
	else if (curTabName == PLANK_FORM_CLASS_NAME)
	{
		try
		{
			std::string fileName = ((PlankForm*)curTab)->ExportTabRadChart();
			ui.statusBar->showMessage(u8"�ѽ�ͼ�񵼳���" + QString::fromStdString(fileName));
		}
		catch (const std::exception& e)
		{
			QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
			goto ExportRadDataErr;
		}
	}
	else
	{
		QMessageBox::critical(NULL, u8"����", u8"��ǰtab������δ����ClearTable()��ʶ��", QMessageBox::Ok);
		goto ExportRadDataErr;
	}

	return;

ExportRadDataErr:
	ui.statusBar->showMessage(u8"������������ֹ��������");
	return;
}
