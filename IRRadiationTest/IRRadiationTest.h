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
	// 在status bar中显示消息
	void ShowInStatusBar(QString text);

	/// 信号——槽
	void GUIConnect();

	// 初始化UI
	void InitUi();

private slots:
	// 关闭标签页
	void CloseTab(int);

	// 新建红外辐射亮度分析标签页
	void NewRadTab();

	// 新建普朗克定律模拟标签页
	void NewPlankTab();

	// 清空表格数据
	void ClearTable();

	/// 导出文本数据
	void ExportRadData();

	/// 导出图片
	void ExportRadChart();
	
	void ImportSunData();


private:
	Ui::IRRadiationTestClass ui;

	const QString RAD_FORM_CLASS_NAME = u8"RadiationForm";
	const QString PLANK_FORM_CLASS_NAME = u8"PlankForm";
};
