#include "framework.h"
#include "Dijkstra.h"

int Di::GetNode(Vector3 pos)
{
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if ((it->second->pos - pos).Length() < 4)
		{
			return it->first;
		}
	}
	return 0;
}

DiNode*& Di::AddNode(Vector3 pos)
{
	int number = 1;
	for (auto it = list.begin(); it != list.end(); it++)
	{
		if (list.find(number) == list.end())
		{
			break;
		}
		number++;
	}
	DiNode* temp = new DiNode(number);
	temp->pos = pos;
	list[number] = temp;
	return temp;
}

bool Di::PopNode(int str)
{
	auto it = list.find(str);
	if (it != list.end())
	{
		for (auto it2 = it->second->linkedList.begin();
			it2 != it->second->linkedList.end(); it2++)
		{
			list[it2->first]->linkedList.erase(str);
		}


		delete it->second;
		list.erase(str);
		return true;
	}
	return false;
}

void Di::LinkNode(int str1, int str2)
{
	if (str1 == str2) return;

	Vector3 temp = list[str1]->pos - list[str2]->pos;
	float dis = temp.Length();
	list[str1]->Link(str2, dis);
	list[str2]->Link(str1, dis);
}

bool Di::PathFinding(int from, int to, std::list<Vector3>& way)
{
	//노드비용을 초기화
	for (auto it = list.begin(); it != list.end(); it++)
	{
		it->second->Reset();
	}
	//우선순위큐
	priority_queue<DiNode*, vector<DiNode*>, DiNodeCompare>
		Searchlist;

	Searchlist.push(list[from]);
	list[from]->cost = 0;
	list[from]->find = true;

	while (not Searchlist.empty())
	{
		DiNode* top = Searchlist.top();
		Searchlist.pop();

		for (auto it = top->linkedList.begin();
			it != top->linkedList.end(); it++)
		{
			//기존노드의 비용 갱신
			if (list[it->first]->cost > top->cost + it->second.cost)
			{
				if (it->second.isActive)
				{
					list[it->first]->cost = top->cost + it->second.cost;
					list[it->first]->prev = top->id;
					if (not list[it->first]->find)
					{
						Searchlist.push(list[it->first]);
						list[it->first]->find = true;
					}
				}
			}
		}


	}
	way.clear();
	DiNode* iter = list[to];
	way.push_back(iter->pos);
	while (1)
	{
		if (not iter->prev)
		{
			return false;
		}
		iter = list[iter->prev];
		way.push_back(iter->pos);
		if (iter->id == from)
		{
			break;
		}
	}
	return true;
}

void Di::SaveFile(string file)
{
	this->file = file;
	BinaryWriter out;
	wstring path = L"../Contents/Dijkstra/" + Util::ToWString(file);
	out.Open(path);
	out.UInt(list.size());
	for (auto it = list.begin(); it != list.end(); it++)
	{
		out.UInt(it->second->id);
		out.vector3(it->second->pos);
		out.UInt(it->second->linkedList.size());
		for (auto it2 = it->second->linkedList.begin();
			it2 != it->second->linkedList.end(); it2++)
		{
			out.Int(it2->first);    // id
			out.Float(it2->second.cost); // cost
			out.Bool(it2->second.isActive); // cost
		}
	}
	out.Close();
}

void Di::LoadFile(string file)
{
	this->file = file;
	BinaryReader in;
	wstring path = L"../Contents/Dijkstra/" + Util::ToWString(file);
	in.Open(path);
	list.clear();
	UINT Listsize = in.UInt();
	for (UINT i = 0; i < Listsize; i++)
	{
		DiNode* temp = new DiNode;
		temp->id = in.UInt();
		temp->pos = in.vector3();
		list[temp->id] = temp;
		UINT NodeListsize = in.UInt();
		for (UINT j = 0; j < NodeListsize; j++)
		{
			int id = in.Int();
			float cost = in.Float();
			list[temp->id]->linkedList[id].cost = cost;
			bool isActive = in.Bool();
			list[temp->id]->linkedList[id].isActive = isActive;
		}
	}
	in.Close();

	for (auto it = list.begin(); it != list.end(); it++)
	{
		it->second->diType = DiNode_TYPE::ORIGINAL;
	}

}
