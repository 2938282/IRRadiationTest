#ifndef __IRRADIATE_H_
#define __IRRADIATE_H_

#include "Export"
#include "IRBasic.h"

//黑体辐射计算类，主要用于建立温度与辐射亮度的关系
namespace  IRModel
{
	class IRRADIATION_EXPORT CIRRadiateParam
	{	
	public:
		CIRRadiateParam(void);
		~CIRRadiateParam(void);
		CIRRadiateParam(const CIRRadiateParam&);

	public:
		CIRRadiateParam & operator =(const CIRRadiateParam &);

	public:
		bool OpenFile(FILE  *  fp);
		void SaveFile(FILE  *  fp);

	public:
		int       m_nStTemperature;			//起始温度
		int       m_nCount;					//温度数量

		vector<glm::vec2>*   m_pvWaveBand;
	};


	class IRRADIATION_EXPORT CIRRadiate
	{	
	public:
		CIRRadiate(void);
		~CIRRadiate(void);
		CIRRadiate(const CIRRadiate &);

	public:
		CIRRadiate & operator =(const CIRRadiate &);

	public:
		bool    Initialize();
		bool    Initialize(CIRRadiateParam param);
		bool    GetRadiation(int index, float T, float & rad);			//根据温度返回辐射值；

	private:

		bool    CheckWaveBand(vector<glm::vec2>  vWaveBand);
		void	CreateArrayData(vector<glm::vec2>  vWaveBand);
		float   RadiationL(float Lambda, float T);
		float   SimpintRadiationL(float a, float b, float step,  float T);

	private:
		CIRRadiateParam  m_Param;
		bool      m_bInit;	
		int       m_nEndTemperature;

		vector<vector<float> >*  m_pWaveData;
	};
};
#endif