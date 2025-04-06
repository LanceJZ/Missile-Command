#pragma once
#include <vector>
#include "raylib.h"
#include "Common.h"
#include "ContentManager.h"

class Entity : public Common
{
public:
	bool Cull = true;
	bool EntityOnly = false;
	bool ShowCollision = false;
	bool HideCollision = false;
	bool IsChild = false;
	bool IsParent = false;
	bool IsConnectedChild = true;
	bool Enabled = true;
	bool IgnoreParentRotation = false;
	bool WasCulled = false;
	bool BeenHit = false;
	bool Stationary = false;
	int ChildNumber = 0;
	float Scale = 1;
	float ModelScale = 1;
	float MaxSpeed = 0;
	float Radius = 0;
	float VerticesSize = 0;
	float RotationX = 0;
	float RotationY = 0;
	float RotationZ = 0;
	float RotationVelocityX = 0;
	float RotationVelocityY = 0;
	float RotationVelocityZ = 0;
	float RotationAccelerationX = 0;
	float RotationAccelerationY = 0;
	float RotationAccelerationZ = 0;
	float DeltaTime = 0;

	Vector3 Acceleration = Vector3Zero();
	Vector3 Velocity = Vector3Zero();
	Vector3 Position = Vector3Zero();
	Vector3 Rotation = Vector3Zero();
	Vector3 RotationVelocity = Vector3Zero();
	Vector3 RotationAcceleration = Vector3Zero();
	Vector3 LastFramePosition = Vector3Zero();
	Vector3 LastFrameWorldPosition = Vector3Zero();
	Vector3 WorldPosition = Vector3Zero();
	Vector3 WorldRotation = Vector3Zero();

	Quaternion RotationQ = {};
	Matrix WorldMatrix = MatrixIdentity();

	std::vector<Entity*> *Parents;
	std::string EntityName = {};

	Entity();
	virtual ~Entity();

	bool Initialize();
	virtual void Update(float deltaTime);
	virtual void AlwaysUpdate(float deltaTime);
	virtual void FixedUpdate(float deltaTime);
	virtual void Draw3D();
	void Draw2D();

	void X(float x);
	void Y(float y);
	void Z(float z);
	void SetScale(float scale);

	virtual void Spawn();
	virtual void Spawn(Vector3 position);
	virtual void Hit();
	virtual void Destroy();

	bool GetBeenHit();
	bool CirclesIntersect(Vector3& targetPosition, float targetRadius);
	bool CirclesIntersect(Entity& target);
	bool CirclesIntersectBullet(Entity& target);
	bool ScreenEdgeBoundY();
	bool ScreenEdgeBoundY(float topOffset, float bottomOffset);
	bool ScreenEdgeBoundX();
	bool ScreenEdgeBoundX(float leftOffset, float rightOffset);
	bool IsOffScreen();
	bool IsOffScreenSide();
	bool IsOffScreenTopBottom();

	float X();
	float Y();
	float Z();
	float GetAngleFromVectorZ(Vector3& target);
	float GetAngleFromWorldVectorZ(Vector3& target);

	Vector3 GetVelocityFromAngleZ(float magnitude);
	Vector3 GetVelocityFromVectorZ(Vector3& target, float magnitude);
	Vector3 GetAccelerationToMaxAtRotation(float accelerationAmount, float topSpeed);
	Vector3 GetWorldPosition();

	virtual bool SetCamera(Camera* camera);
	virtual void SetModel(Model &model, float scale);
	virtual void SetModel(Model &model);
	virtual void SetModelCopy(Model model, float scale);
	virtual void SetModelWithTexture(Model &model, Texture2D &texture);
	virtual LineModelPoints GetLineModel();
	virtual std::vector<Vector3> GetModel();
	virtual void SetModel(std::vector<Vector3> lines);
	virtual void SetModel(LineModelPoints lines);
	virtual void SetModel(LineModelPoints lines, float scale);
	virtual Model& Get3DModel();
	virtual void Reset();
	void SetParent(Entity& parent);
	void RemoveParent(Entity* parent);
	void ClearParents();
	void CheckScreenEdge();
	void CheckScreenEdgeX();
	void CheckScreenEdgeY();
	void CheckPlayfieldSidesWarp();
	bool CheckPlayfieldSidesWarp(float left, float right);
	void CheckPlayfieldHeightWarp(float top, float bottom);

protected:
	std::vector<Vector3> LinePoints;
	LineModelPoints Lines;

	Camera* TheCamera3D = {};
	Model TheModel = {};

	void BeforeCalculate();
	void CalculateWorldSpace();
	void CalculateWorldVectors();
	void AfterCalculate();

private:
	Ray TheRay = { 0 };
	//RayCollision TheRayCollision = { 0 };

	float RadianSpin(float radian);
	float AddRotationVelAcc(float rotation, float rotationVelocity,
		float rotationAcceleration, float deltaTime);

	void CalculateRadius();
};

