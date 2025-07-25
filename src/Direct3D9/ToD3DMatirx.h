#ifndef TOD3DMATRIX_H
#define TOD3DMATRIX_H

#include <d3d9.h>
#include "../MD_Math.h"

D3DMATRIX ToD3DMATRIX(MD_Math::MATRIX m)
{
	D3DMATRIX temp;
	temp._11 = m._11;
	temp._21 = m._12;
	temp._31 = m._13;
	temp._41 = m._14;

	temp._12 = m._21;
	temp._22 = m._22;
	temp._32 = m._23;
	temp._42 = m._24;

	temp._13 = m._31;
	temp._23 = m._32;
	temp._33 = m._33;
	temp._43 = m._34;

	temp._14 = m._41;
	temp._24 = m._42;
	temp._34 = m._43;
	temp._44 = m._44;

	return temp;
}

#endif // !TOD3DMATRIX_H


