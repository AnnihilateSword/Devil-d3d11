#pragma once

#include <DirectXMath.h>

inline DirectX::XMMATRIX XM_CALLCONV InverseTranspose(DirectX::FXMMATRIX M)
{
	using namespace DirectX;

	// The transpose of the inverse of the world matrix is only for the normal vector, 
	// and we do not need the translation component of the world matrix
	// And if you do not remove it, the subsequent multiplication of 
	// the observation matrix will produce the wrong transformation result
	XMMATRIX A = M;
	A.r[3] = g_XMIdentityR3;

	return XMMatrixTranspose(XMMatrixInverse(nullptr, A));
}