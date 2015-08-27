#pragma once
#include "stdfax.h"

using namespace std;

namespace util
{
	template <typename K, typename V>
	class safemap
	{
	public:
		safemap() 
		{ 
			top = 0; 
		}
		
		~safemap() {}
		
		V* get(K key) 
		{ 
			return &stdmap[key]; 
		}

		V* getnext(int i)
		{
			if (i > top)
			{
				return 0;
			}
			else
			{
				K key = keys[i];
				return &stdmap[key];
			}
		}

		int getend() 
		{ 
			return top; 
		}

		bool contains(K k) 
		{ 
			return stdmap.count(k) > 0; 
		}

		void add(K key, V value)
		{
			if (stdmap.count(key) == 0)
			{
				stdmap[key] = value;
				keys.push_back(key);
				top += 1;
			}
		}

		void remove(int i)
		{
			if (top > 0)
			{
				top -= 1;
				stdmap.erase(keys[i]);
				keys.erase(keys.begin() + i);
			}
		}

		void clear()
		{
			top = 0;
			stdmap.clear();
			keys.clear();
		}
	private:
		vector<K> keys;
		map<K, V> stdmap;
		int top;
	};
}

