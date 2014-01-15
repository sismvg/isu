template<std::size_t numic,std::size_t pw>
struct static_pow
{
	static const std::size_t value = static_pow<numic, pw - 1>::value*numic;
};

template<std::size_t numic>
struct static_pow<numic,0>
{
	static const std::size_t value = 1;
};

template<std::size_t Dix>
std::size_t dix_of(std::size_t num)
{
	static_assert(Dix != 0, "the Dix have to much to 0");
	//Dix必须是1开始，如果10^Dix-1>num则返回dix_too_big
	return (num /static_pow<10,Dix-1>::value) % 10;
}

template<std::size_t Dix,std::size_t pos,class Iterator>
void count_sort(Iterator begin, Iterator end)
{
	typedef typename value_type<Iterator>::type value_type;
	std::array<std::vector<value_type>, Dix> but;
	std::for_each(begin, end, [&](const value_type& val)
	{
		but[dix_of<pos>(val)].push_back(val);
	});
	for (std::size_t index = 0; index != Dix; ++index)
		begin = std::copy(but[index].begin(), but[index].end(), begin);
}

using std::size_t;
template<
	class Iterator,size_t Hexadecimal,
	size_t NowDix,size_t MaxDix>
void radix_sort_impl(Iterator begin, Iterator end, std::false_type)
{}

template<
	class Iterator, size_t Hexadecimal,
	size_t NowDix, size_t MaxDix>
void radix_sort_impl(Iterator begin, Iterator end,std::true_type)
{
	count_sort<10, NowDix>(begin, end);
	radix_sort_impl<Hexadecimal,NowDix+1, MaxDix>
		(begin, end, 
		std::integral_constant<bool,NowDix!=MaxDix>());
}

template<
	class Iterator, size_t Hexadecimal,
	size_t NowDix, size_t MaxDix>
void radix_sort(Iterator begin,Iterator end)
{
		radix_sort_impl<Hexadecimal,1,MaxDix>(begin, end,std::true_type());
}
