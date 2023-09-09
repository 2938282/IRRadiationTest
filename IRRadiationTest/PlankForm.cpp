#include "PlankForm.h"

PlankForm::PlankForm(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	GUIConnect();
	ui.radioButtonWave->setChecked(true);
	// ���ñ�ͷ
	SetHeader();
}

PlankForm::~PlankForm()
{}

void PlankForm::ClearTabTable()
{
	try
	{
		// ����ڲ�������
		inputInfo.clear();

		// ��ձ���ϵ�����
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
	// Ŀ���ļ����ļ���
	std::string fileName = "";

	// Ŀ���ļ�
	ofstream fileStream;

	if (result.empty())
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


	fileStream << u8"�ܹ�����" << result.size() << u8"��������Ϣ:" << endl << endl;

	try
	{
		// ������������2�����Ŀ¼
		if (result.size() > 1)
		{
			int lineCount = 0;
			fileStream << u8"Ŀ¼��" << endl;

			for (int i = 0; i < result.size(); i++)
			{
				fileStream << (i + 1) << u8"." << inputInfo[i].name.toStdString() << u8"����" << (result.size() + lineCount + 5) << u8"��" << endl;
				lineCount += result[inputInfo[i].name].size() + 5;
			}
			fileStream << endl;
		}

		// �������
		int count = 0;
		for (auto i = result.begin(); i != result.end(); i++)
		{
			count++;
			// ��������
			fileStream << count << u8". �������ƣ�" << i->first.toStdString() << endl;
			fileStream << "  " << u8"�̶�ֵ��" << ((inputInfo[count - 1].isWaveFixed)? (u8"����") : (u8"�¶�")) << endl;
			fileStream << "  " << u8"�仯ֵ��" << ((inputInfo[count - 1].isWaveFixed) ? (u8"�¶�") : (u8"����")) << endl;

			fileStream << "  " << u8"�仯ֵ��Χ(�¶� K������ um)��" << inputInfo[count - 1].start << "~ " << inputInfo[count - 1].end << endl;
			fileStream << "  " << u8"�仯ֵ����(�¶� K������ um)��" << inputInfo[count - 1].step << endl;
			fileStream << "  " << u8"�̶�ֵ(�¶� K������ um)��" << inputInfo[count - 1].fixed << endl;

			// ��������
			std::vector<std::pair<float, float>> dataList = i->second;

			// ��ͷ
			fileStream << '\t' << u8"�仯ֵ(�¶� K������ um)" << '\t' << u8"����ֵ" << endl;

			// ����
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
	// Ŀ���ļ����ļ���
	std::string fileName = "";


	if (ui.widgetChart->GetLineCount() <= 0)
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
		// ��������̶�
		if (inputInfo[i].isWaveFixed)
		{
			for (double j = inputInfo[i].start; j <= inputInfo[i].end; j += inputInfo[i].step)
			{
				temp.push_back(std::make_pair(j, RadiationL(inputInfo[i].fixed, j)));
			}
		}
		// ����¶ȹ̶�
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
		QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
		return;
	}

	return;
}

void PlankForm::AddData()
{
	// �̶���
	QString fixed = ui.radioButtonTemp->isChecked() ? u8"�¶�" : u8"����";
	double fixedValue = ui.doubleSpinFixedValue->value();

	// �仯��
	QString variable = ui.radioButtonTemp->isChecked() ? u8"����  " : u8"�¶�";
	double start = ui.doubleSpinBoxStart->value();
	double end = ui.doubleSpinBoxEnd->value();
	double space = ui.doubleSpinBoxSpace->value();

	// ��������
	QString name = ui.lineEditName->text();

	// ������Ϊ�գ���ĳɷ�������xx
	if (name == "")
	{
		name = u8"��������" + QString::number(inputInfo.size() + 1);

		//((IRRadiationTest*)(parentWidget()->parentWidget()))->ShowInStatusBar(u8"δ�����������ƣ�����ΪĬ������");
	}

	// �����ֹ���������Ϸ���
	if (start > end)
	{
		QMessageBox::critical(NULL, u8"����", u8"��ֵֹӦ�ô�����ʼֵ��", QMessageBox::Ok);

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

	// �ڱ��������ʾ
	try
	{
		// ��Χ
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

		// �������ݵ�����С
		ui.tableAddedData->resizeColumnsToContents();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(NULL, u8"����", e.what(), QMessageBox::Ok);
		return;
	}

	return;
}

void PlankForm::SelectTemp()
{
	ui.radioButtonWave->setChecked(false);
	ui.labelRange->setText(u8"������Χ����λ��um��");
	ui.labelStep->setText(u8"������������λ��um��");
	ui.lableFixed->setText(u8"�¶ȣ���λ��K��");
}

void PlankForm::SelectWave()
{
	ui.radioButtonTemp->setChecked(false);
	ui.labelRange->setText(u8"�¶ȷ�Χ����λ��K��");
	ui.labelStep->setText(u8"�¶Ȳ�������λ��K��");
	ui.lableFixed->setText(u8"��������λ��um��");
}

/////////////////////////
//�������ƣ�RadiationL(float Lambda, float T)
//�������ܣ����ú�����乫ʽ����������ȣ���λw/(sr.cm-2)��
//����˵����
//-----Lambda:   ��������λ��΢�ף�
//-----num��     ����������Ϊ1
//-----param:    ��Ų��������飬param[0]�������¶ȣ���λ��K
//����ֵ����Ӧ�����ķ�������M��lambda��
//ȫ�ֱ����� ��
//��д�ˣ����   ��д���ڣ� 2009/09/09
//�޸��ˣ�         �޸����ڣ�
////////////////////////
float  PlankForm::RadiationL(float Lambda, float T)
{
	float c1;            //c1 = 2 * pi * h * c*c; h:���ʿ˳����� c������
	float c2;            //c2 = hc/k;  k:������������
	float temp1, temp2;
	float result;

	c1 = 3.741832e4f;      //��λ�� W cm-2 ΢��-1
	c2 = 1.43879e4f;      //��λ�� ΢�� K ;  K:�����¶�

	temp1 = c2 / (Lambda * T);
	temp2 = exp(temp1) - 1;
	temp1 = pow(Lambda, 5);
	temp2 = temp1 * temp2;

	result = c1 / (M_PI * temp2);

	return result;
}

void PlankForm::SetHeader()
{
	// ����model����
	ui.tableAddedData->setModel(model);

	// ��ͷ
	QStringList header = { u8"Ŀ����������", u8"�̶���",u8"�̶���ֵ", u8"�仯��", u8"�仯������", u8"�仯������"};

	// ��ʾ����
	ui.tableAddedData->setShowGrid(true);

	// ���ñ�ͷ
	model->setHorizontalHeaderLabels(header);
}

// ��������1
void PlankForm::Draw()
{
	try
	{
		ui.widgetChart->DrawLine(QString::fromLocal8Bit("����ͼ"), result);
		ui.widgetChart->show();

	}
	catch (const std::exception&)
	{
		throw;
	}
}