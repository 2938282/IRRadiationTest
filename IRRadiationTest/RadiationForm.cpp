#include "RadiationForm.h"

RadiationForm::RadiationForm(QWidget* parent)
	: QWidget(parent),
	ui(new Ui::RadiationFormClass)
{
	ui->setupUi(this);

	ui->widgetChart->SetColor(QColor(128, 128, 128));

	InitSet();

	GUIConnect();

	// 设置表头
	SetHeader();
}

RadiationForm::~RadiationForm()
{}


void RadiationForm::GUIConnect()
{
	connect(ui->pushButton_Count, SIGNAL(clicked()), this, SLOT(RadCountSlots()));
	connect(ui->buttonAddData, SIGNAL(clicked()), this, SLOT(AddData()));
}

// 清空当前标签页表格
void RadiationForm::ClearTabTable()
{
	try
	{
		// 清空内部的数据
		nameList.clear();
		temperatureList.clear();
		m_Param.m_pvWaveBand->clear();
		m_mapChartTemperRad.clear();

		// 清空表格上的数据
		model->clear();
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	return;

}

// 导出当前标签页数据
// 返回保存的目标文件的完整路径
std::string RadiationForm::ExportTabRadData()
{
	// 目标文件的文件名
	std::string fileName = "";

	// 目标文件
	ofstream fileStream;

	if (m_mapChartTemperRad.empty())
	{
		throw u8"还没有进行计算，没有可以导出的内容。";
	}

	try
	{
		// 在文件浏览器中选择目标文件
		fileName = QFileDialog::getSaveFileName(this, u8"选择保存位置", "./", u8"文本文件(*.txt *.text)").toStdString();
	}
	catch (const std::exception&)
	{
		throw;
	}


	try
	{
		// 打开目标文件
		fileStream.open(fileName);
	}
	catch (const std::exception&)
	{
		throw;
	}


	fileStream << u8"总共包含" << m_mapChartTemperRad.size() << u8"个曲线信息:" << endl << endl;

	try
	{
		// 若曲线数大于2，输出目录
		if (nameList.size() > 1)
		{
			int lineCount = 0;
			fileStream << u8"目录：" << endl;

			for (int i = 0; i < nameList.size(); i++)
			{
				fileStream << (i + 1) << u8"." << nameList[i].toStdString() << u8"：第" << (nameList.size() + lineCount + 5) << u8"行" << endl;
				lineCount += m_mapChartTemperRad[nameList[i]].size() + 5;
			}
			fileStream << endl;
		}

		// 输出正文
		int count = 0;
		for (auto i = m_mapChartTemperRad.begin(); i != m_mapChartTemperRad.end(); i++)
		{
			count++;
			// 曲线名称
			fileStream << count << u8". 曲线名称：" << i->first.toStdString() << endl;

			fileStream << "  " << u8"温度范围(K)：" << temperatureList[count - 1][0] << "~ " << temperatureList[count - 1][1] << endl;
			fileStream << "  " << u8"温度步长(K)：" << temperatureList[count - 1][2] << endl;
			fileStream << "  " << u8"波段(um)：" << waveBandList[count - 1].first << "~ " << waveBandList[count - 1].second << endl;

			// 曲线数据
			std::vector<std::pair<float, float>> dataList = i->second;

			// 表头
			fileStream << '\t' << u8"温度(K)" << '\t' << u8"辐射值" << endl;

			// 表项
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
	// 目标文件的文件名
	std::string fileName = "";


	if (ui->widgetChart->GetLineCount() <= 0)
	{
		throw u8"还没有进行计算，没有可以导出的内容。";
	}

	try
	{
		// 在文件浏览器中选择目标文件
		fileName = QFileDialog::getSaveFileName(this, u8"选择保存位置", "./", u8"图片(*.png)").toStdString();
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

// 初始化参数设定
void RadiationForm::InitSet()
{
	//// 起始温度
	//m_Param.m_nStTemperature = ui->doubleSpinBoxStartTemperature->value();

	//// 温度数量 = (终止温度 - 起始温度)/步长 + 1
	//m_Param.m_nCount = (ui->doubleSpinBoxEndTemperature->value() - ui->doubleSpinBoxStartTemperature->value())
	//	/ ui->doubleSpinBoxSpacingTemperature->value() + 1;

	//// 起止波长
	//m_Param.m_pvWaveBand->push_back(glm::vec2(ui->doubleSpinBoxWaveStart->value(), ui->doubleSpinBoxWaveEnd->value()))

}

// 辐射计算响应函数
void RadiationForm::RadCountSlots()
{
	if (temperatureList.empty())
	{
		QMessageBox::critical(NULL, u8"错误", u8"还未添加参数，无法计算", QMessageBox::Ok);
		return;
	}

	// 计算辐射数据；
	m_IRRadiate.Initialize(m_Param);


	for (int j = 0; j < temperatureList.size(); j++)
	{
		// 温度-辐射值数组
		std::vector<std::pair<float, float>> temperaRadPairs;

		std::vector<double> t_data = temperatureList[j];

		// 曲线名称
		QString chartTitle = nameList[j];

		// 起始温度
		float startTemperature = t_data[0];

		// 温度步长
		float stepLength = t_data[2];

		// 终止温度
		float endTemperature = t_data[1];

		// 目标温度数量
		int temperatureCount = (endTemperature - startTemperature) / stepLength + 1;

		// 算出温度-辐射值
		for (int i = 0; i < temperatureCount; i++)
		{
			// 当前温度
			float curT = startTemperature + i * stepLength;

			// 当前辐射值
			float curRad = 0;

			m_IRRadiate.GetRadiation(j, curT, curRad);

			//将根据当前温度算出的辐射值加入数组中
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
		QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
		return;
	}

	return;
}

// 添加计算数据
void RadiationForm::AddData()
{
	std::vector <double> temperatureData;

	// 波段
	double waveS = ui->doubleSpinBoxWaveStart->value();
	double waveE = ui->doubleSpinBoxWaveEnd->value();

	// 曲线名称
	QString name = ui->lineEditName->text();

	// 若名称为空，则改成辐射曲线xx
	if (name == "")
	{
		name = u8"辐射曲线" + QString::number(nameList.size() + 1);

		//((IRRadiationTest*)(parentWidget()->parentWidget()))->ShowInStatusBar(u8"未输入曲线名称，已置为默认名称");
	}

	// 检查起止波长参数合法性
	if (waveS > waveE)
	{
		QMessageBox::critical(NULL, u8"错误", u8"终止波长应该大于起始波长。", QMessageBox::Ok);

		return;
	}
	if (waveS <= 0)
	{
		QMessageBox::critical(NULL, u8"错误", u8"波长应不小于0", QMessageBox::Ok);

		return;
	}

	// 检查起止温度参数合法性
	if (ui->doubleSpinBoxEndTemperature->value() < ui->doubleSpinBoxStartTemperature->value())
	{
		QMessageBox::critical(NULL, u8"错误", u8"终止温度应该大于起始温度。", QMessageBox::Ok);
		return;
	}

	// 温度数据，格式为{起始温度，终止温度，步长}
	temperatureData = { ui->doubleSpinBoxStartTemperature->value(), ui->doubleSpinBoxEndTemperature->value(), ui->doubleSpinBoxSpacingTemperature->value() };
	temperatureList.push_back(temperatureData);

	// 起止波长
	m_Param.m_pvWaveBand->push_back(glm::vec2(waveS, waveE));
	waveBandList.push_back(std::pair<double, double>(waveS, waveE));

	// 添加曲线名称
	nameList.push_back(name);

	// 在表中添加显示
	try
	{

		// 温度范围
		QString temBand = QString::fromStdString(std::to_string(temperatureData[0]) + "~" + std::to_string(temperatureData[1]));

		// 温度步长
		QString temStep = QString::fromStdString(std::to_string(temperatureData[2]));

		// 波段
		QString waveBand = QString::fromStdString(std::to_string(waveS) + "~" + std::to_string(waveE));

		model->appendRow(QList<QStandardItem*>{new QStandardItem(name), new QStandardItem(temBand), new QStandardItem(temStep), new QStandardItem(waveBand)});

		// 根据内容调整大小
		ui->tableAddedData->resizeColumnsToContents();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
		return;
	}

	return;

}

// 绘制曲线1
void RadiationForm::Draw1()
{
	try
	{
		ui->widgetChart->DrawLine(QString::fromLocal8Bit("曲线图"), m_mapChartTemperRad);
		ui->widgetChart->show();

	}
	catch (const std::exception&)
	{
		throw;
	}
}

// 设置tableView的表头
void RadiationForm::SetHeader()
{
	// 设置model到表
	ui->tableAddedData->setModel(model);

	// 表头
	QStringList header = { u8"目标曲线名称", u8"温度范围",u8"温度步长",u8"波段" };

	// 显示格子
	ui->tableAddedData->setShowGrid(true);

	// 设置表头
	model->setHorizontalHeaderLabels(header);

}
