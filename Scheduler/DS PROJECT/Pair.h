#ifndef PAIR_
#define PAIR_
template <class T, class S>
class Pair
{
private:
	T first;
	S second;
public:
	Pair(T one = 0, S two = 0) :first(one), second(two)
	{}
	void setfirst(T one)
	{
		first = one;
	}
	T getfirst()const
	{
		return first;
	}
	void setsecond(S two)
	{
		second = two;
	}
	S getsecond()const
	{
		return second;
	}
	void operator=(const Pair<T, S>& p)
	{
		first = p.first;
		second = p.second;
	}
	bool operator==(const Pair<T, S>& p2)const
	{
		return second == p2.second;
	}
	bool operator>(const Pair<T, S>& p2) const
	{
		return second > p2.second;
	}
	bool operator<(const Pair<T, S>& p2) const
	{
		return second < p2.second;
	}
	bool operator<=(const Pair<T, S>& p2)const
	{
		return second <= p2.second;
	}
	bool operator>=(const Pair<T, S>& p2)const
	{
		return second >= p2.second;
	}

};
#endif

