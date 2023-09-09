#include "PlankForm.h"

PlankForm::PlankForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	GUIConnect();
	ui.radioButtonWave->setChecked(true);
	// 设置表头
	SetHeader();
}

PlankForm::~PlankForm()
{}

void PlankForm::ClearTabTable()
{
	try
	{
		// 清空内部的数据
		inputInfo.clear();

		// 清空表格上的数据
		model->clear();
	}
	catch (const std::exception& e)
	{
		throw e;
	}

	return;
}

void PlankForm::ImportSunData()
{
	ui.radioButtonTemp->setChecked(true);
	SelectTemp();
	ui.doubleSpinFixedValue->setValue(5778);
	ui.doubleSpinBoxStart->setValue(0.2);
	ui.doubleSpinBoxEnd->setValue(2.6);
	ui.doubleSpinBoxSpace->setValue(1e-3);
	
}

std::string PlankForm::ExportTabRadData()
{
	// 目标文件的文件名
	std::string fileName = "";

	// 目标文件
	ofstream fileStream;

	if (result.empty())
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


	fileStream << u8"总共包含" << result.size() << u8"个曲线信息:" << endl << endl;

	try
	{
		// 若曲线数大于2，输出目录
		if (result.size() > 1)
		{
			int lineCount = 0;
			fileStream << u8"目录：" << endl;

			for (int i = 0; i < result.size(); i++)
			{
				fileStream << (i + 1) << u8"." << inputInfo[i].name.toStdString() << u8"：第" << (result.size() + lineCount + 5) << u8"行" << endl;
				lineCount += result[inputInfo[i].name].size() + 5;
			}
			fileStream << endl;
		}

		// 输出正文
		int count = 0;
		for (auto i = result.begin(); i != result.end(); i++)
		{
			count++;
			// 曲线名称
			fileStream << count << u8". 曲线名称：" << i->first.toStdString() << endl;
			fileStream << "  " << u8"固定值：" << ((inputInfo[count - 1].isWaveFixed)? (u8"波长") : (u8"温度")) << endl;
			fileStream << "  " << u8"变化值：" << ((inputInfo[count - 1].isWaveFixed) ? (u8"温度") : (u8"波长")) << endl;

			fileStream << "  " << u8"变化值范围(温度 K，波长 um)：" << inputInfo[count - 1].start << "~ " << inputInfo[count - 1].end << endl;
			fileStream << "  " << u8"变化值步长(温度 K，波长 um)：" << inputInfo[count - 1].step << endl;
			fileStream << "  " << u8"固定值(温度 K，波长 um)：" << inputInfo[count - 1].fixed << endl;

			// 曲线数据
			std::vector<std::pair<float, float>> dataList = i->second;

			// 表头
			fileStream << '\t' << u8"变化值(温度 K，波长 um)" << '\t' << u8"辐射值" << endl;

			// 表项
			for (int j = 0; j < dataList.size(); j++)
			{
				std::pair<float, float> data = dataList[j];

				fileStream << '\t' << data.first << "\t\t\t" << data.second << endl;
			}
		}
	}
	catch (const std::exception&)
	{
		throw;
	}

	return fileName;
}

std::string PlankForm::ExportTabRadChart()
{
	// 目标文件的文件名
	std::string fileName = "";


	if (ui.widgetChart->GetLineCount() <= 0)
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
		ui.widgetChart->grab().save(QString::fromStdString(fileName));
	}
	catch (const std::exception& e)
	{
		throw;
	}


	return fileName;
}

void PlankForm::GUIConnect()
{
	// connect radioButtonTemp
	connect(ui.radioButtonTemp, SIGNAL(clicked()), this, SLOT(SelectTemp()));
	// connect radioButtonWave
	connect(ui.radioButtonWave, SIGNAL(clicked()), this, SLOT(SelectWave()));
	connect(ui.pushButton_Count, SIGNAL(clicked()), this, SLOT(RadCountSlots()));
	connect(ui.buttonAddData, SIGNAL(clicked()), this, SLOT(AddData()));
}

void PlankForm::RadCountSlots()
{
	result.clear();
	for (int i = 0; i < inputInfo.size(); i++)
	{
		std::vector<std::pair<float, float>> temp;
		// 如果波长固定
		if (inputInfo[i].isWaveFixed)
		{
			for (double j = inputInfo[i].start; j <= inputInfo[i].end; j += inputInfo[i].step)
			{
				temp.push_back(std::make_pair(j, RadiationL(inputInfo[i].fixed, j)));
			}
		}
		// 如果温度固定
		else
		{
			for (double j = inputInfo[i].start; j <= inputInfo[i].end; j += inputInfo[i].step)
			{
				temp.push_back(std::make_pair(j, RadiationL(j, inputInfo[i].fixed)));
			}
		}
		result.insert(std::make_pair(inputInfo[i].name, temp));
	}

	try
	{
		Draw();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
		return;
	}

	return;
}

void PlankForm::AddData()
{
	// 固定量
	QString fixed = ui.radioButtonTemp->isChecked() ? u8"温度" : u8"波长";
	double fixedValue = ui.doubleSpinFixedValue->value();

	// 变化量
	QString variable = ui.radioButtonTemp->isChecked() ? u8"波长  " : u8"温度";
	double start = ui.doubleSpinBoxStart->value();
	double end = ui.doubleSpinBoxEnd->value();
	double space = ui.doubleSpinBoxSpace->value();

	// 曲线名称
	QString name = ui.lineEditName->text();

	// 若名称为空，则改成辐射曲线xx
	if (name == "")
	{
		name = u8"辐射曲线" + QString::number(inputInfo.size() + 1);

		//((IRRadiationTest*)(parentWidget()->parentWidget()))->ShowInStatusBar(u8"未输入曲线名称，已置为默认名称");
	}

	// 检查起止波长参数合法性
	if (start > end)
	{
		QMessageBox::critical(NULL, u8"错误", u8"终止值应该大于起始值。", QMessageBox::Ok);

		return;
	}
	
	InputInfo* tmp = new InputInfo();
	tmp->name = name;
	tmp->isWaveFixed = ui.radioButtonWave->isChecked();
	tmp->fixed = fixedValue;
	tmp->start = start;
	tmp->end = end;
	tmp->step = space;
	inputInfo.push_back(*tmp);

	// 在表中添加显示
	try
	{
		// 范围
		QString band = QString::fromStdString(std::to_string(start) + "~" + std::to_string(end));

		QString fixedValueStr = QString::fromStdString(std::to_string(fixedValue));
		QString spaceStr = QString::fromStdString(std::to_string(space));

		model->appendRow(QList<QStandardItem*>{
			new QStandardItem(name),
			new QStandardItem(fixed),
			new QStandardItem(fixedValueStr),
			new QStandardItem(variable),
			new QStandardItem(band),
			new QStandardItem(spaceStr)
		});

		// 根据内容调整大小
		ui.tableAddedData->resizeColumnsToContents();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(NULL, u8"错误", e.what(), QMessageBox::Ok);
		return;
	}

	return;
}

void PlankForm::SelectTemp()
{
	ui.radioButtonWave->setChecked(false);
	ui.labelRange->setText(u8"波长范围（单位：um）");
	ui.labelStep->setText(u8"波长步长（单位：um）");
	ui.lableFixed->setText(u8"温度（单位：K）");
}

void PlankForm::SelectWave()
{
	ui.radioButtonTemp->setChecked(false);
	ui.labelRange->setText(u8"温度范围（单位：K）");
	ui.labelStep->setText(u8"温度步长（单位：K）");
	ui.lableFixed->setText(u8"波长（单位：um）");
}

/////////////////////////
//函数名称：RadiationL(float Lambda, float T)
//函数功能：利用黑体辐射公式进计算光亮度，单位w/(sr.cm-2)；
//参数说明：
//-----Lambda:   波长，单位：微米；
//-----num：     参数个数，为1
//-----param:    存放参数的数组，param[0]：绝对温度，单位：K
//返回值：对应波长的辐射出射度M（lambda）
//全局变量： 无
//编写人：杨东升   编写日期： 2009/09/09
//修改人：         修改日期：
////////////////////////
float  PlankForm::RadiationL(float Lambda, float T)
{
	float c1;            //c1 = 2 * pi * h * c*c; h:普朗克常数； c：光速
	float c2;            //c2 = hc/k;  k:波耳兹曼常数
	float temp1, temp2;
	float result;

	c1 = 3.741832e4f;      //单位： W cm-2 微米-1
	c2 = 1.43879e4f;      //单位： 微米 K ;  K:绝对温度

	temp1 = c2 / (Lambda * T);
	temp2 = exp(temp1) - 1;
	temp1 = pow(Lambda, 5);
	temp2 = temp1 * temp2;

	result = c1 / (M_PI * temp2);

	return result;
}

void PlankForm::SetHeader()
{
	// 设置model到表
	ui.tableAddedData->setModel(model);

	// 表头
	QStringList header = { u8"目标曲线名称", u8"固定量",u8"固定量值", u8"变化量", u8"变化量区间", u8"变化量步长"};

	// 显示格子
	ui.tableAddedData->setShowGrid(true);

	// 设置表头
	model->setHorizontalHeaderLabels(header);
}

// 绘制曲线1
void PlankForm::Draw()
{
	try
	{
		ui.widgetChart->DrawLine(QString::fromLocal8Bit("曲线图"), result);
		ui.widgetChart->show();

	}
	catch (const std::exception&)
	{
		throw;
	}
}