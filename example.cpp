#define TABLE_MAX_LEN 11

#include "cuckoo_hash_table.h"

#include <iostream>

using namespace cuckoo_hash;

template<typename T>
void print_table(cuckoo_hash_table<T>& table)
{
	for (int i = 0; i < TABLE_COL_LEN; i++)
	{
		std::cout << "Line no. " << i << ": ";
		for (unsigned j = 0; j < TABLE_MAX_LEN; j++)
		{
			int value = table(i, j);
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	typedef int hash_item_type;

	cuckoo_hash_table<hash_item_type> table;

	std::function<int(hash_item_type)> hashing_functions[] = 
	{ 
		[](int x) { return x % TABLE_MAX_LEN; }, 
		[](int x) { return (x / TABLE_MAX_LEN) % TABLE_MAX_LEN; } 
	};

	table.set_hashing_functions(hashing_functions);

	table.add(20);
	table.add(50);
	table.add(53);
	table.add(75);
	table.add(100);
	table.add(67);
	table.add(105);
	table.add(3);
	table.add(36);
	table.add(39);

	print_table(table);
}