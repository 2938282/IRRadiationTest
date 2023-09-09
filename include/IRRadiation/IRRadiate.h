#ifndef __IRRADIATE_H_
#define __IRRADIATE_H_

#include "Export"
#include "IRBasic.h"

//�����������࣬��Ҫ���ڽ����¶���������ȵĹ�ϵ
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
		int       m_nStTemperature;			//��ʼ�¶�
		int       m_nCount;					//�¶�����

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
		bool    GetRadiation(int index, float T, float & rad);			//�����¶ȷ��ط���ֵ��

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