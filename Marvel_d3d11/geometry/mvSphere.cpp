#include "mvSphere.h"
#include <DirectXMath.h>
#include <cmath>

constexpr float PI = 3.14159265f;

namespace Marvel
{

	static void AddVertex(std::vector<float>& verticies, float x, float y, float z)
	{
		verticies.push_back(x);
		verticies.push_back(y);
		verticies.push_back(z);
	}

	static void AddIndex(std::vector<unsigned short>& indicies, unsigned short x, unsigned short y, unsigned short z)
	{
		indicies.push_back(x);
		indicies.push_back(y);
		indicies.push_back(z);
	}

	void mvSphere::Create(std::vector<float>& verticies, std::vector<unsigned short>& indicies)
	{

		verticies.clear();
		indicies.clear();

		int latDiv = 5;
		int longDiv = 5;

		constexpr float radius = 1.0f;
		const auto base = DirectX::XMVectorSet(0.0f, 0.0f, radius, 0.0f);
		const float lattitudeAngle = PI / latDiv;
		const float longitudeAngle = 2.0f * PI / longDiv;

		for (int iLat = 1; iLat < latDiv; iLat++)
		{
			const auto latBase = DirectX::XMVector3Transform(
				base,
				DirectX::XMMatrixRotationX(lattitudeAngle * iLat)
			);
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				DirectX::XMFLOAT3 calculatedPos;
				auto v = DirectX::XMVector3Transform(
					latBase,
					DirectX::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				DirectX::XMStoreFloat3(&calculatedPos, v);
				AddVertex(verticies, calculatedPos.x, calculatedPos.y, calculatedPos.z);
			}
		}

		// add the cap vertices
		const auto iNorthPole = (unsigned short)verticies.size();
		{
			DirectX::XMFLOAT3 northPos;
			DirectX::XMStoreFloat3(&northPos, base);
			AddVertex(verticies, northPos.x, northPos.y, northPos.z);
		}
		const auto iSouthPole = (unsigned short)verticies.size();
		{
			DirectX::XMFLOAT3 southPos;
			DirectX::XMStoreFloat3(&southPos, DirectX::XMVectorNegate(base));
			AddVertex(verticies, southPos.x, southPos.y, southPos.z);
		}

		const auto calcIdx = [latDiv, longDiv](unsigned short iLat, unsigned short iLong)
		{ return iLat * longDiv + iLong; };
		for (unsigned short iLat = 0; iLat < latDiv - 2; iLat++)
		{
			for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
			{
				indicies.push_back(calcIdx(iLat, iLong));
				indicies.push_back(calcIdx(iLat + 1, iLong));
				indicies.push_back(calcIdx(iLat, iLong + 1));
				indicies.push_back(calcIdx(iLat, iLong + 1));
				indicies.push_back(calcIdx(iLat + 1, iLong));
				indicies.push_back(calcIdx(iLat + 1, iLong + 1));
			}
			// wrap band
			indicies.push_back(calcIdx(iLat, longDiv - 1));
			indicies.push_back(calcIdx(iLat + 1, longDiv - 1));
			indicies.push_back(calcIdx(iLat, 0));
			indicies.push_back(calcIdx(iLat, 0));
			indicies.push_back(calcIdx(iLat + 1, longDiv - 1));
			indicies.push_back(calcIdx(iLat + 1, 0));
		}

		// cap fans
		for (unsigned short iLong = 0; iLong < longDiv - 1; iLong++)
		{
			// north
			indicies.push_back(iNorthPole);
			indicies.push_back(calcIdx(0, iLong));
			indicies.push_back(calcIdx(0, iLong + 1));
			// south
			indicies.push_back(calcIdx(latDiv - 2, iLong + 1));
			indicies.push_back(calcIdx(latDiv - 2, iLong));
			indicies.push_back(iSouthPole);
		}
		// wrap triangles
		// north
		indicies.push_back(iNorthPole);
		indicies.push_back(calcIdx(0, longDiv - 1));
		indicies.push_back(calcIdx(0, 0));
		// south
		indicies.push_back(calcIdx(latDiv - 2, 0));
		indicies.push_back(calcIdx(latDiv - 2, longDiv - 1));
		indicies.push_back(iSouthPole);

	}

}