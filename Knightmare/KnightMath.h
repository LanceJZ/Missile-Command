#pragma once
#include "Common.h"
#include <random>
#include "raylib.h"
#include "raymath.h"

class KnightMath
{
public:
	KnightMath();
	virtual ~KnightMath();

	int WindowWidth = 0;
	int WindowHeight = 0;
	int GameWindowHalfWidth = { 0 };
	int GameWindowHalfHeight = { 0 };

	bool Initialize();
	bool BeginRun();

	float GetRandomScreenY();
	float GetRandomScreenX();
	float GetRandomFloat(float min, float max);
	float GetRandomRadian();
	float GetRotationTowardsTargetZ(Vector3& origin, Vector3& target, float facingAngle,
		float magnitude);
	float GetAngleFromVectorsZ(Vector3& origin, Vector3& target);
	Vector3 GetRandomVelocity(float speed);
	Vector3 GetRandomVelocity(float speed, float radianDirection);
	Vector3 GetVelocityFromAngleZ(float rotation, float magnitude);
	Vector3 GetVelocityFromAngleZ(float magnitude);
	Color GetRandomColor();
	Quaternion EulerToQuaternion(float yaw, float pitch, float roll);

	void SetRotationZTowardsTargetZ(Vector3& target, float magnitude);
	void SetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed);
	void SetAccelerationToZero(float decelerationAmount);
	void SetRotateVelocity(Vector3& position, float turnSpeed, float speed);
	void SetRotationZFromVector(Vector3& target);
	void SetAimAtTargetZ(Vector3& target, float facingAngle, float magnitute);

private:
	std::random_device RandomDevice = {};
	std::mt19937_64 Random19937 = {};

};

