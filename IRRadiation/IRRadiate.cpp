#include "pch.h"
#include "IRRadiate.h"
#include "math.h"

using namespace  IRModel;

///////////////////////
//CIRRadiateParam
/////////////////////
CIRRadiateParam::CIRRadiateParam(void)
{
	m_nStTemperature = 200;			//起始温度
	m_nCount = 4000;					//温度数量

	m_pvWaveBand = new vector<glm::vec2>;
	//m_pvWaveBand->push_back(glm::vec2(3.0f, 5.0f));     //(glm::vec2(3.6f, 4.9f));
	//m_pvWaveBand->push_back(glm::vec2(3.0f, 3.7f));
}

CIRRadiateParam::~CIRRadiateParam(void)
{
	if (m_pvWaveBand != nullptr)
	{
		m_pvWaveBand->clear();
		m_pvWaveBand->shrink_to_fit();
		delete  m_pvWaveBand;
		m_pvWaveBand = nullptr;
	}
}

CIRRadiateParam::CIRRadiateParam(const CIRRadiateParam& rhs)
{
	m_pvWaveBand = new vector<glm::vec2>;

	this->m_nStTemperature = rhs.m_nStTemperature;
	this->m_nCount = rhs.m_nCount;

	this->m_pvWaveBand->clear();
	this->m_pvWaveBand->assign(rhs.m_pvWaveBand->begin(), rhs.m_pvWaveBand->end());
}

CIRRadiateParam& CIRRadiateParam::operator =(const CIRRadiateParam& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	this->m_nStTemperature = rhs.m_nStTemperature;
	this->m_nCount = rhs.m_nCount;

	this->m_pvWaveBand->clear();
	this->m_pvWaveBand->assign(rhs.m_pvWaveBand->begin(), rhs.m_pvWaveBand->end());
	return *this;
}


bool CIRRadiateParam::OpenFile(FILE* fp)
{
	float  fval;
	//读取文件路径及名称长度
	if (feof(fp))
	{
		return FALSE;
	}
	fwscanf_s(fp, L"%d", &m_nStTemperature);

	if (feof(fp))
	{
		return FALSE;
	}
	fwscanf_s(fp, L"%d", &m_nCount);

	int i, count;
	if (feof(fp))
	{
		return FALSE;
	}
	fwscanf_s(fp, L"%d", &count);
	m_pvWaveBand->clear();
	for (i = 0; i < count; i++)
	{
		glm::vec2  v2;
		fwscanf_s(fp, L"%f", &v2.x);
		fwscanf_s(fp, L"%f", &v2.y);
		m_pvWaveBand->push_back(v2);
	}

	return TRUE;
}

void CIRRadiateParam::SaveFile(FILE* fp)
{
	float  fval;

	//读取文件路径及名称长度	
	fwprintf_s(fp, L"%d", m_nStTemperature);
	fwprintf_s(fp, L"%d", m_nCount);

	int i, count;
	count = m_pvWaveBand->size();
	fwprintf_s(fp, L"%d", count);
	for (i = 0; i < count; i++)
	{
		glm::vec2  v2 = m_pvWaveBand->at(i);
		fwprintf_s(fp, L"%f", v2.x);
		fwprintf_s(fp, L"%f", v2.y);
	}
}


///////////////////////
//CIRRadiate
/////////////////////

CIRRadiate::CIRRadiate(void)
{
	m_bInit = FALSE;
	m_nEndTemperature = 0;
	m_pWaveData = new vector<vector<float> >;
}

CIRRadiate::~CIRRadiate(void)
{
	if (m_pWaveData != nullptr)
	{
		m_pWaveData->clear();
		m_pWaveData->shrink_to_fit();
		delete  m_pWaveData;
		m_pWaveData = nullptr;
	}
}

CIRRadiate::CIRRadiate(const CIRRadiate& rhs)
{
	m_pWaveData = new vector<vector<float> >;

	this->m_bInit = rhs.m_bInit;
	this->m_Param = rhs.m_Param;
	this->m_nEndTemperature = rhs.m_nEndTemperature;

	this->m_pWaveData->assign(rhs.m_pWaveData->begin(), rhs.m_pWaveData->end());
}

CIRRadiate& CIRRadiate::operator =(const CIRRadiate& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	this->m_bInit = rhs.m_bInit;
	this->m_Param = rhs.m_Param;
	this->m_nEndTemperature = rhs.m_nEndTemperature;

	this->m_pWaveData->clear();
	this->m_pWaveData->assign(rhs.m_pWaveData->begin(), rhs.m_pWaveData->end());
	return *this;
}

bool CIRRadiate::Initialize()
{
	if (m_bInit)
	{
		return TRUE;
	}

	if ((m_Param.m_nStTemperature <= 0) || (m_Param.m_nCount <= 1))
	{
		return FALSE;
	}
	if (!CheckWaveBand(*m_Param.m_pvWaveBand))
	{
		return FALSE;
	}
	m_nEndTemperature = m_Param.m_nStTemperature + m_Param.m_nCount;

	CreateArrayData(*m_Param.m_pvWaveBand);

	m_bInit = TRUE;
	return TRUE;
}

bool CIRRadiate::Initialize(CIRRadiateParam param)
{
	if (param.m_nCount <= 1)
	{
		return FALSE;
	}
	if (!CheckWaveBand(*param.m_pvWaveBand))
	{
		return FALSE;
	}
	m_Param = param;
	m_nEndTemperature = m_Param.m_nStTemperature + m_Param.m_nCount;

	CreateArrayData(*param.m_pvWaveBand);

	m_bInit = TRUE;
	return TRUE;
}

bool  CIRRadiate::CheckWaveBand(vector<glm::vec2>  vWaveBand)
{
	if (vWaveBand.size() <= 0)
	{
		return FALSE;
	}

	for (int i = 0; i < vWaveBand.size(); i++)
	{
		if ((vWaveBand[i].x <= 0.0f) || (vWaveBand[i].x >= vWaveBand[i].y))
		{
			return FALSE;
		}
	}

	return TRUE;
}

//根据温度返回辐射值；
bool  CIRRadiate::GetRadiation(int index, float T, float& rad)
{
	if ((!m_bInit) || (index >= m_pWaveData->size()))
	{
		return FALSE;
	}

	float  fval;
	int T1, id;

	fval = floorf(T);
	T1 = (int)fval;

	if (T1 <= m_Param.m_nStTemperature)
	{
		rad = (*m_pWaveData)[index][0];
		return TRUE;
	}

	if (T1 >= m_nEndTemperature - 1)
	{
		rad = (*m_pWaveData)[index][m_Param.m_nCount - 1];
		return TRUE;
	}

	float  cx, fval1, favl2;
	cx = T - fval;

	id = T1 - m_Param.m_nStTemperature;
	fval1 = (*m_pWaveData)[index][id];
	id++;

	favl2 = (*m_pWaveData)[index][id];
	rad = (1.0f - cx) * fval1 + cx * favl2;		//线性插值
	return TRUE;
}

void  CIRRadiate::CreateArrayData(vector<glm::vec2>  vWaveBand)
{
	//清除原有数据
	m_pWaveData->clear();

	int  i, j, count;
	float T;
	float step = 0.001f;		//波长积分步长
	float fval;

	count = vWaveBand.size();
	for (i = 0; i < count; i++)
	{
		vector<float> vData;
		glm::vec2  band;
		band.x = (*m_Param.m_pvWaveBand)[i].x;
		band.y = (*m_Param.m_pvWaveBand)[i].y;

		for (j = 0; j < m_Param.m_nCount; j++)
		{
			T = (float)(j + m_Param.m_nStTemperature);
			fval = SimpintRadiationL(band.x, band.y, step, T);

			vData.push_back(fval);
		}
		m_pWaveData->push_back(vData);
	}
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
float  CIRRadiate::RadiationL(float Lambda, float T)
{
	float c1;            //c1 = 2 * pi * h * c*c; h:普朗克常数； c：光速
	float c2;            //c1 = hc/k;  k:波耳兹曼常数
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

/////////////////////////
//函数名称：Simpintegral(float a, float b, int num, float * param, float step, USERFUN Fct)
//函数功能：利用复化辛普森公式对函数Fct进行数值积分
//参数说明：
//-----a:     积分上限
//-----b:     积分下限
//-----num:   函数的其它参数个数
//-----param: 函数的其它参数
//-----step:  积分步长
//-----Fct:   需要求积分的函数； float (*USERFUN)(float x, int num, float * param);
//返回值：   积分值
//全局变量： 无
//编写人：杨东升   编写日期： 2009/11/18
//修改人：         修改日期：
////////////////////////
float   CIRRadiate::SimpintRadiationL(float a, float b, float step, float T)
{
	float  xk;
	float  fa, fb, fxk1, fxk2;
	float  hk;    //步长
	int     k, n;
	float  result = 0;

	n = (int)fabs((b - a) / step);
	if (n % 2 != 0)
	{
		n = n + 1;
	}

	hk = (b - a) / n;

	fa = RadiationL(a, T);
	fb = RadiationL(b, T);

	fxk1 = 0;
	fxk2 = 0;
	for (k = 1; k < (0.5 * n); k++)
	{
		xk = a + (2 * k - 1) * hk;
		fxk1 += RadiationL(xk, T);

		xk = a + 2 * k * hk;
		fxk2 += RadiationL(xk, T);
	}
	xk = a + (2 * k - 1) * hk;
	fxk1 += RadiationL(xk, T);

	result = (fa + 4 * fxk1 + 2 * fxk2 + fb) * hk / 3;

	return result;
}

