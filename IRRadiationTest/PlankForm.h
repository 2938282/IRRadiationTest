#pragma once

#include <QWidget>
#include "ui_PlankForm.h"
#include <IRRadiation/IRRadiate.h>
#include <QStandardItemModel>
// qt messenge box
#include <qmessagebox.h>
// qt�ļ������
#include <QFileDialog>
// c++�ļ�����
#include <iostream>
#include <fstream>

using namespace IRModel;

// �������
class InputInfo
{
public:
	//��������
	QString name;
	// �̶�ֵ�Ƿ�Ϊ����
	bool isWaveFixed;
	// �̶�ֵ
	double fixed;
	// �仯ֵ
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
	// ��յ�ǰ��ǩҳ���
	void ClearTabTable();

	// ����̫����������
	void ImportSunData();

	// ������ǰ��ǩҳ���ݵ��ļ�
	// ���ر����Ŀ���ļ�������·��
	std::string ExportTabRadData();

	// ������ǰ��ǩҳ���ͼƬ
	// ���ر����Ŀ���ļ�������·��
	std::string ExportTabRadChart();

private:
	void GUIConnect();
	float RadiationL(float Lambda, float T);
	void SetHeader();
	void Draw();
	
	Ui::PlankFormClass ui;
	//����������
	CIRRadiateParam  m_Param;
	//���������ʵ��
	CIRRadiate  m_IRRadiate;
	// ��model
	QStandardItemModel* model = new QStandardItemModel();
	// ��������
	std::vector<InputInfo> inputInfo;
	
	// ���
	std::map<QString, std::vector<std::pair<float, float>>> result;
	
private slots:
	// ���������Ӧ����
	void RadCountSlots();

	// �������
	void AddData();
	void SelectTemp();
	void SelectWave();
};
