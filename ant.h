#ifndef NOTE_HELP_T1_H_
#define NOTE_HELP_T1_H_
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <functional>
#include <random>
#include <set>

using std::set;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::string;
using std::pair;
using std::default_random_engine;
using std::uniform_int_distribution;

struct Err
{
	string msg;
	Err(const string & m) :msg(m){}
};


class AntColonySystem
{
	typedef int		VIndex;		//������
	typedef int		EIndex;		//�߱��
	typedef int		Weight;		//Ȩ��
	typedef int		FIndex;		//ʳ����
	typedef double	Info;		//��Ϣ��
	typedef double	AInfo;		//��Ϣ����
	typedef double	DisRate;	//��ɢϵ��
	typedef int		AdjIndex;	//�ڽӶ�����

	struct Ant
	{
		unordered_set < VIndex >	_forbiden_table;	//���ɱ�  ����ʼʱ�����յ�
		unordered_set < VIndex >	_food_bag;			//��ǰ�Ѿ���ȡ��ʳ����
		vector	<VIndex>			_path;				//��ǰ���Ͼ�����·��
		VIndex						_loc;				//���ϵ�ǰ����λ��
		AntColonySystem&			_ant_system;		//��Ⱥ��������
		int							_rand_fac;			//���ϵ��������

	public:
		void resetAnt();		//������������
		void moveNext();	//���������ƶ�����һ��λ�ã�����������Ⱥ�ֲ���Ϣ�ĸ���
		int checkAdj();		//����ڽӶ����Ƿ��зǽ��ɵ�ʳ�ﶥ��, ����ʳ�ﶥ�㣬����>=0; ���ޣ�����-1
		void updateFoodBag(VIndex);	//�ߵ��µ�ʳ�ﶥ��ʱ����ʳ�ﶥ��
		int chooseWay();		//����Χ�ǽ��ɵ��ڽӶ����Һ��ʵĶ��㣬��һ����ʧ���� �����ҵ��˺��ʵ��ڽӶ��� ,���� >=0������������ ,���� < 0
		void terminate();		//�����ƶ�����
		
		inline bool isForbidden(VIndex); //������ڽӶ����Ƿ��ǽ��ɵ�
		bool isMistake();

		Ant(AntColonySystem & s) :_ant_system(s)
		{
			//��̬���������
			static default_random_engine e;
			static uniform_int_distribution<int> u(0, 7); 
			_rand_fac = u(e);
			resetAnt();
		}
	};

public:

	struct EInfo
	{
		Info	_info;			//��Ϣ��
		AInfo	_ainfo;			//��Ϣ����

		EInfo(AInfo ainfo = 0, Info info = 0.0) :_ainfo(ainfo), _info(info){}
	};

	struct AdjEdg
	{
		EIndex _adj_e_index;	//�ڽӱߵı߱��
		Weight _adj_e_weight;	//�ڽӱߵ�Ȩ��
		unordered_map<FIndex, EInfo>	_adj_e_ftable;	//�ڽӱߵ�ʳ����Ϣ��
		EInfo	_adj_e_home;	//�ڽӱߵļ���Ϣ

		AdjEdg(EIndex index = 0, Weight weight = 0) :_adj_e_index(index), _adj_e_weight(weight){ _adj_e_home._ainfo = 1.0 / _adj_e_weight; }
	};

private:
	void loadInfo(const string& f1, const string& f2);		//����ͼ��Ϣ������ʳ����Ϣ

public:
	typedef unordered_map<AdjIndex, AdjEdg> AdjSet;
	
	unordered_map<VIndex, AdjSet> _graph;	//ͼ
	unordered_set<VIndex> _food;			//�ع�����
	DisRate		_frate;						//ʳ����Ϣ��ɢ��
	DisRate		_hrate;						//����Ϣ��ɢ��
	size_t		_ants_num;					//��������
	vector<Ant> _ants;						//���ϼ�Ⱥ
	size_t		_steps_num;					//����
	VIndex		_src;						//��㶥���
	VIndex		_dest;						//�յ㶥���
	vector<VIndex> _best_path;				//���ŵ�·��
	double		_mrate;						//���Ϸ�������


public:
	AntColonySystem(const string& f1, const string & f2, double frate, double hrate, double mis_rate, size_t ants_num, size_t steps_num)  //����˳��ʳ����Ϣ����ɢ�ʣ�����Ϣ����ɢ�ʣ����Ϸ������ʣ����������������
		:_frate(frate), _hrate(hrate), _mrate(mis_rate)
		, _ants_num(ants_num), _steps_num(steps_num)
		,_ants(_ants_num, *this)       //��ʼ����������
	{ loadInfo(f1, f2); }
	
	double calInfo(Ant&, AdjIndex);			//����������Ϣ����������ϵ��ڽӶ������Ϣ���ܺ�
	void localUpdate(Ant&, AdjIndex);		//��������ѡ��Ķ��㣬���¸ñߵ���Ϣ��
	void activeFood(Ant&, FIndex);			//�ҵ�ʳ������ϣ�����������·���ϵ�ʳ������
	void init();							//��ʼ����������

};


#endif