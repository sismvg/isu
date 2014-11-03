#ifndef ISU_ALG_MORE_FOR_EACH
#define ISU_ALG_MORE_FOR_EACH

//COMPLETE ME
	//for_each_if

#include <algorithm>

namespace isu
{
	namespace alg
	{
		template<class ForwardIterator,class Func>
		void for_each_if(ForwardIterator begin, ForwardIterator end,Func fn)
		{
			for (; begin != end; ++begin)
			{
				if (!fn(*begin)) break;
			}
		}

		template<class IndexType,class Func>
		void index_each(IndexType begin, IndexType end, Func fn)
		{
			for (; begin != end; ++begin)
				fn(begin);
		}

		template<class IndexType,class AdvanceType,class Func>
		void index_each(IndexType begin, IndexType end, AdvanceType adv, Func fn)
		{
			for (; begin != end; begin += adv)
				fn(begin);
		}

	}
}
#endif