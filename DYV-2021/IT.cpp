#include "Error.h"
#include "FST.h"
#include "LT.h"
#include "IT.h"
#include "LitT.h"
#include <string>

namespace IT
{
	IdTable Create(int size)
	{
		IdTable idtable;
		idtable.maxsize = size;
		idtable.size = 0;
		idtable.table = new PropTable[size];
		return idtable;
	}

	int Add(IdTable& idtable, PropTable entry)
	{
		std::string buff = "";
		for (int i = 0; entry.string[i] != '\0'; ++i)
			buff += entry.string[i];
		if ((buff == "\"integer\"") || (buff == "\"string\"") || (buff == "\"doublenum\"") || (buff == "\"hexinteger\"") || (buff == "\"name\"") || (buff == "\"propertycounter\""))
			return -1;
		if ((buff == "\"integerdefault\"") || (buff == "\"stringdefault\"") || (buff == "\"doubledefault\"") || (buff == "\"start\"") || (buff == "\"return\""))
			return -1;
		++idtable.size;
		if (idtable.size > idtable.maxsize)
			ERROR_THROW(110);
		idtable.table[idtable.size - 1] = entry;
		return 0;
	}
	/*void Add(ObjTable& objtable, PropTable entry)
	{
		++objtable.propertycounter;
		if (objtable.propertycounter > PROP_T_MAXSIZE)
			ERROR_THROW(111);
		objtable.table[objtable.propertycounter - START_COUNTER] = entry;
		return;
	}*/
	int Add(PropTable& proptable, PropTable& entry)
	{
		std::string buff = "";
		for (int i = 0; entry.string[i] != '\0'; ++i)
			buff += entry.string[i];
		if ((buff == "\"integer\"") || (buff =="\"string\"") || (buff == "\"doublenum\"") || (buff == "\"hexinteger\"") || (buff == "\"name\"") || (buff == "\"propertycounter\""))
			return -1;
		if ((buff == "\"integerdefault\"") || (buff == "\"stringdefault\"") || (buff == "\"doubledefault\"") || (buff == "\"start\"") || (buff == "\"return\""))
			return -1;
		++proptable.propertycounter;
		if (proptable.propertycounter > PROP_T_MAXSIZE)
			ERROR_THROW(111);
		//proptable.table = new PropTable[PROP_T_MAXSIZE];
		proptable.table[proptable.propertycounter - START_COUNTER - 1] = entry;
		return 0;
	}

	PropTable GetEntry(IdTable& idtable, int n)
	{
		if ((n > idtable.size) || (n < 0))
			ERROR_THROW(112);
		return idtable.table[n - 1];
	}

	int IsObj(IdTable* idtable, char* id, int len)
	{
		bool ohmy = true;
		for (int i = 0; i < idtable->size; ++i)
		{
			for (int j = 0; j < len; ++j)
				if (idtable->table[i].name[j] != id[j])
				{
					ohmy = false;
					break;
				}
			if (ohmy)
			{
				return i + 1;
			}
			ohmy = true;
		}
		return TI_NULLIDX;
	}
	/*int IsObjWithProp(IdTable& idtable, char* id, int len)
	{
		bool ohmy = true;
		for (int i = 0; i < idtable.size; ++i)
		{
			for (int k = 7; k < idtable.table[i].propertycounter; ++k)
			{
				for (int j = 0; j < len; ++j)
					if (idtable.table[i].table[k - START_COUNTER - 1].name[j] != id[j])
					{
						ohmy = false;
						break;
					}
				if (ohmy)
				{
					if ((id != "string") && (id != "doublenum") && (id != "integer") && (id != "propertycounter") && (id != "hexinteger") && (id != "name") && (id != "integerdefault") && (id != "stringdefault") && (id != "doubledefault"))
						return i + 1;
					else
						ERROR_THROW(123);
				}
			}
			ohmy = true;
		}
		return TI_NULLIDX;
	}*/
	/*int IsProp(ObjTable* objtable, char* id, int len)
	{
		bool ohmy = true;
		for (int i = 0; i < objtable->propertycounter - START_COUNTER; ++i)
		{
			for (int j = 0; j < len; ++j)
				if (objtable->table[i].name[j] != id[j])
				{
					ohmy = false;
					break;
				}
			if (ohmy)
			{
				return i + 1;
			}
			ohmy = true;
		}
		if ((id == "integer") || (id == "string") || (id == "doublenum") || (id == "hexinteger") || (id == "name") || (id == "propertycounter"))
			return -1;
		if ((id == "integerdefault") || (id == "stringdefault") || (id == "doubledefault"))
			return -10;
		return TI_NULLIDX;
	}*/
	int IsProp(PropTable& proptable, char* id, int len)
	{
		bool ohmy = true;
		for (int i = 0; i <= ((proptable.propertycounter) - START_COUNTER - 1); ++i)
		{
			for (int j = 0; (j < len)&&(proptable.table[i].name[j] != '\0')&&(id[j] != '\0'); ++j)
				if (proptable.table[i].name[j] != id[j])
				{
					ohmy = false;
					break;
				}
			if (ohmy)
			{
				return i + 1;
			}
			ohmy = true;
		}
		if ((id == "integer") || (id == "string") || (id == "doublenum") || (id == "hexinteger") || (id == "name") || (id == "propertycounter") || (id == "start") || (id == "return"))
			return -1;
		if ((id == "integerdefault") || (id == "stringdefault") || (id == "doubledefault"))
			return -10;
		return TI_NULLIDX;
	}
	/*int IsId(IdTable& idtable, std::string id, int len)
	{
		char* buffId = new char[id.length() + 1];
		for (int i = 0; ((i < ID_MAXSIZE)&&(i < len)); ++i)
			buffId[i] = id[i];
		buffId[len] = id[id.length()];
		bool ohmy = true;
		for (int i = 0; i < idtable.size; ++i)
		{
			for (int j = 0; j < len; ++j)
				if (idtable.table[i].id[j] != buffId[j])
				{
					ohmy = false;
					break;
				}
			if (ohmy)
			{
				return i + 1;
			}
			ohmy = true;
		}
		return TI_NULLIDX;
	}*/

	void Delete(IdTable& idtable)
	{
		delete[] idtable.table;
		return;
	}

	/*void ShowAll(IdTable& idtable)
	{
		for (int i = 0; i < idtable.size; ++i)
		{
			printf("%5s%4i%2i%2i\n", idtable.table[i].id, idtable.table[i].idxfirstLE, idtable.table[i].idtype, idtable.table[i].iddatatype);

		}
			printf("\n");
		return;
	}*/
	bool WasUsed(IT::IdTable& idtable, char* str, int pos)
	{
		std::string buff = std::string(str);
		for (int i = 0; i < pos; ++i)
		{
			if (std::string(idtable.table[i].name) == buff)
				return true;
		}
		return false;
	}
	IdTable Smooth(IdTable& idtable)
	{
		IdTable newid = Create(65536);
		for (int i = 0; i < idtable.size; ++i)
		{
			if ((!WasUsed(newid, idtable.table[i].name, newid.size))&&(
				(std::string(idtable.table[i].name) != std::string("string")) && (std::string(idtable.table[i].name) != std::string("integer"))&& (std::string(idtable.table[i].name) != std::string("doublenum"))&&
				(std::string(idtable.table[i].name) != std::string("hexinteger"))&& (std::string(idtable.table[i].name) != std::string("name"))&& (std::string(idtable.table[i].name) != std::string("propertycounter"))&&
				(std::string(idtable.table[i].name) != std::string("integerdefault"))&& (std::string(idtable.table[i].name) != std::string("stringdefault"))&& (std::string(idtable.table[i].name) != std::string("doubledefault"))&&
				(std::string(idtable.table[i].name) != std::string("table"))&&(std::string(idtable.table[i].name) != std::string("start"))&& (std::string(idtable.table[i].name) != std::string("return"))&&
				(std::string(idtable.table[i].name) != std::string("check")) && (std::string(idtable.table[i].name) != std::string("equals")) && (std::string(idtable.table[i].name) != std::string("_"))&&
				(std::string(idtable.table[i].name) != std::string("loop")) && (std::string(idtable.table[i].name) != std::string("add")) && (std::string(idtable.table[i].name) != std::string("append"))&&
				(std::string(idtable.table[i].name) != std::string("print"))))
				Add(newid, idtable.table[i]);
		}
		return newid;
	}

	void WriteIT(IdTable& idtable)
	{
		std::ofstream stream;//заношу в лог таблицы лексем номер первой строки
		remove("IdT.log");
		stream.open("IdT.log", std::ios_base::app);
		if (!stream.is_open())
		{
			ERROR_THROW(5)
		}
		for (int i = 0; i < idtable.size; ++i)
			stream << "	" << idtable.table[i].name << " | " << idtable.table[i].integer << " | " << idtable.table[i].string << "\n";
		stream.close();
	}
}