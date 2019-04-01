namespace std2
{
	template<typename T, typename L>
	auto minElement(T& iterable, L&& expression)
	{
		auto result = iterable.begin();
		
		if (result == iterable.end())
			return iterable.end();
			
		auto bestValue = expression(*result);
		
		auto iter = result;
		auto end = iterable.end();
		
		while(++iter != end)
		{
			auto value = expression(*iter);
			if (value < bestValue)
			{
				bestValue = value;
				result = iter;
			}
		}
		return result;
	}
	
	template<typename T, typename L>
	auto maxElement(T& iterable, L&& expression)
	{
		auto result = iterable.begin();
		
		if (result == iterable.end())
			return iterable.end();
			
		auto bestValue = expression(*result);
		
		auto iter = result;
		auto end = iterable.end();
		
		while(++iter != end)
		{
			auto value = expression(*iter);
			if (value > bestValue)
			{
				bestValue = value;
				result = iter;
			}
		}
		return result;
	}
}