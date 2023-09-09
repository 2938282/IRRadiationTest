#include "RadiationForm.h"

RadiationForm::RadiationForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::RadiationFormClass)
{
	ui->setupUi(this);

	ui->widgetChart->SetColor(QColor(128, 128, 128));

	InitSet();

	GUIConnect();

	// ���ñ�ͷ
	SetHeader();
}

RadiationForm::~RadiationForm()
{}


void RadiationForm::GUIConnect()
{
	connect(ui->pushButton_Count, SIGNAL(clicked()), this, SLOT(RadCountSlots()));
	connect(ui->buttonAddData, SIGNAL(clicked()), this, SLOT(AddData()));
}

// ��յ�ǰ��ǩҳ���
void RadiationForm::ClearTabTable()
{
	try
	{
		// ����ڲ�������
		nameList.clear();
		temperatureList.clear();
		m_Param.m_pvWaveBand->clear();
		m_mapChartTemperRad.clear();

		// ��ձ���ϵ�����
		model->clear();
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	return;

}

// ������ǰ��ǩҳ����
// ���ر����Ŀ���ļ�������·��
std::string RadiationForm::ExportTabRadData()
{
	// Ŀ���ļ����ļ���
	std::string fileName = "";

	// Ŀ���ļ�
	ofstream fileStream;

	if (m_mapChartTemperRad.empty())
	{
		throw u8"��û�н��м��㣬û�п��Ե��������ݡ�";
	}

	try
	{
		// ���ļ��������ѡ��Ŀ���ļ�
		fileName = QFileDialog::getSaveFileName(this, u8"ѡ�񱣴�λ��", "./", u8"�ı��ļ�(*.txt *.text)").toStdString();
	}
	catch (const std::exception&)
	{
		throw;
	}


	try
	{
		// ��Ŀ���ļ�
		fileStream.open(fileName);
	}
	catch (const std::exception&)
	{
		throw;
	}


	fileStream << u8"�ܹ�����" << m_mapChartTemperRad.size() << u8"��������Ϣ:" << endl << endl;

	try
	{
		// ������������2�����Ŀ¼
		if (nameList.size() > 1)
		{
			int lineCount = 0;
			fileStream << u8"Ŀ¼��" << endl;

			for (int i = 0; i < nameList.size(); i++)
			{
				fileStream << (i + 1) << u8"." << nameList[i].toStdString() << u8"����" << (nameList.size() + lineCount + 5) << u8"��" << endl;
				lineCount += m_mapChartTemperRad[nameList[i]].size() + 5;
			}
			fileStream << endl;
		}

		// �������
		int count = 0;
		for (auto i = m_mapChartTemperRad.begin(); i != m_mapChartTemperRad.end(); i++)
		{
			count++;
			// ��������
			fileStream << count << u8". �������ƣ�" << i->first.toStdString() << endl;

			fileStream << "  " << u8"�¶ȷ�Χ(K)��" << temperatureList[count - 1][0] << "~ " << temperatureList[count - 1][1] << endl;
			fileStream << "  " << u8"�¶Ȳ���(K)��" << temperatureList[count - 1][2] << endl;
			fileStream << "  " << u8"����(um)��" << waveBandList[count - 1].first << "~ " << waveBandList[count - 1].second << endl;

			// ��������
			std::vector<std::pair<float, float>> dataList = i->second;

			// ��ͷ
			fileStream << '\t' << u8"�¶�(K)" << '\t' << u8"����ֵ" << endl;

			// ����
			for (int j = 0; j < dataList.size(); j++)
			{
				std::pair<float, float> data = dataList[j];

				fileStream << '\t' << data.first << '\t' << data.second << endl;
			}
		}
	}
	catch (const std::exception&)
	{
		throw;
	}

	return fileName;
}

std::string RadiationForm::ExportTabRadChart()
{
	// Ŀ���ļ����ļ���
	std::string fileName = "";


	if (ui->widgetChart->GetLineCount() <= 0)
	{
		throw u8"��û�н��м��㣬û�п��Ե��������ݡ�";
	}

	try
	{
		// ���ļ��������ѡ��Ŀ���ļ�
		fileName = QFileDialog::getSaveFileName(this, u8"ѡ�񱣴�λ��", "./", u8"ͼƬ(*.png)").toStdString();
	}
	catch (const std::exception&)
	{
		throw;
	}


	try
	{
		ui->widgetChart->grab().save(QString::fromStdString(fileName));
	}
	catch (const std::exception& e)
	{
		throw;
	}


	return fileName;
}

// ��ʼ�������趨
void RadiationForm::InitSet()
{
	//// ��ʼ�¶�
	//m_Param.m_nStTemperature = ui->doubleSpinBoxStartTemperature->value();

	//// �¶����� = (��ֹ�¶� - ��ʼ�¶�)/���� + 1
	//m_Param.m_nCount = (ui->doubleSpinBoxEndTemperature->value() - ui->doubleSpinBoxStartTemperature->value())
	//	/ ui->doubleSpinBoxSpacingTemperature->value() + 1;

	//// ��ֹ����
	//m_Param.m_pvWaveBand->push_back(glm::vec2(ui->doubleSpinBoxWaveStart->value(), ui->doubleSpinBoxWaveEnd->value()))

}

// ���������Ӧ����
void RadiationForm::RadCountSlots()
{
	if (temperatureList.empty())
	{
		QMessageBox::critical(NULL, u8"����", u8"��δ��Ӳ������޷�����", QMessageBox::Ok);
		return;
	}

	// ����������ݣ�
	m_IRRadiate.Initialize(m_Param);


	for (int j = 0; j < temperatureList.size(); j++)
	{
		// �¶�-����ֵ����
		std::vector<std::pair<float, float>> temperaRadPairs;

		std::vector<double> t_data = temperatureList[j];

		// ��������
		QString chartTitle = nameList[j];

		// ��ʼ�¶�
		float startTemperature = t_data[0];

		// �¶Ȳ���
		float stepLength = t_data[2];

		// ��ֹ�¶�
		float endTemperature = t_data[1];

		// Ŀ���¶�����
		int temperatureCount = (endTemperature - startTemperature) / stepLength + 1;

		// ����¶�-����ֵ
		for (int i = 0; i < temperatureCount; i++)
		{
			// ��ǰ�¶�
			float curT = startTemperature + i * stepLength;

			// ��ǰ����ֵ
			float curRad = 0;

			m_IRRadiate.GetRadiation(j, curT, curRad);

			//�����ݵ�ǰ�¶�����ķ���ֵ����������
			temperaRadPairs.push_back(std::pair<float, float>(curT, curRad));
		}

		m_mapChartTemperRad[chartTitle] = temperaRadPairs;
	}

	try
	{
		Draw1();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
		return;
	}

	return;
}

// ��Ӽ�������
void RadiationForm::AddData()
{
	std::vector <double> temperatureData;

	// ����
	double waveS = ui->doubleSpinBoxWaveStart->value();
	double waveE = ui->doubleSpinBoxWaveEnd->value();

	// ��������
	QString name = ui->lineEditName->text();

	// ������Ϊ�գ���ĳɷ�������xx
	if (name == "")
	{
		name = u8"��������" + QString::number(nameList.size() + 1);

		//((IRRadiationTest*)(parentWidget()->parentWidget()))->ShowInStatusBar(u8"δ�����������ƣ�����ΪĬ������");
	}

	// �����ֹ���������Ϸ���
	if (waveS > waveE)
	{
		QMessageBox::critical(NULL, u8"����", u8"��ֹ����Ӧ�ô�����ʼ������", QMessageBox::Ok);

		return;
	}
	if (waveS <= 0)
	{
		QMessageBox::critical(NULL, u8"����", u8"����Ӧ��С��0", QMessageBox::Ok);

		return;
	}

	// �����ֹ�¶Ȳ����Ϸ���
	if (ui->doubleSpinBoxEndTemperature->value() < ui->doubleSpinBoxStartTemperature->value())
	{
		QMessageBox::critical(NULL, u8"����", u8"��ֹ�¶�Ӧ�ô�����ʼ�¶ȡ�", QMessageBox::Ok);
		return;
	}

	// �¶����ݣ���ʽΪ{��ʼ�¶ȣ���ֹ�¶ȣ�����}
	temperatureData = { ui->doubleSpinBoxStartTemperature->value(), ui->doubleSpinBoxEndTemperature->value(), ui->doubleSpinBoxSpacingTemperature->value() };
	temperatureList.push_back(temperatureData);

	// ��ֹ����
	m_Param.m_pvWaveBand->push_back(glm::vec2(waveS, waveE));
	waveBandList.push_back(std::pair<double, double>(waveS, waveE));

	// �����������
	nameList.push_back(name);

	// �ڱ��������ʾ
	try
	{

		// �¶ȷ�Χ
		QString temBand = QString::fromStdString(std::to_string(temperatureData[0]) + "~" + std::to_string(temperatureData[1]));

		// �¶Ȳ���
		QString temStep = QString::fromStdString(std::to_string(temperatureData[2]));

		// ����
		QString waveBand = QString::fromStdString(std::to_string(waveS) + "~" + std::to_string(waveE));

		model->appendRow(QList<QStandardItem*>{new QStandardItem(name), new QStandardItem(temBand), new QStandardItem(temStep), new QStandardItem(waveBand)});

		// �������ݵ�����С
		ui->tableAddedData->resizeColumnsToContents();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
		return;
	}

	return;

}

// ��������1
void RadiationForm::Draw1()
{
	try
	{
		ui->widgetChart->DrawLine(QString::fromLocal8Bit("����ͼ"), m_mapChartTemperRad);
		ui->widgetChart->show();

	}
	catch (const std::exception&)
	{
		throw;
	}
}

// ����tableView�ı�ͷ
void RadiationForm::SetHeader()
{
	// ����model����
	ui->tableAddedData->setModel(model);

	// ��ͷ
	QStringList header = { u8"Ŀ����������", u8"�¶ȷ�Χ",u8"�¶Ȳ���",u8"����" };

	// ��ʾ����
	ui->tableAddedData->setShowGrid(true);

	// ���ñ�ͷ
	model->setHorizontalHeaderLabels(header);

}
