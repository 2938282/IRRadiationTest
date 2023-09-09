#pragma once

#include "ui_IRRadiationTest.h"
#include <QtWidgets/QMainWindow>
#include "RadiationForm.h"
#include "PlankForm.h"

#include <qinputdialog.h>


class IRRadiationTest : public QMainWindow
{
	Q_OBJECT

public:
	IRRadiationTest(QWidget* parent = Q_NULLPTR);
	~IRRadiationTest();
public:
	// ��status bar����ʾ��Ϣ
	void ShowInStatusBar(QString text);

	/// �źš�����
	void GUIConnect();

	// ��ʼ��UI
	void InitUi();

private slots:
	// �رձ�ǩҳ
	void CloseTab(int);

	// �½�����������ȷ�����ǩҳ
	void NewRadTab();

	// �½����ʿ˶���ģ���ǩҳ
	void NewPlankTab();

	// ��ձ������
	void ClearTable();

	/// �����ı�����
	void ExportRadData();

	/// ����ͼƬ
	void ExportRadChart();
	
	void ImportSunData();


private:
	Ui::IRRadiationTestClass ui;

	const QString RAD_FORM_CLASS_NAME = u8"RadiationForm";
	const QString PLANK_FORM_CLASS_NAME = u8"PlankForm";
};
