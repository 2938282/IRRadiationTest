#ifndef RADIATIONFORM_H
#define RADIATIONFORM_H

#include <QWidget>
#include "ui_RadiationForm.h"

// qt�ļ������
#include <QFileDialog>
// qt messenge box
#include <qmessagebox.h>
// ������
#include <QStandardItemModel>
// ��ͼ���
#include "QChartWidget.h"

// c++�ļ�����
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

	// ��ʼ������
	void InitSet();

	//�źš�����
	void GUIConnect();

	// ��յ�ǰ��ǩҳ���
	void ClearTabTable();

	// ������ǰ��ǩҳ���ݵ��ļ�
	// ���ر����Ŀ���ļ�������·��
	std::string ExportTabRadData();

	// ������ǰ��ǩҳ���ͼƬ
	// ���ر����Ŀ���ļ�������·��
	std::string ExportTabRadChart();


private slots:

	// ���������Ӧ����
	void RadCountSlots();

	// �������
	void AddData();

private:

	/// ��������1
	void Draw1();

	/// ����tableView�ı�ͷ
	void SetHeader();

private:

	//����������
	CIRRadiateParam  m_Param;

	//���������ʵ��
	CIRRadiate  m_IRRadiate;

	std::vector<std::vector<double>> temperatureList;

	// �������ݣ��¶�-����ֵ���������
	std::map<QString, std::vector<std::pair<float, float>>> m_mapChartTemperRad;

	// ��������
	std::vector<QString> nameList;

	// ���Σ�ֻ��¼��������ʱ�� m_Param.m_pvWaveBand
	std::vector<std::pair<double, double>> waveBandList;

	// ��model
	QStandardItemModel* model = new QStandardItemModel();

private:
	Ui::RadiationFormClass *ui;
};

#endif