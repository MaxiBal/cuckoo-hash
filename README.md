# Cuckoo Hash Table

A simple, c++ cuckoo hash table implementation

## Usage

```c++
typedef int hash_item_type;

cuckoo_hash_table<hash_item_type> table;

std::function<int(hash_item_type)> hashing_functions[] =
{
	[](int x) { return x % TABLE_MAX_LEN; },
	[](int x) { return (x / TABLE_MAX_LEN) % TABLE_MAX_LEN; }
};

table.set_hashing_functions(hashing_functions);

table.add(20);
table.add(40);

std::cout << table.is_in(20) << std::endl;
```  
See example.cpp for more usage.
