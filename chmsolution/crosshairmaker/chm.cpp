#include <windows.h>
#include <tchar.h>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "xhair.h"
#include "component.h"


#define FILE_MENU_NEW 1
#define FILE_MENU_EXIT 2
#define FILE_MENU_OPEN 3
#define FILE_MENU_SAVE 4
#define FILE_MENU_SAVEAS 5
#define FILE_MENU_EXPORT 6
#define NEW_CROSS 7
#define SAVE_CROSS 8
#define DUMMY_CROSS 9
#define PREVIEW 10
#define TEST 100


// Function to render the crosshair onto the preview window
void RenderCrosshairPreview(HDC hdc, const Crosshair& crosshair, int x, int y, int previewSize) {
	int m = max(crosshair.GetWidth(), crosshair.GetHeight());
	const int pixelSize = previewSize / m;

	for (size_t i = 0; i < crosshair.GetWidth(); ++i) {
		for (size_t j = 0; j < crosshair.GetHeight(); ++j) {
			const Pixel& pixel = crosshair.GetPixels()[i][j];

			COLORREF color = RGB(pixel.red, pixel.green, pixel.blue);

			// Adjust the position based on the pixel size and coordinates
			int xPos = x + i * pixelSize;
			int yPos = y + j * pixelSize;

			// Draw a rectangle with the specified color
			HBRUSH hBrush = CreateSolidBrush(color);
			RECT rect = { xPos, yPos, xPos + pixelSize, yPos + pixelSize };
			FillRect(hdc, &rect, hBrush);
			DeleteObject(hBrush);
		}
	}
}

Crosshair xhair;

static TCHAR szWindowClass[] = _T("Crosshair Maker");
static TCHAR szTitle[] = _T("Crosshair Maker");

HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
void UpdatePreview(Crosshair, HWND);
void AddEditControls(HWND);
void DrawPlus(Component*);

HMENU hMenu;

HWND dimx, dimy, newXhairButton, loadXhairButton, saveXhairButton, textx, texty;
HWND hEditingInterface, testButton, hPreview;

bool setfn = false;
bool xhairExists = false;

OPENFILENAME ofn;
wchar_t szFile[MAX_PATH] = L"";

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
	case WM_PAINT: {
		OutputDebugString(L"Painting\n");
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
	}
	case WM_DESTROY: { //check if saved changes
		PostQuitMessage(0);
		break;
	}
	case WM_CREATE: {
		OutputDebugString(L"Creating\n");
		AddMenus(hWnd);
		AddControls(hWnd);
		
		break;
	}

	case WM_CLOSE: {
		// Handle the window close event
		if (MessageBox(hWnd, L"Do you want to save changes?", L"Confirmation", MB_YESNO | MB_ICONQUESTION) == IDYES) {
			// Save changes or perform other actions
			//xhair.SaveAsPng(L"changes_saved.png");
		}
		DestroyWindow(hWnd);
		break;
	}

	case WM_COMMAND: {//menu selection
		switch (wParam) {
		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;

		case NEW_CROSS: {

			OutputDebugString(L"New\n");
			wchar_t tempw[5];
			wchar_t temph[5];
			std::wstring xStr(tempw, GetWindowTextW(dimx, tempw, 5));
			std::wstring yStr(temph, GetWindowTextW(dimy, temph, 5));
			try {
				xhair = Crosshair(std::stoi(xStr), std::stoi(yStr));
			}
			catch (const std::exception& e) {
				// Handle the exception (e.g., display an error message)
				//use default 20x20
				xhair = Crosshair(20, 20);
			}

			xhairExists = true;

			DestroyWindow(newXhairButton);
			DestroyWindow(saveXhairButton);
			DestroyWindow(loadXhairButton);
			DestroyWindow(dimx);
			DestroyWindow(dimy);
			DestroyWindow(textx);
			DestroyWindow(texty);

			

			AddEditControls(hWnd);
			xhair.InitializeTest();

			UpdatePreview(xhair, hPreview);

			break;

		}
		case SAVE_CROSS: {
			if (xhairExists) {
				OutputDebugString(L"Save\n");
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hWnd;
				ofn.lpstrFile = szFile;
				ofn.nMaxFile = sizeof(szFile) / sizeof(*szFile);
				ofn.lpstrFilter = L"PNG Files (*.png)\0*.png\0All Files (*.*)\0*.*\0";
				ofn.lpstrDefExt = L"png";
				ofn.Flags = OFN_OVERWRITEPROMPT;

				if (GetSaveFileName(&ofn)) {
					xhair.SaveAsPng(szFile);
				}
			}
			else {
				MessageBox(hWnd, L"You must load or create a crosshair to save!", L"Umm", MB_OK);
			}
			
			break;
			
		}
		case TEST: {
			Plus* newplus = new Plus();
			Plus* newplus2 = new Plus("OP", {0,255,0,255}, 5, 1, false, {0,0,0,0}, 0, 0, 0);
			xhair.AddLayer(newplus);
			xhair.AddLayer(newplus2);

			xhair.layers[0]->SetGap(4);

			for (Component* c : xhair.layers) {
				int type = c->GetType();
				OutputDebugString(std::to_wstring(type).c_str());

				if (type == 4) {
					OutputDebugString(L"Drawing a plus\n");
					DrawPlus(c);
				}
			}

			//DrawPlus(xhair.layers[1]);

			UpdatePreview(xhair, hPreview);
		}
		}
	}
	default: {
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
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

	textx = CreateWindowW(L"Static", L"x: ", WS_VISIBLE | WS_CHILD, 
		dimposx, dimposy, dimwidth, dimheight, hWnd, NULL,
		NULL, NULL);
	texty = CreateWindowW(L"Static", L"y: ", WS_VISIBLE | WS_CHILD,
		dimposx, dimposy+ dimheight + 5, dimwidth, dimheight, hWnd, NULL,
		NULL, NULL);
	dimx = CreateWindowW(L"Edit", L"20", WS_VISIBLE | WS_CHILD | WS_BORDER, 
		dimposx + dimwidth + 5, dimposy, dimtextwidth, dimheight, hWnd,
		NULL, NULL, NULL);
	dimy = CreateWindowW(L"Edit", L"20", WS_VISIBLE | WS_CHILD | WS_BORDER,
		dimposx + dimwidth + 5, dimposy + dimheight + 5, dimtextwidth, dimheight, hWnd,
		NULL, NULL, NULL);
	SendMessage(dimx, EM_SETLIMITTEXT, 4, 0);
	SendMessage(dimy, EM_SETLIMITTEXT, 4, 0);

	int mainBtnWidth = 100;
	int mainBtnHeight = 75;
	int mainBtnSpace = 10;


	newXhairButton = CreateWindowW(L"Button", L"New", WS_VISIBLE | WS_CHILD, 
		100, 100, mainBtnWidth, mainBtnHeight, hWnd, (HMENU)NEW_CROSS, NULL, NULL);

	loadXhairButton = CreateWindowW(L"Button", L"Load", WS_VISIBLE | WS_CHILD,
		100 + mainBtnWidth + mainBtnSpace, 100, mainBtnWidth, mainBtnHeight, hWnd, NULL, NULL, NULL);

	saveXhairButton = CreateWindowW(L"Button", L"Save", WS_VISIBLE | WS_CHILD,
		100 + mainBtnWidth*2 + mainBtnSpace*2, 100, mainBtnWidth, mainBtnHeight, hWnd, (HMENU)SAVE_CROSS, NULL, NULL);
	//CreateWindowW(L"Button", L"Test", WS_VISIBLE | WS_CHILD,
		//155, 100, 50, dimheight, hWnd, (HMENU)DUMMY_CROSS, NULL, NULL);
}

void AddEditControls(HWND hWnd) {
	//hEditingInterface = CreateWindowW(L"STATIC", L"This is the new interface!",
	//	WS_VISIBLE | WS_CHILD, 10, 10, 200, 50, hWnd, NULL, NULL, NULL);
	hPreview = CreateWindowW(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_CENTER,
		250, 50, 0, 0, hWnd, (HMENU)PREVIEW, NULL, NULL);

	saveXhairButton = CreateWindowW(L"Button", L"Save", WS_VISIBLE | WS_CHILD,
		100, 300, 70, 30, hWnd, (HMENU)SAVE_CROSS, NULL, NULL);

	testButton = CreateWindowW(L"Button", L"Test", WS_VISIBLE | WS_CHILD,
		100, 200, 70, 30, hWnd, (HMENU)TEST, NULL, NULL);
}


void UpdatePreview(Crosshair crosshair, HWND hWnd) {
	OutputDebugString(L"UpdatePreview called\n");
	// Get pixel data from the Crosshair object
	const int previewSize = 200;

	// Get HDC of the preview static control
	//HWND hPreview = GetDlgItem(hParent, PREVIEW); // Assuming IDC_PREVIEW is the control ID
	HDC hdcPreview = GetDC(hWnd);

	// Clear the preview window
	RECT rect;
	GetClientRect(hWnd, &rect);
	FillRect(hdcPreview, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

	// Render the crosshair onto the preview window
	RenderCrosshairPreview(hdcPreview, crosshair, 0, 0, previewSize);

	// Release HDC
	ReleaseDC(hWnd, hdcPreview);
	OutputDebugString(L"UpdatePreview finished\n");
}

void DrawPlus(Component* c) {

	Plus* plus = dynamic_cast<Plus*>(c);
	//Plus plus = &plusp;

	if (plus) {
		int xcenter = xhair.GetWidth() / 2;
		int ycenter = xhair.GetHeight() / 2;

		int width = plus->GetWidth();
		int length = plus->GetSize();
		int gap = plus->GetGap();

		Pixel color = plus->GetColor();
		Pixel outline_color = plus->GetOutlineColor();

		int outline = plus->GetOutlineThickness();


		//Each Loop draws one arm, if i or j are outside a certain boundry it draws the outline color instead of the shape color
		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				if (i < 0 || i >= width || j < 0 || j >= length) {
					xhair.SetColor(xcenter - (width / 2) + i, ycenter + gap + j, outline_color);
				}
				else {
					xhair.SetColor(xcenter - (width / 2) + i, ycenter + gap + j, color);
				}
			}
		}
		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				if (i < 0 || i >= width || j < 0 || j >= length) {
					xhair.SetColor(xcenter - (width / 2) + i, ycenter - gap + j - length, outline_color);
				}
				else {
					xhair.SetColor(xcenter - (width / 2) + i, ycenter - gap + j - length, color);
				}
			}
		}
		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				if (i < 0 || i >= width || j < 0 || j >= length) {
					xhair.SetColor(xcenter - length + j - gap, ycenter - (width / 2) + i, outline_color);
				}
				else {
					xhair.SetColor(xcenter - length + j - gap, ycenter - (width / 2) + i, color);
				}
			}
		}
		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				if (i < 0 || i >= width || j < 0 || j >= length) {
					xhair.SetColor(xcenter + j + gap, ycenter - (width / 2) + i, outline_color);
				}
				else {
					xhair.SetColor(xcenter + j + gap, ycenter - (width / 2) + i, color);
				}
			}
		}
	}
	else {
		OutputDebugString(L"Not a plus\n");
		//std::cout << "Error: Not a Plus object" << std::endl;
	}
	
}