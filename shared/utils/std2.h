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
	
	/**
		Does the equivalent of std::remove_if but also executes onRemove/onPreserve after the element has been processed.
	 */
	template<typename T, typename L, typename P, typename N>
	auto removeAndExecute(T& iterable, L&& removeCondition, P&& onRemove, N&& onPreserve)
	{
		auto cursor = iterable.begin();
		auto last = iterable.end();
		cursor = std::find_if(cursor, last, removeCondition);
		if (cursor != last)
		{
			auto removeFrom = cursor;
			onRemove(*cursor);
			++cursor;
			for (; cursor != last; ++cursor)
			{
				if (!removeCondition(*cursor))
				{
					*removeFrom = std::move(*cursor);
					onPreserve(*removeFrom);
					++removeFrom;
				}
				else
				{
					onRemove(*cursor);
				}					
			}
			return removeFrom;
		}
		return last;
	}
}