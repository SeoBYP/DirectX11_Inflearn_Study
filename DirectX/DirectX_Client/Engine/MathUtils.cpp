#include "pch.h"
#include "MathUtils.h"
// 점이 구 안에 있는지 판별합니다.
bool MathUtils::PointInSphere(const Point3D& point, const Sphere3D& sphere) {
	float magSq = (point - sphere.position).LengthSquared();
	float radSq = sphere.radius * sphere.radius;
	return magSq <= radSq;
}

// 구에서 주어진 점에 가장 가까운 점을 찾습니다.
Point3D MathUtils::ClosestPoint(const Sphere3D& sphere, const Point3D& point) {
	Vec3 sphereToPointDir = (point - sphere.position);
	sphereToPointDir.Normalize();
	return sphere.position + sphereToPointDir * sphere.radius;
}

// 점이 AABB 안에 있는지 판별합니다.
bool MathUtils::PointInAABB(const Point3D& point, const AABB3D& aabb) {
	Point3D min = AABB3D::GetMin(aabb);
	Point3D max = AABB3D::GetMax(aabb);

	if (point.x < min.x || point.y < min.y || point.z < min.z)
		return false;
	if (point.x > max.x || point.y > max.y || point.z > max.z)
		return false;

	return true;
}

// AABB에서 주어진 점에 가장 가까운 점을 찾습니다.
Point3D MathUtils::ClosestPoint(const AABB3D& aabb, const Point3D& point) {
	Point3D result = point;
	Point3D minPt = AABB3D::GetMin(aabb);
	Point3D maxPt = AABB3D::GetMax(aabb);

	result.x = max(result.x, minPt.x);
	result.y = max(result.y, minPt.y);
	result.z = max(result.z, minPt.z);

	result.x = min(result.x, minPt.x);
	result.y = min(result.y, minPt.y);
	result.z = min(result.z, minPt.z);

	return result;
}

// 점이 OBB 안에 있는지 판별합니다.
bool MathUtils::PointInOBB(const Point3D& point, const OBB3D& obb) {
	Vec3 dir = point - obb.position;

	vector<Vec3> axis = { obb.orientation.Right(), obb.orientation.Up(), obb.orientation.Backward() };
	vector<float> size = { obb.size.x, obb.size.y, obb.size.z };

	for (int i = 0; i < 3; i++) {
		float distance = dir.Dot(axis[i]);

		if (distance > size[i] || distance < -size[i])
			return false;
	}

	return true;
}

// OBB에서 주어진 점에 가장 가까운 점을 찾습니다.
Point3D MathUtils::ClosestPoint(const OBB3D& obb, const Point3D& point) {
	Vec3 dir = point - obb.position;
	Point3D result;
	vector<Vec3> axis = { obb.orientation.Right(), obb.orientation.Up(), obb.orientation.Backward() };
	vector<float> size = { obb.size.x, obb.size.y, obb.size.z };

	for (int i = 0; i < 3; i++) {
		float distance = dir.Dot(axis[i]);

		distance = clamp(distance, -size[i], size[i]);
		result = result + (axis[i] * distance);
	}

	return obb.position + result;
}

// 점이 평면 위에 있는지 판별합니다.
bool MathUtils::PointInPlane(const Point3D& point, const Plane3D& plane) {
	float dot = point.Dot(plane.normal);
	return fabs(dot - plane.distance) < FLT_EPSILON; // 부동소수점 비교 시 EPSILON 사용
}

// 평면에서 주어진 점에 가장 가까운 점을 찾습니다.
Point3D MathUtils::ClosestPoint(const Plane3D& plane, const Point3D& point) {
	float dot = point.Dot(plane.normal);
	float distance = dot - plane.distance;
	return point - plane.normal * distance;
}

// 점이 선 위에 있는지 판별합니다.
bool MathUtils::PointInLine(const Point3D& point, const Line3D& line) {
	Point3D closest = ClosestPoint(line, point);
	return (closest - point).LengthSquared() == 0.f;
}

// 선에서 주어진 점에 가장 가까운 점을 찾습니다.
Point3D MathUtils::ClosestPoint(const Line3D& line, const Point3D& point) {
	Vec3 lineVec = line.end - line.start;
	float t = (point - line.start).Dot(lineVec) / lineVec.Dot(lineVec);
	t = clamp(t, 0.0f, 1.0f); // t 값을 0과 1 사이로 제한
	return line.start + lineVec * t;
}

// 점이 광선 위에 있는지 판별합니다.
bool MathUtils::PointInRay(const Point3D& point, const Ray3D& ray) {
	if (point == ray.origin)
		return true;
	Vec3 norm = point - ray.origin;
	norm.Normalize();
	return fabs(norm.Dot(ray.direction) - 1.0f) < FLT_EPSILON; // 방향 비교 시 EPSILON 사용
}

// 광선에서 주어진 점에 가장 가까운 점을 찾습니다.
Point3D MathUtils::ClosestPoint(const Ray3D& ray, const Point3D& point) {
	float t = (point - ray.origin).Dot(ray.direction);
	t = fmaxf(t, 0.0f); // t가 음수이면 0으로 설정
	return ray.origin + ray.direction * t;
}

// 구와 구의 충돌 검사
bool MathUtils::SphereToSphere(const Sphere3D& s1, const Sphere3D& s2)
{
	float sum = s1.radius + s2.radius; // 두 구의 반지름 합
	float sqDistance = (s1.position - s2.position).LengthSquared(); // 중심 간 거리의 제곱
	return sqDistance <= sum * sum; // 중심 거리의 제곱이 반지름 합의 제곱보다 작거나 같으면 겹침
}

// 구와 AABB의 충돌 검사
bool MathUtils::SphereToAABB(const Sphere3D& sphere, const AABB3D& aabb)
{
	Point3D closestPoint = ClosestPoint(aabb, sphere.position); // AABB 상에서 구의 중심에 가장 가까운 점
	float distSq = (sphere.position - closestPoint).LengthSquared(); // 중심에서 가장 가까운 점까지 거리의 제곱
	float radiusSq = sphere.radius * sphere.radius; // 구의 반지름 제곱
	return distSq < radiusSq; // 중심에서 가장 가까운 점까지의 거리가 반지름보다 작으면 겹침
}

// 구와 OBB의 충돌 검사
bool MathUtils::SphereToOBB(const Sphere3D& sphere, const OBB3D& obb)
{
	Point3D closestPoint = ClosestPoint(obb, sphere.position); // OBB 상에서 구의 중심에 가장 가까운 점
	float distSq = (sphere.position - closestPoint).LengthSquared(); // 중심에서 가장 가까운 점까지 거리의 제곱
	float radiusSq = sphere.radius * sphere.radius; // 구의 반지름 제곱
	return distSq < radiusSq; // 조건 동일
}

// 구와 평면의 충돌 검사
bool MathUtils::SphereToPlane(const Sphere3D& sphere, const Plane3D& plane)
{
	Point3D closestPoint = ClosestPoint(plane, sphere.position); // 평면 상에서 구의 중심에 가장 가까운 점
	float distSq = (sphere.position - closestPoint).LengthSquared(); // 중심에서 가장 가까운 점까지 거리의 제곱
	float radiusSq = sphere.radius * sphere.radius; // 구의 반지름 제곱
	return distSq < radiusSq; // 조건 동일
}

// AABB와 AABB의 충돌 검사
bool MathUtils::AABBToAABB(const AABB3D& aabb1, const AABB3D& aabb2)
{
	Point3D aMin = AABB3D::GetMin(aabb1); // aabb1의 최소 좌표
	Point3D aMax = AABB3D::GetMax(aabb1); // aabb1의 최대 좌표
	Point3D bMin = AABB3D::GetMin(aabb2); // aabb2의 최소 좌표
	Point3D bMax = AABB3D::GetMax(aabb2); // aabb2의 최대 좌표

	return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
		(aMin.y <= bMax.y && aMax.y >= bMin.y) &&
		(aMin.z <= bMax.z && aMax.z >= bMin.z); // 모든 축에 대해 겹치면 true
}

// AABB의 주어진 축에 대한 구간을 계산
Interval3D MathUtils::GetInterval(const AABB3D& aabb, const Vec3& axis)
{
	Vec3 i = AABB3D::GetMin(aabb); // 최소 좌표
	Vec3 a = AABB3D::GetMax(aabb); // 최대 좌표

	Vec3 vertex[8] =
	{
		Vec3(i.x, a.y, a.z), Vec3(i.x, a.y, i.z), Vec3(i.x, i.y, a.z), Vec3(i.x, i.y, i.z),
		Vec3(a.x, a.y, a.z), Vec3(a.x, a.y, i.z), Vec3(a.x, i.y, a.z), Vec3(a.x, i.y, i.z),
	};

	Interval3D result; // 구간 초기화
	result.min = result.max = axis.Dot(vertex[0]); // 첫 정점을 기준으로 초기화

	for (int i = 1; i < 8; ++i)
	{
		float projection = axis.Dot(vertex[i]);
		result.min = min(result.min, projection); // 최소값 갱신
		result.max = max(result.max, projection); // 최대값 갱신
	}
	return result; // 계산된 구간 반환
}

// OBB와 주어진 축에 대한 구간(Interval) 계산
Interval3D MathUtils::GetInterval(const OBB3D& obb, const Vec3& axis)
{
	Vec3 vertex[8]; // OBB의 꼭짓점을 저장할 배열

	Vec3 C = obb.position; // OBB의 중심 위치
	Vec3 E = obb.size; // OBB의 크기(각 축에 대한 반 길이)

	vector<Vec3> A; // OBB의 축
	A.push_back(obb.orientation.Right()); // OBB의 오른쪽 방향 축
	A.push_back(obb.orientation.Up()); // OBB의 위쪽 방향 축
	A.push_back(obb.orientation.Backward()); // OBB의 뒤쪽 방향 축

	// OBB의 8개 꼭짓점 계산
	vertex[0] = C + A[0] * E.x + A[1] * E.y + A[2] * E.z;
	vertex[1] = C - A[0] * E.x + A[1] * E.y + A[2] * E.z;
	vertex[2] = C + A[0] * E.x - A[1] * E.y + A[2] * E.z;
	vertex[3] = C + A[0] * E.x + A[1] * E.y - A[2] * E.z;
	vertex[4] = C - A[0] * E.x - A[1] * E.y - A[2] * E.z;
	vertex[5] = C + A[0] * E.x - A[1] * E.y - A[2] * E.z;
	vertex[6] = C - A[0] * E.x + A[1] * E.y - A[2] * E.z;
	vertex[7] = C - A[0] * E.x - A[1] * E.y + A[2] * E.z;

	// 주어진 축에 대해 OBB의 꼭짓점들을 투영하여 최소/최대 값 계산
	Interval3D result;
	result.min = result.max = axis.Dot(vertex[0]); // 첫 꼭짓점으로 초기화

	for (int i = 1; i < 8; ++i)
	{
		float projection = axis.Dot(vertex[i]); // 꼭짓점을 축에 투영
		result.min = min(result.min, projection); // 최소값 갱신
		result.max = max(result.max, projection); // 최대값 갱신
	}
	return result; // 계산된 구간 반환
}

// AABB와 OBB가 주어진 축에 대해 겹치는지 검사
bool MathUtils::OverlapOnAxis(const AABB3D& aabb, const OBB3D& obb, const Vec3& axis)
{
	Interval3D a = GetInterval(aabb, axis); // AABB의 구간 계산
	Interval3D b = GetInterval(obb, axis); // OBB의 구간 계산
	return ((b.min <= a.max) && (a.min <= b.max)); // 구간이 겹치면 true 반환
}

// 두 OBB가 주어진 축에 대해 겹치는지 검사
bool MathUtils::OverlapOnAxis(const OBB3D& obb1, const OBB3D& obb2, const Vec3& axis)
{
	Interval3D a = GetInterval(obb1, axis); // obb1의 구간 계산
	Interval3D b = GetInterval(obb2, axis); // obb2의 구간 계산
	return ((b.min <= a.max) && (a.min <= b.max)); // 구간이 겹치면 true 반환
}

// AABB와 OBB의 충돌 검사
bool MathUtils::AABBToOBB(const AABB3D& aabb, const OBB3D& obb)
{
	Vec3 test[15] = // 충돌 검사에 사용될 축
	{
		Vec3(1,0,0), // AABB 축 1
		Vec3(0,1,0), // AABB 축 2
		Vec3(0,0,1), // AABB 축 3
		obb.orientation.Right(), // OBB 축 1
		obb.orientation.Up(), // OBB 축 2
		obb.orientation.Backward(), // OBB 축 3
		// 외적으로 생성된 추가 축은 여기에서 계산됨
	};

	// 추가 축 계산 (AABB 축과 OBB 축의 외적)
	for (int i = 0; i < 3; ++i)
	{
		test[6 + i * 3 + 0] = test[i].Cross(test[3]);
		test[6 + i * 3 + 1] = test[i].Cross(test[4]);
		test[6 + i * 3 + 2] = test[i].Cross(test[5]);
	}

	// 모든 축에 대해 겹치는지 검사
	for (int i = 0; i < 15; ++i)
	{
		if (!OverlapOnAxis(aabb, obb, test[i])) // 하나라도 겹치지 않으면 false 반환
			return false;
	}

	return true; // 모두 겹치면 true 반환
}

// AABB와 평면의 충돌 검사
bool MathUtils::AABBToPlane(const AABB3D& aabb, const Plane3D& plane)
{
	float pLen = aabb.size.x * fabsf(plane.normal.x) + // AABB의 각 축에 대한 반응성 계산
		aabb.size.y * fabsf(plane.normal.y) +
		aabb.size.z * fabsf(plane.normal.z);

	float dot = plane.normal.Dot(aabb.position); // 평면의 법선과 AABB 중심의 내적
	float dist = dot - plane.distance; // 평면 상수와의 차이 계산

	return fabsf(dist) <= pLen; // 겹치는지 여부 반환
}

// 두 OBB의 충돌 검사
bool MathUtils::OBBToOBB(const OBB3D& obb1, const OBB3D& obb2)
{
	Vec3 test[15] = // 충돌 검사에 사용될 축
	{
		obb1.orientation.Right(), // OBB1 축 1
		obb1.orientation.Up(), // OBB1 축 2
		obb1.orientation.Backward(), // OBB1 축 3
		obb2.orientation.Right(), // OBB2 축 1
		obb2.orientation.Up(), // OBB2 축 2
		obb2.orientation.Backward(), // OBB2 축 3
		// 외적으로 생성된 추가 축은 여기에서 계산됨
	};

	// 추가 축 계산 (OBB1 축과 OBB2 축의 외적)
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j) {
			test[6 + i * 3 + j] = test[i].Cross(test[3 + j]);
		}
	}

	// 모든 축에 대해 겹치는지 검사
	for (int i = 0; i < 15; ++i)
	{
		if (!OverlapOnAxis(obb1, obb2, test[i])) // 하나라도 겹치지 않으면 false 반환
			return false;
	}

	return true; // 모두 겹치면 true 반환
}

// 두 평면의 충돌 검사
bool MathUtils::PlaneToPlane(const Plane3D& plane1, const Plane3D& plane2)
{
	Vec3 d = plane1.normal.Cross(plane2.normal); // 두 평면의 법선 벡터의 외적
	return d.Dot(d) != 0; // 외적의 결과가 0이 아니면 두 평면은 평행하지 않은 것으로 간주
}

// 구와 레이의 충돌 검사
bool MathUtils::Raycast(const Sphere3D& sphere, const Ray3D& ray, OUT float& distance)
{
	Vec3 e = sphere.position - ray.origin; // 레이의 시작점에서 구의 중심까지의 벡터

	float rSq = sphere.radius * sphere.radius; // 구의 반지름의 제곱
	float eSq = e.LengthSquared(); // e 벡터의 길이의 제곱

	float a = e.Dot(ray.direction); // 레이 방향과 e 벡터의 내적

	float bSq = eSq - (a * a); // b의 제곱(삼각형의 한 변의 제곱)
	float f = sqrt(rSq - bSq); // f는 삼각형의 다른 변(구의 반지름에서 b를 뺀 값)

	// 실제 충돌이 발생하지 않는 경우
	if (rSq - (eSq - (a * a)) < 0.0f)
		return false;

	// 레이의 시작점이 구 내부에 있는 경우
	if (eSq < rSq)
	{
		distance = a + f; // 구를 뚫고 나가는 지점까지의 거리
		return true;
	}
	// 구 외부에서 시작하여 구에 닿지 않는 경우
	distance = a - f; // 구에 가장 가까운 점까지의 거리
	return false;
}

// AABB와 레이의 충돌 검사 (Cyrus-Beck clipping algorithm을 사용)
bool MathUtils::Raycast(const AABB3D& aabb, const Ray3D& ray, OUT float& distance)
{
	Vec3 min = AABB3D::GetMin(aabb); // AABB의 최소 좌표
	Vec3 max = AABB3D::GetMax(aabb); // AABB의 최대 좌표

	// 각 축에 대해 레이가 AABB의 두 평면(최소값, 최대값)과 만나는 t 값을 계산
	float t1 = (min.x - ray.origin.x) / ray.direction.x;
	float t2 = (max.x - ray.origin.x) / ray.direction.x;

	float t3 = (min.y - ray.origin.y) / ray.direction.y;
	float t4 = (max.y - ray.origin.y) / ray.direction.y;

	float t5 = (min.z - ray.origin.z) / ray.direction.z;
	float t6 = (max.z - ray.origin.z) / ray.direction.z;

	// 가장 큰 최소 t 값(tmin)과 가장 작은 최대 t 값(tmax)을 계산
	float tmin = fmaxf(fmaxf(fminf(t1, t2), fminf(t3, t4)), fminf(t5, t6));
	float tmax = fminf(fminf(fmaxf(t1, t2), fmaxf(t3, t4)), fmaxf(t5, t6));

	// tmax가 0보다 작으면 레이는 AABB의 뒤쪽을 향함
	if (tmax < 0)
		return false;

	// tmin이 tmax보다 크면 레이는 AABB를 교차하지 않음
	if (tmin > tmax)
		return false;

	// 실제 충돌 거리 계산
	distance = (tmin < 0.0f) ? tmax : tmin;
	return true;
}

// 평면과 레이의 충돌 검사
bool MathUtils::Raycast(const Plane3D& plane, const Ray3D& ray, OUT float& distance)
{
	float nd = ray.direction.Dot(plane.normal); // 레이 방향과 평면의 법선의 내적
	float pn = ray.origin.Dot(plane.normal); // 레이의 시작점과 평면의 법선의 내적

	// nd가 0보다 크거나 같으면 레이와 평면은 평행하거나 레이가 평면에서 멀어짐
	if (nd >= 0.0f)
		return false;

	// 실제 충돌 거리 계산
	float t = (plane.distance - pn) / nd;
	if (t >= 0.0f) {
		distance = t; // 충돌 지점까지의 거리
		return true;
	}

	return false;
}

// 점이 삼각형 내부에 있는지 판단하는 함수
bool MathUtils::PointInTriangle(const Point3D& p, const Triangle3D& t)
{
	Vec3 a = t.a - p; // 점 p에서 삼각형의 꼭짓점 a로의 벡터
	Vec3 b = t.b - p; // 점 p에서 삼각형의 꼭짓점 b로의 벡터
	Vec3 c = t.c - p; // 점 p에서 삼각형의 꼭짓점 c로의 벡터

	// 삼각형의 각 변에 대해 점 p를 포함하는 평면의 법선 벡터 계산
	Vec3 normPBC = b.Cross(c); // PBC의 법선 벡터 (u)
	Vec3 normPCA = c.Cross(a); // PCA의 법선 벡터 (v)
	Vec3 normPAB = a.Cross(b); // PAB의 법선 벡터 (w)

	// 법선 벡터들의 방향이 모두 같은지 확인하여 점이 삼각형 내부에 있는지 판단
	if (normPBC.Dot(normPCA) < 0.0f)
		return false; // PBC와 PCA 법선 벡터가 서로 반대 방향이면 점은 삼각형 내부에 없음

	else if (normPBC.Dot(normPAB) < 0.0f)
		return false; // PBC와 PAB 법선 벡터가 서로 반대 방향이면 점은 삼각형 내부에 없음

	return true; // 그 외의 경우 점은 삼각형 내부에 있음
}

// 삼각형으로부터 평면을 생성하는 함수
Plane3D MathUtils::FromTriangle(const Triangle3D& t)
{
	Plane3D result;

	// 삼각형의 두 변의 벡터를 외적하여 평면의 법선 벡터를 계산
	result.normal = (t.b - t.a).Cross(t.c - t.a);
	result.normal.Normalize(); // 법선 벡터를 정규화

	// 평면의 거리 계산 (법선 벡터와 삼각형의 한 꼭짓점의 내적으로 계산)
	result.distance = result.normal.Dot(t.a);

	return result;
}

// 미구현된 함수, 삼각형 내의 점에 대한 바리센트릭 좌표를 계산
Vec3 MathUtils::Barycentric(const Point3D& p, const Triangle3D& t)
{
	// 이 함수는 여기에서 구현되지 않았습니다.
	return Vec3();
}

// 레이와 삼각형의 충돌 검사
bool MathUtils::Raycast(const Triangle3D& triangle, const Ray3D& ray, OUT float& distance)
{
	Plane3D plane = FromTriangle(triangle); // 삼각형으로부터 평면 생성

	float t = 0;
	// 레이와 평면의 충돌 검사
	if (!Raycast(plane, ray, OUT t))
		return false; // 충돌하지 않으면 false 반환

	// 충돌 지점 계산
	Point3D result = ray.origin + ray.direction * t;

	// 충돌 지점에 대한 바리센트릭 좌표 계산
	Vec3 barycentric = Barycentric(result, triangle);

	// 바리센트릭 좌표를 사용하여 충돌 지점이 삼각형 내부에 있는지 확인
	if (barycentric.x >= 0.0f && barycentric.x <= 1.0f &&
		barycentric.y >= 0.0f && barycentric.y <= 1.0f &&
		barycentric.z >= 0.0f && barycentric.z <= 1.0f)
	{
		distance = t; // 충돌 거리 설정
		return true; // 충돌 발생
	}
	return false; // 충돌하지 않음
}

// 벡터 a를 벡터 b에 투영하는 함수
Vec3 MathUtils::ProjectVecOnVec(Vec3 a, Vec3 b)
{
	b.Normalize(); // 벡터 b를 정규화

	float dist = a.Dot(b); // 벡터 a와 정규화된 벡터 b의 내적 계산

	return b * dist; // 투영된 벡터 반환
}


float MathUtils::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;

	return r1 + val;
}

Vec2 MathUtils::RandomVec2(float r1, float r2)
{
	Vec2 result;
	result.x = Random(r1, r2);
	result.y = Random(r1, r2);

	return result;
}

Vec3 MathUtils::RandomVec3(float r1, float r2)
{
	Vec3 result;
	result.x = Random(r1, r2);
	result.y = Random(r1, r2);
	result.z = Random(r1, r2);

	return result;
}