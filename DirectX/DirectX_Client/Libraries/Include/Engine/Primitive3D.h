#pragma once

//************
// Point3D
//************
// 3차원 점을 나타내는 별칭입니다. Vec3 타입을 사용합니다.
using Point3D = Vec3;

//************
// Line3D
//************

// 선(Line)을 3차원 공간에서 표현합니다. 시작점과 끝점을 갖습니다.
struct Line3D
{
	Point3D start = Point3D(0.f); // 시작점, 기본값은 원점입니다.
	Point3D end = Point3D(0.f); // 끝점, 기본값 역시 원점입니다.

	float Length() { return Vec3::Distance(start, end); } // 선의 길이를 계산합니다.
	float LengthSq() { return Vec3::DistanceSquared(start, end); } // 선의 길이의 제곱을 계산합니다. 계산 비용을 줄이기 위해 사용됩니다.
};

//************
// Ray3D
//************

// 광선(Ray)을 3차원 공간에서 표현합니다. 원점과 방향 벡터를 갖습니다.
struct Ray3D
{
	Point3D origin = Point3D(0.f); // 광선의 시작점, 기본값은 원점입니다.
	Vec3 direction = Vec3(0.f); // 광선의 방향, 기본값은 (0,0,0)입니다.

	void NormalizeDirection() { direction.Normalize(); } // 방향 벡터를 정규화합니다.
	static Ray3D FromPoints(const Point3D& from, const Point3D& to) { return Ray3D{ from, to - from }; } // 두 점으로부터 광선을 생성합니다.
};


//************
// Sphere3D
//************

// 구(Sphere)를 3차원 공간에서 표현합니다. 중심 위치와 반지름을 갖습니다.
struct Sphere3D
{
	Point3D position; // 구의 중심 위치입니다.
	float radius; // 구의 반지름입니다.
};

//************
// AABB3D
//************

// 축에 정렬된 경계 상자(Axis-Aligned Bounding Box, AABB)를 3차원 공간에서 표현합니다.
struct AABB3D
{
	Point3D position = Vec3(0.f); // AABB의 중심 위치입니다.
	Vec3 size = Vec3(1.f, 1.f, 1.f); // AABB의 크기(각 축에 대한 반 길이)입니다.

	// AABB의 최소 좌표를 계산합니다.
	static Vec3 GetMin(const AABB3D& aabb) {
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;
		return Vec3(fminf(p1.x, p2.x), fminf(p1.y, p2.y), fminf(p1.z, p2.z));
	}

	// AABB의 최대 좌표를 계산합니다.
	static Vec3 GetMax(const AABB3D& aabb) {
		Vec3 p1 = aabb.position + aabb.size;
		Vec3 p2 = aabb.position - aabb.size;
		return Vec3(fmaxf(p1.x, p2.x), fmaxf(p1.y, p2.y), fmaxf(p1.z, p2.z));
	}

	// 최소 및 최대 좌표를 이용하여 AABB를 생성합니다.
	static AABB3D FromMinMax(const Vec3& min, const Vec3& max) {
		return AABB3D{ (min + max) / 2, (max - min) / 2 };
	}
};


//*********
// OBB3D
//*********

// 방향이 있는 경계 상자(Oriented Bounding Box, OBB)를 3차원 공간에서 표현합니다.
struct OBB3D
{
	Point3D position = Vec3(0.f); // OBB의 중심 위치입니다.
	Vec3 size = Vec3(1.f, 1.f, 1.f); // OBB의 크기입니다.
	Matrix orientation; // OBB의 방향을 나타내는 변환 행렬입니다.
	//Vec4 quaternion;
	//Vec3 rotation => 짐벌락 이슈
};

//***********
// Plane3D
//***********
//삼각형 방식
//노멀 정점 1개
//노멀 정점 3개

// 평면(Plane)을 3차원 공간에서 표현합니다. 법선 벡터와 원점으로부터의 거리를 갖습니다.
struct Plane3D
{
	Vec3 normal; // 평면의 법선 벡터입니다.
	float distance; // 원점으로부터의 거리입니다.
};


//*************
//Triangle3D
//*************

// 삼각형(Triangle)을 3차원 공간에서 표현합니다. 세 개의 꼭짓점을 갖습니다.
struct Triangle3D
{
	union
	{
		struct
		{
			Point3D a; // 첫 번째 꼭짓점입니다.
			Point3D b; // 두 번째 꼭짓점입니다.
			Point3D c; // 세 번째 꼭짓점입니다.
		};
		Point3D points[3]; // 꼭짓점 배열입니다.
		float values[9]; // 꼭짓점 좌표 값 배열입니다.
	};
};

// 구간(Interval)을 3차원 공간에서 표현합니다. 최소 및 최대 값을 갖습니다.
struct Interval3D
{
	float min; // 구간의 최소값입니다.
	float max; // 구간의 최대값입니다.
};