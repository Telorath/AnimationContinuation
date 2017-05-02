#pragma once
#include "BaseObject.h"

#define Pi 3.141592654

class Camera : public BaseObject
{
	DirectX::XMFLOAT4X4 m_View;
	DirectX::XMFLOAT4X4 m_Projection;
	float m_FieldOfView;
	float m_AspectRatio;
	float m_Near;
	float m_Far;
	float m_YRotation;
	float m_XRotation;
	float m_ZRotation;
	float m_moveSpeed = 1.0f;
	float m_MouseSensitivity = 0.002f;
	void UpdateViewProjection();
public:
	Camera();
	~Camera();
	void CreateCameraInfo(unsigned int Width, unsigned int Height);
	void update(float DeltaTime) override;
	DirectX::XMFLOAT4X4* GetViewMatrixPTR();
	DirectX::XMFLOAT4X4* GetProjectionMatrixPTR();
};