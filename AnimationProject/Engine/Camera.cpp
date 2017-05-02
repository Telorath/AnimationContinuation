#include "Camera.h"
#include "InputManager.h"
#include "TimeManager.h"

using namespace DirectX;

void Camera::UpdateViewProjection()
{

}

Camera::Camera()
{
}


Camera::~Camera()
{
}

void Camera::CreateCameraInfo(unsigned int Width, unsigned int Height)
{
	float aspectRatio = (float)Width / (float)Height;
	float fovAngleY = 70.0f * DirectX::XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	DirectX::XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, 0.01f, 100.0f);

//	DirectX::XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

//	DirectX::XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);
	DirectX::XMStoreFloat4x4(&m_Projection, perspectiveMatrix);

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	static const DirectX::XMVECTORF32 eye = { 0.0f, 0.7f, -1.5f, 0.0f };
	static const DirectX::XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const DirectX::XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_World, XMMatrixInverse(nullptr, XMMatrixLookAtLH(eye, at, up)));
	XMStoreFloat4x4(&m_View, XMMatrixLookAtLH(eye, at, up));

	m_XRotation = 0;
	m_YRotation = 0;
	m_ZRotation = 0;

}

void Camera::update(float DeltaTime)
{
	if (InputManager::getkeystate(InputManager::Button::keypad_w))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, m_moveSpeed * DeltaTime);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_World, result);
	}
	if (InputManager::getkeystate(InputManager::Button::keypad_s))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, 0.0f, -m_moveSpeed * DeltaTime);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_World, result);
	}
	if (InputManager::getkeystate(InputManager::Button::keypad_q))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, m_moveSpeed * DeltaTime, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_World, result);
	}
	if (InputManager::getkeystate(InputManager::Button::keypad_e))
	{
		XMMATRIX translation = XMMatrixTranslation(0.0f, -m_moveSpeed * DeltaTime, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_World, result);
	}
	if (InputManager::getkeystate(InputManager::Button::keypad_a))
	{
		XMMATRIX translation = XMMatrixTranslation(-m_moveSpeed * DeltaTime, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_World, result);
	}
	if (InputManager::getkeystate(InputManager::Button::keypad_d))
	{
		XMMATRIX translation = XMMatrixTranslation(m_moveSpeed * DeltaTime, 0.0f, 0.0f);
		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		XMMATRIX result = XMMatrixMultiply(translation, temp_camera);
		XMStoreFloat4x4(&m_World, result);
	}
	if (InputManager::getkeystate(InputManager::Button::mouse_2))
	{
		float dx;
		float dy;

		InputManager::GetCursorDifference(&dx, &dy);

		XMFLOAT4 pos = XMFLOAT4(m_World._41, m_World._42, m_World._43, m_World._44);

		m_World._41 = 0;
		m_World._42 = 0;
		m_World._43 = 0;

		XMMATRIX rotX = XMMatrixRotationX(dy * m_MouseSensitivity);// *DeltaTime);
		XMMATRIX rotY = XMMatrixRotationY(dx * m_MouseSensitivity);// *DeltaTime);

		XMMATRIX temp_camera = XMLoadFloat4x4(&m_World);
		temp_camera = XMMatrixMultiply(rotX, temp_camera);
		temp_camera = XMMatrixMultiply(temp_camera, rotY);

		XMStoreFloat4x4(&m_World, temp_camera);

		m_World._41 = pos.x;
		m_World._42 = pos.y;
		m_World._43 = pos.z;
	}
	XMStoreFloat4x4(&m_View, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_World)));
}

DirectX::XMFLOAT4X4 * Camera::GetViewMatrixPTR()
{
	return &m_View;
}

DirectX::XMFLOAT4X4 * Camera::GetProjectionMatrixPTR()
{
	return &m_Projection;
}
