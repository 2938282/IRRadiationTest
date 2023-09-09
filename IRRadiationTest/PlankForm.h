#pragma once

#include <QWidget>
#include "ui_PlankForm.h"
#include <IRRadiation/IRRadiate.h>
#include <QStandardItemModel>
// qt messenge box
#include <qmessagebox.h>
// qt文件浏览器
#include <QFileDialog>
// c++文件操作
#include <iostream>
#include <fstream>

using namespace IRModel;

// 输入参数
class InputInfo
{
public:
	//曲线名称
	QString name;
	// 固定值是否为波长
	bool isWaveFixed;
	// 固定值
	double fixed;
	// 变化值
	double start;
	double end;
	double step;
};

class PlankForm : public QWidget
{
	Q_OBJECT

public:
	PlankForm(QWidget *parent = nullptr);
	~PlankForm();
	// 清空当前标签页表格
	void ClearTabTable();

	// 导入太阳特征数据
	void ImportSunData();

	// 导出当前标签页数据到文件
	// 返回保存的目标文件的完整路径
	std::string ExportTabRadData();

	// 导出当前标签页表格到图片
	// 返回保存的目标文件的完整路径
	std::string ExportTabRadChart();

private:
	void GUIConnect();
	float RadiationL(float Lambda, float T);
	void SetHeader();
	void Draw();
	
	Ui::PlankFormClass ui;
	//辐射计算参数
	CIRRadiateParam  m_Param;
	//辐射计算类实例
	CIRRadiate  m_IRRadiate;
	// 表model
	QStandardItemModel* model = new QStandardItemModel();
	// 曲线名称
	std::vector<InputInfo> inputInfo;
	
	// 结果
	std::map<QString, std::vector<std::pair<float, float>>> result;
	
private slots:
	// 辐射计算响应函数
	void RadCountSlots();

	// 添加数据
	void AddData();
	void SelectTemp();
	void SelectWave();
};
