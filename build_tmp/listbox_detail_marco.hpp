#ifndef ISU_LISTBOX_DETAIL_MARCO_HPP
#define ISU_LISTBOX_DETAIL_MARCO_HPP
#define CON_DEF(c)\
	typedef typename c::size_type size_type; \
	typedef typename c::value_type value_type; \
	typedef typename c::iterator iterator; \
	typedef typename c::const_iterator const_iterator;
#define DIS_FN(name) this->container()->name
#define DIS_REFRESH_FN(name,refresh) auto proxy=_make_refresh_proxy(refresh);\
	return DIS_FN(name)
#endif