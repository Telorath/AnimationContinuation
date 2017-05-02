#include "GameEngine.h"
#include "BaseApp.h"
#include "..\Log.h"
#include "Rendering.h"
#include "Camera.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceFactory.h"
//#include "KeyFrame.h"

#define RENDERBOX false
#define RENDERTEDDY true
#define RENDERMAGE false

#define RENDERBONES false


#define BACKBUFFER_WIDTH	900
#define BACKBUFFER_HEIGHT	800

void GameEngine::WindowSetup()
{

}

GameEngine::GameEngine(HINSTANCE hinst, WNDPROC proc)
{
	//Save off the data in case we need it, then do something with it.
	application = hinst;
	appWndProc = proc;

	WNDCLASSEX  wndClass;
	ZeroMemory(&wndClass, sizeof(wndClass));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = appWndProc;
	wndClass.lpszClassName = L"DirectXApplication";
	wndClass.hInstance = application;
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
	//wndClass.hIcon			= LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_FSICON));
	RegisterClassEx(&wndClass);

	RECT window_size = { 0, 0, BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT };
	AdjustWindowRect(&window_size, WS_OVERLAPPEDWINDOW, false);

	BaseApp* Application = BaseApp::GetApplication();

	if (Application != nullptr)
	{
		window = CreateWindow(L"DirectXApplication", L"CGS Hardware Project", WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX),
			CW_USEDEFAULT, CW_USEDEFAULT, window_size.right - window_size.left, window_size.bottom - window_size.top,
			NULL, NULL, application, this);

		ShowWindow(window, SW_SHOW);
		Application->init();
		Rendering::SetBackBufferDimensions(BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);
		directXManager = new Rendering::DXManager();
		directXManager->SetWindow(window);
		directXManager->init();

		MainCamera = new Camera();

		MainCamera->CreateCameraInfo(BACKBUFFER_WIDTH, BACKBUFFER_HEIGHT);

		directXManager->SetViewProjection(MainCamera->GetViewMatrixPTR(), MainCamera->GetProjectionMatrixPTR());
		directXManager->SetCameraPosition(MainCamera->GetWorldMatrixPTR());

		ResourceFactory::SetDirectXManager(directXManager);
		ResourceFactory::CreateFBXVertexShader();
		ResourceFactory::CreateAnimationVertexShader();
		ResourceFactory::CreateMappingVertexShader();
		ResourceFactory::CreateUntexturedPixelShader();
		ResourceFactory::CreateTexturedPixelShader();
		ResourceFactory::CreateMappingPixelShader();
		ResourceFactory::CreateBasicPlane();
		ResourceFactory::CreateBoxTexture();
		ResourceFactory::CreateBasicShere();
		ResourceFactory::CreateTeddyTexture();
		ResourceFactory::CreateMageTexture();

		BaseObject* Plane = new BaseObject();
		BaseObjects.push_back(Plane);

		DirectX::XMStoreFloat4x4(Plane->GetWorldMatrixPTR(), DirectX::XMMatrixIdentity());

		directXManager->CreateRenderable("FBXVertexShader", "ModelViewProjection", "UntexturedPixelShader", "PlaneVertexBuffer", "PlaneBufferIndices", "CubeTexture", Plane->GetWorldMatrixPTR(), nullptr);

#if RENDERBOX

		BaseObject* Box = new BaseObject();

		BaseObjects.push_back(Box);

		DirectX::XMStoreFloat4x4(Box->GetWorldMatrixPTR(), DirectX::XMMatrixIdentity());

//		std::vector<EngineAnimation::KeyFrame> Keyframes;

		ResourceFactory::CreateAnimatedBox(Box->GetBonesList());

		directXManager->CreateRenderable("AnimationVertexShader", "AnimationInputLayout", "TexturedPixelShader", "BoxVertexBuffer", "BoxBufferIndices", "CubeTexture", Box->GetWorldMatrixPTR(), Box->GetBonesList());

		for (size_t i = 0; i < Box->GetBonesList()->size(); i++)
		{
		#if RENDERBONES
			directXManager->CreateRenderable("FBXVertexShader", "ModelViewProjection", "UntexturedPixelShader", "SphereVertexBuffer", "SphereBufferIndices", "CubeTexture", Box->GetBonesList()->operator[](i).GetWorldMatrixPTR(), nullptr);
		#endif
			DirectX::XMStoreFloat4x4(Box->GetBonesList()->operator[](i).GetWorldMatrixPTR(), DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(Box->GetBonesList()->operator[](i).GetWorldMatrixPTR())));
		}
#endif
#if RENDERTEDDY
		BaseObject* Teddy = new BaseObject();

		BaseObjects.push_back(Teddy);

		ResourceFactory::CreateTeddyBear(Teddy->GetBonesList(), Teddy->GetAnimationList());
		

		DirectX::XMStoreFloat4x4(Teddy->GetWorldMatrixPTR(), DirectX::XMMatrixIdentity());


		directXManager->CreateRenderable("AnimationVertexShader", "AnimationInputLayout", "TexturedPixelShader", "TeddyVertexBuffer", "TeddyBufferIndices", "TeddyBearTexture", Teddy->GetWorldMatrixPTR(), Teddy->GetBonesList());

		for (size_t i = 0; i < Teddy->GetBonesList()->size(); i++)
		{
		#if RENDERBONES
			directXManager->CreateRenderable("FBXVertexShader", "ModelViewProjection", "UntexturedPixelShader", "SphereVertexBuffer", "SphereBufferIndices", "CubeTexture", Teddy->GetBonesList()->operator[](i).GetWorldMatrixPTR(), nullptr);
		#endif
			DirectX::XMStoreFloat4x4(Teddy->GetBonesList()->operator[](i).GetWorldMatrixPTR(), DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(Teddy->GetBonesList()->operator[](i).GetWorldMatrixPTR())));
		}
#endif
#if RENDERMAGE
		BaseObject* Mage = new BaseObject();

		BaseObjects.push_back(Mage);

		ResourceFactory::CreateMage(Mage->GetBonesList(), Mage->GetKeyFrameList());

		DirectX::XMStoreFloat4x4(Mage->GetWorldMatrixPTR(), DirectX::XMMatrixIdentity());


		directXManager->CreateRenderable("MappingVertexShader", "MappingInputLayout", "MappingPixelShader", "MageVertexBuffer", "MageBufferIndices", "MageTexture", Mage->GetWorldMatrixPTR(), Mage->GetBonesList(), "MageSpecularMap", "MageNormalMap");
#if RENDERBONES

		for (size_t i = 0; i < Mage->GetBonesList()->size(); i++)
		{
			directXManager->CreateRenderable("FBXVertexShader", "ModelViewProjection", "UntexturedPixelShader", "SphereVertexBuffer", "SphereBufferIndices", "CubeTexture", Mage->GetBonesList()->operator[](i).GetWorldMatrixPTR(), nullptr);
		}
#endif
#endif

		GameTimer = TimeManager::CreateWindowsTimeManager();
		GameTimer->init();

		for (size_t i = 0; i < BaseObjects.size(); i++)
		{
			BaseObjects[i]->init();
		}

	}
}

bool GameEngine::Run()
{
	GameTimer->ping();
	float DeltaTime = GameTimer->GetDeltaTime();
	InputManager::update();

	if (InputManager::getkeydown(InputManager::Button::keypad_t))
	{
		//Do a thing
	}

	MainCamera->update(DeltaTime);
	for (size_t i = 0; i < BaseObjects.size(); i++)
	{
		BaseObjects[i]->update(DeltaTime);
	}
	directXManager->update();
	InputManager::finalupdate();
	return true;
}

bool GameEngine::Shutdown()
{
	directXManager->shutdown();
	delete directXManager;
	delete MainCamera;
	delete GameTimer;
	for (size_t i = 0; i < BaseObjects.size(); i++)
	{
		delete BaseObjects[i];
	}
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
	srand(unsigned int(time(0)));
	LogSetUp(L"RTA");
	GameEngine myApp(hInstance, (WNDPROC)WndProc);
	MSG msg; ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT && myApp.Run())
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	myApp.Shutdown();
	return 0;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case (WM_DESTROY): { PostQuitMessage(0); }
					   break;
	case (WM_KEYDOWN):
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		case 'W':
			InputManager::setstate(InputManager::Button::keypad_w, true);
			break;
		case 'S':
			InputManager::setstate(InputManager::Button::keypad_s, true);
			break;
		case 'Q':
			InputManager::setstate(InputManager::Button::keypad_q, true);
			break;
		case 'E':
			InputManager::setstate(InputManager::Button::keypad_e, true);
			break;
		case 'A':
			InputManager::setstate(InputManager::Button::keypad_a, true);
			break;
		case 'D':
			InputManager::setstate(InputManager::Button::keypad_d, true);
			break;
		case 'T':
			InputManager::setstate(InputManager::Button::keypad_t, true);
			break;
		case VK_OEM_PLUS:
			InputManager::setstate(InputManager::Button::keypad_equals, true);
			break;
		case VK_UP:
			InputManager::setstate(InputManager::Button::arrow_up, true);
			break;
		case VK_DOWN:
			InputManager::setstate(InputManager::Button::arrow_down, true);
			break;
		case VK_LEFT:
			InputManager::setstate(InputManager::Button::arrow_left, true);
			break;
		case VK_RIGHT:
			InputManager::setstate(InputManager::Button::arrow_right, true);
			break;
		}
		break;
	case (WM_KEYUP):
		switch (wParam)
		{
		case 'W':
			InputManager::setstate(InputManager::Button::keypad_w, false);
			break;
		case 'S':
			InputManager::setstate(InputManager::Button::keypad_s, false);
			break;
		case 'Q':
			InputManager::setstate(InputManager::Button::keypad_q, false);
			break;
		case 'E':
			InputManager::setstate(InputManager::Button::keypad_e, false);
			break;
		case 'A':
			InputManager::setstate(InputManager::Button::keypad_a, false);
			break;
		case 'D':
			InputManager::setstate(InputManager::Button::keypad_d, false);
			break;
		case 'T':
			InputManager::setstate(InputManager::Button::keypad_t, false);
			break;
		case VK_OEM_PLUS:
			InputManager::setstate(InputManager::Button::keypad_equals, false);
			break;
		case VK_UP:
			InputManager::setstate(InputManager::Button::arrow_up, false);
			break;
		case VK_DOWN:
			InputManager::setstate(InputManager::Button::arrow_down, false);
			break;
		case VK_LEFT:
			InputManager::setstate(InputManager::Button::arrow_left, false);
			break;
		case VK_RIGHT:
			InputManager::setstate(InputManager::Button::arrow_right, false);
			break;
		}
		break;
	case (WM_RBUTTONDOWN):
		InputManager::setstate(InputManager::Button::mouse_2, true);
		break;
	case (WM_RBUTTONUP):
		InputManager::setstate(InputManager::Button::mouse_2, false);
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}