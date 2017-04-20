#pragma once
template<typename _Tp> inline int sign(_Tp val) {
	return 1 - (val <= 0) - (val < 0);
}
