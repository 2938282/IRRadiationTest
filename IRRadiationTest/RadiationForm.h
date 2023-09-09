#ifndef RADIATIONFORM_H
#define RADIATIONFORM_H

#include <QWidget>
#include "ui_RadiationForm.h"

// qt文件浏览器
#include <QFileDialog>
// qt messenge box
#include <qmessagebox.h>
// 表格相关
#include <QStandardItemModel>
// 绘图相关
#include "QChartWidget.h"

// c++文件操作
#include <iostream>
#include <fstream>

#include <QStatusBar>
#include <IRRadiation/IRRadiate.h>

using namespace IRModel;

namespace Ui {
	class RadiationFormClass;
}

class RadiationForm : public QWidget
{
	Q_OBJECT

public:
	explicit RadiationForm(QWidget *parent = nullptr);
	~RadiationForm();

	// 初始化参数
	void InitSet();

	//信号——槽
	void GUIConnect();

	// 清空当前标签页表格
	void ClearTabTable();

	// 导出当前标签页数据到文件
	// 返回保存的目标文件的完整路径
	std::string ExportTabRadData();

	// 导出当前标签页表格到图片
	// 返回保存的目标文件的完整路径
	std::string ExportTabRadChart();


private slots:

	// 辐射计算响应函数
	void RadCountSlots();

	// 添加数据
	void AddData();

private:

	/// 绘制曲线1
	void Draw1();

	/// 设置tableView的表头
	void SetHeader();

private:

	//辐射计算参数
	CIRRadiateParam  m_Param;

	//辐射计算类实例
	CIRRadiate  m_IRRadiate;

	std::vector<std::vector<double>> temperatureList;

	// 曲线数据（温度-辐射值），即结果
	std::map<QString, std::vector<std::pair<float, float>>> m_mapChartTemperRad;

	// 曲线名称
	std::vector<QString> nameList;

	// 波段（只记录），计算时用 m_Param.m_pvWaveBand
	std::vector<std::pair<double, double>> waveBandList;

	// 表model
	QStandardItemModel* model = new QStandardItemModel();

private:
	Ui::RadiationFormClass *ui;
};

#endif