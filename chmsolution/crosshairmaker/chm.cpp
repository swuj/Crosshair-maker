#include <windows.h>
#include <tchar.h>
#include <vector>
#include <string>
#include <iostream>

#define FILE_MENU_NEW 1
#define FILE_MENU_EXIT 2
#define FILE_MENU_OPEN 3
#define FILE_MENU_SAVE 4
#define FILE_MENU_SAVEAS 5
#define FILE_MENU_EXPORT 6
#define NEW_CROSS 7
#define SAVE_CROSS 8




struct Pixel {
	uint8_t red, green, blue, alpha;
};

class Crosshair {
private:
	int width;
	int height;
	std::vector<std::vector<Pixel>> pixels;

public:
	Crosshair() : width(0), height(0) {}

	Crosshair(int w, int h) : width(w), height(h), pixels(w, std::vector<Pixel>(h)) {}

	//set pixel color
	void SetColor(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
		if (x >= 0 && x < width && y >= 0 && y < height) {
			pixels[x][y] = { r, g, b, a };
		}
	}

	//new layer

	//delete layer

	//save as file
	void SaveAsPng(const std::string& filename) {
		//save as a png
	}
};

Crosshair xhair;

static TCHAR szWindowClass[] = _T("Crosshair Maker");
static TCHAR szTitle[] = _T("Crosshair Maker");

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);

HMENU hMenu;

HWND dimx;
HWND dimy;

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// The parameters to CreateWindowEx explained:
	// WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// 500, 100: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 500,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindowEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd,
		nCmdShow);
	UpdateWindow(hWnd);


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(
	_In_ HWND   hWnd,
	_In_ UINT   message,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR greeting[] = _T("Crosshairmaker");

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"
		// in the top left corner.
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY: //check if saved changes
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;

	case NEW_CROSS: {
		wchar_t tempw[5];
		wchar_t temph[5];
		std::wstring xStr(tempw, GetWindowTextW(dimx, tempw, 5));
		std::wstring yStr(temph, GetWindowTextW(dimy, temph, 5));
		xhair = Crosshair(std::stoi(xStr),
			std::stoi(yStr));
	}
		break;

	case WM_COMMAND: //menu selection
		switch (wParam) {
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
}

void AddMenus(HWND hWnd) {
	hMenu = CreateMenu();

	HMENU hFileMenu = CreateMenu();
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_SAVE, L"Save");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_SAVEAS, L"Save As");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, L"New crosshair");
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_OPEN, L"Open crosshair");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXPORT, L"Export as PNG");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, L"Exit");


	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, L"File");

	SetMenu(hWnd, hMenu);
}

void AddControls(HWND hWnd) {
	int dimposx = 45;
	int dimposy = 45;
	int dimwidth = 18;
	int dimheight = 18;
	int dimtextwidth = 50;

	CreateWindowW(L"Static", L"x: ", WS_VISIBLE | WS_CHILD, 
		dimposx, dimposy, dimwidth, dimheight, hWnd, NULL,
		NULL, NULL);
	CreateWindowW(L"Static", L"y: ", WS_VISIBLE | WS_CHILD,
		dimposx, dimposy+ dimheight + 5, dimwidth, dimheight, hWnd, NULL,
		NULL, NULL);
	dimx = CreateWindowW(L"Edit", L"20", WS_VISIBLE | WS_CHILD | WS_BORDER, 
		dimposx + dimwidth + 5, dimposy, dimtextwidth, dimheight, hWnd,
		NULL, NULL, NULL);
	dimy = CreateWindowW(L"Edit", L"20", WS_VISIBLE | WS_CHILD | WS_BORDER,
		dimposx + dimwidth + 5, dimposy + dimheight + 5, dimtextwidth, dimheight, hWnd,
		NULL, NULL, NULL);
	CreateWindowW(L"Button", L"NewCrosshair", WS_VISIBLE | WS_CHILD, 
		100, 100, 50, dimheight, hWnd, (HMENU)NEW_CROSS, NULL, NULL);
}