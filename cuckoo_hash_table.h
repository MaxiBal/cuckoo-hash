#pragma once

#ifndef TABLE_MAX_LEN
#define TABLE_MAX_LEN 20
#endif

#ifndef TABLE_COL_LEN
#define TABLE_COL_LEN 2
#endif

#define MAX_RECURSION_DEPTH 1000

#include <functional>
#include <exception>
#include <array>

namespace cuckoo_hash
{
template<typename T>
class cuckoo_hash_table
{
	const typedef T& item_type;

public:

	cuckoo_hash_table() : size_(0) { fill_table(); }
	cuckoo_hash_table(const cuckoo_hash_table& other) = delete;

	void add(item_type item)
	{
		place(item, 0, 0);
		++size_;
	}

	void set_hashing_functions(std::function<int(T)> funcs[])
	{
		// copy from funcs until the hashing functions are filled
		for (unsigned i = 0; i < TABLE_COL_LEN; i++)
		{
			hashing_functions[i] = funcs[i];
		}
	}
	
	bool is_in(item_type item)
	{
		return index_of(item) != -1;
	}

	void remove(item_type item)
	{
		const int id_in_table = index_of(item);
		table[id_in_table][hashing_functions[id_in_table](item)] = default_item;
	}

	size_t size()
	{
		return size_;
	}

	cuckoo_hash_table& operator=(const cuckoo_hash_table& other) = delete;
	T operator ()(const size_t col, const size_t row)
	{
		return table[col][row];
	}

private:
	/// <summary>
	/// Places the item into the table
	/// </summary>
	/// <param name="item">the item to place in the table</param>
	/// <param name="table_id">the column of the table</param>
	/// <param name="cnt">the number of times the table has recursed</param>
	void place(item_type item, int table_id, int cnt)
	{
		if (cnt >= MAX_RECURSION_DEPTH)
		{
			throw std::exception("Item was unable to be placed in the table. Re-hash.");
		}

		const int item_hash = hash(table_id, item);

		if (item_hash >= TABLE_MAX_LEN || item_hash < 0)
		{
			throw std::exception("Hash generated doesn't fit the range of the hash table.");
		}

		if (table[table_id][item_hash] != default_item)
		{
			int new_table_id = (table_id + 1) % TABLE_COL_LEN;

			int table_item = table[table_id][item_hash];

			table[table_id][item_hash] = item;

			place(table_item, new_table_id, cnt + 1);
		}

		table[table_id][item_hash] = item;
	}

	int index_of(item_type item) const noexcept // returns the table column of the item
	{
		for (unsigned i = 0; i < TABLE_COL_LEN; i++)
		{
			int hash = hashing_functions[i](item);

			if (table[i][hash] == item) return i;
		}
		return -1;
	}

	int constexpr hash(int function, T item) const
	{
		return hashing_functions[function](item);
	}

	void fill_table()
	{
		for (unsigned i = 0; i < TABLE_COL_LEN; i++)
		{
			for (unsigned j = 0; j < TABLE_MAX_LEN; j++)
			{
				table[i][j] = default_item;
			}
		}
	}

	size_t size_;
	std::function<int(T)> hashing_functions[TABLE_COL_LEN];
	T table[TABLE_COL_LEN][TABLE_MAX_LEN];
	item_type default_item = T();
}; // cuckoo_hash_table

}; // namespace