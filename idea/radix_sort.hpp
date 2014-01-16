using std::size_t;
	template<size_t Hexadecimal>
	class _dix_of
	{
	public:
		template<size_t Dix>
		static size_t dix(size_t val)
		{
			return (val / static_pow<Hexadecimal, Dix - 1>::value) % Hexadecimal;
		}
	};

	template<>
	class _dix_of<16>
	{
	public:
		template<size_t Dix>
		static size_t dix(size_t val)
		{
			return (val >> 4 * (Dix - 1)) & 0xF;
		}
	};
	template<size_t Hexadecimal,size_t Dix>
	size_t dix_of(size_t val)
	{
		static_assert(Dix != 0, "Dix can not zero");
		return _dix_of<Hexadecimal>::dix<Dix>(val);
	}

	template<size_t Hexadecimal,size_t Dix,class Iterator>
	void count_sort(Iterator begin, Iterator end)
	{
		typedef typename value_type<Iterator>::type value_type;
		std::array<std::vector<value_type>, Hexadecimal> but;
		std::for_each(begin, end, [&](const value_type& val)
		{
			but[dix_of<Hexadecimal,Dix>(val)].push_back(val);
		});
		for (size_t index = 0; index != Hexadecimal; ++index)
			begin = std::copy(but[index].begin(), but[index].end(), begin);
	}

	template<class Iterator,size_t Hexadecimal,size_t NowDix>
	void radix_sort_impl(Iterator begin, Iterator end, std::false_type)
	{
	}

	template<class Iterator,size_t Hexadecimal,size_t NowDix>
	void radix_sort_impl(Iterator begin, Iterator end, std::true_type)
	{
		count_sort<Hexadecimal, NowDix>(begin, end);
		radix_sort_impl<Iterator, Hexadecimal, NowDix + 1>
			(begin, end, std::integral_constant<bool, NowDix !=sizeof(size_t)*2>() );
	}
	template<class Iterator,size_t Hexadecimal=16>
	void radix_sort(Iterator begin, Iterator end)
	{
		radix_sort_impl<Iterator, Hexadecimal, 1>(begin, end, std::true_type() );
	}
