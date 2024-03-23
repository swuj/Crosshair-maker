// Start of wxWidgets "Hello World" Program
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include "wclasses.h"



Crosshair xhair;

int LoadFromFile();
void DrawPlus(Component* c);
void UpdateCrosshairPixels();

class MainApp : public wxApp // MainApp is the class for our application
{
    // MainApp just acts as a container for the window,
public: // or frame in MainFrame
    virtual bool OnInit();
};

class MyFrame : public wxFrame // MainFrame is the class for our window,
{
private:
	wxPanel* mainpanel;
    wxBoxSizer* mainsizer;
	wxBoxSizer* mainsizer2;
    wxButton* saveButton;
    wxButton* testButton;
	ImagePanel* previewimg;
	NumericTextCtrl* widthEntry;
	NumericTextCtrl* heightEntry;
	wxButton* confirmNewButton;
	ScrolledWidgetsPane* layerlist;

	//Edit Interface
	wxBoxSizer* layersizer;
	wxBoxSizer* controlsizer;
	wxBoxSizer* previewsizer;

	LayerListPane* layerListPane;
	ControlPanelPane* controlPanelPane;
	PreviewPanelPane* previewPanelPane;

	std::vector<wxButton*> layerlistbuttons;

	int wwidth;
	int wheight;

public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    

    void OnExit(wxCommandEvent& event);
    void LoadButtonClicked(wxCommandEvent& event);
	void LoadButtonClicked2(wxCommandEvent& event);
    void NewButtonClicked(wxCommandEvent& event);
	void NewButtonClicked2(wxCommandEvent& event);
    void TestButtonClicked(wxCommandEvent& event);
	void NewLayerButtonClicked(wxCommandEvent& event);
	void SaveButtonClicked(wxCommandEvent& event);
	void DeleteLayerButtonClicked(wxCommandEvent& event);
	void UpdateLayerListPane();
	void UpdateLayerListPane2();
	void CreateLayerListPane();
	void UpdateLayerControlPane();
	void UpdateLayerControlPane2();
	void CreateLayerControlPane();
	void UpdatePreviewPane();
	void PopulateLayerButtons(Crosshair crosshair);
	void InsertButton(Crosshair crosshair);
	void RemoveButton(Crosshair crosshair);
	void LayerButtonClicked(wxCommandEvent& event);
	void SliderChanged(wxCommandEvent& event);
	void OnNumericTextEnter(wxCommandEvent& event);
	void OutlineCheckboxClicked(wxCommandEvent& event);
	void VisibilityButtonClicked(wxCommandEvent& event);

	void ShowInitialInterface() {
		mainsizer2->Clear(true);

		wxButton* newCrosshairButton = new wxButton(mainpanel, BUTTON_NEW, ("New"));
		wxButton* loadCrosshairButton = new wxButton(mainpanel, BUTTON_LOAD, ("Load"));

		mainsizer2->Add(newCrosshairButton, 0, wxCENTER, 5);
		mainsizer2->Add(loadCrosshairButton, 0, wxCENTER, 5);

		//SetSizer(mainsizer);
	}

    void ShowEditInterface() {
        // Clear current interface
		mainsizer2->Clear(true);

		/*************************************/
		//LAYER LIST PANE
		/*************************************/
		layersizer = new wxBoxSizer(wxHORIZONTAL);
		//UpdateLayerListPane();
		CreateLayerListPane();
		
		mainsizer2->Add(layersizer, 1, wxEXPAND | wxALL, 2);

		/***************************************/
		//LAYER CONTROL PANE
		/***************************************/
		controlsizer = new wxBoxSizer(wxHORIZONTAL);
		//UpdateLayerControlPane();
		CreateLayerControlPane();
		mainsizer2->Add(controlsizer, 1, wxEXPAND | wxALL, 2);

		/***************************************/
		//PREVIEW PANE
		/***************************************/
		previewsizer = new wxBoxSizer(wxHORIZONTAL);
		UpdatePreviewPane();
		mainsizer2->Add(previewsizer, 1, wxEXPAND | wxALL, 2);

		Layout();
		Refresh();
		Update();
    }

	void ShowNewInterface() {
		mainsizer2->Clear(true);

		//wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 200));

		//text boxes for height and width dimensions
		wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
		widthEntry = new NumericTextCtrl(mainpanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		heightEntry = new NumericTextCtrl(mainpanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		confirmNewButton = new wxButton(mainpanel, BUTTON_NEW2, "Save");

		
		vbox->Add(widthEntry, 1, wxEXPAND | wxALL, 5);
		vbox->Add(heightEntry, 1, wxEXPAND | wxALL, 5);
		vbox->Add(confirmNewButton, 1, wxEXPAND | wxALL, 5);
		//panel->SetSizer(vbox);
		mainsizer2->Add(vbox);

		Layout();
		Refresh();
		Update();
	}

    DECLARE_EVENT_TABLE()
};

wxIMPLEMENT_APP(MainApp);

bool MainApp::OnInit()
{
    MyFrame* frame = new MyFrame("Crosshair Maker", wxPoint(1, 1), wxSize(1000, 800));
    frame->Show(true);
    SetTopWindow(frame);

    return TRUE;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(BUTTON_LOAD, MyFrame::LoadButtonClicked) // Tell the OS to run MainFrame::OnExit when
	EVT_BUTTON(BUTTON_SAVE, MyFrame::SaveButtonClicked)
    EVT_BUTTON(BUTTON_NEW, MyFrame::NewButtonClicked)
	EVT_BUTTON(BUTTON_NEW2, MyFrame::NewButtonClicked2)
	EVT_BUTTON(BUTTON_NEWLAYER, MyFrame::NewLayerButtonClicked)
	EVT_BUTTON(BUTTON_DELETELAYER, MyFrame::DeleteLayerButtonClicked)
	EVT_BUTTON(BUTTON_TEST, MyFrame::TestButtonClicked)
	EVT_BUTTON(BUTTON_LAYER, MyFrame::LayerButtonClicked)
	EVT_BUTTON(BUTTON_VISIBLE, MyFrame::VisibilityButtonClicked)
	EVT_COMMAND(TEXT_UPDATE, wxEVT_COMMAND_TEXT_UPDATED, MyFrame::SliderChanged)
	EVT_COMMAND(SLIDER_UPDATE, wxEVT_COMMAND_TEXT_UPDATED, MyFrame::OnNumericTextEnter)
	EVT_COMMAND(CHECKBOX_HASOUTLINE, wxEVT_CHECKBOX, MyFrame::OutlineCheckboxClicked)
	//EVT_SCROLL(SLIDER_CONTROLZ, MyFrame::SliderChanged)
    //EVT_BUTTON(BUTTON_TEST, MyFrame::TestButtonClicked)
END_EVENT_TABLE() // The button is pressed

BEGIN_EVENT_TABLE(ImagePanel, wxPanel)
	EVT_PAINT(ImagePanel::OnPaint)
END_EVENT_TABLE()


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize
    & size) : wxFrame((wxFrame*)NULL, -1, title, pos, size)
{
	wwidth = size.x;
	wheight = size.y;

	mainpanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, size);
	mainpanel->SetBackgroundColour(wxColor(50, 50, 50));
	mainsizer = new wxBoxSizer(wxHORIZONTAL);
	mainsizer2 = new wxBoxSizer(wxHORIZONTAL);
	mainsizer->Add(mainpanel, 1);

	mainpanel->SetSizer(mainsizer2);
	SetSizer(mainsizer);

	ShowInitialInterface();
}

void MyFrame::LoadButtonClicked(wxCommandEvent& event) {
	if (LoadFromFile() == 10) {
		UpdateCrosshairPixels();
		ShowEditInterface();
	}
}

void MyFrame::NewButtonClicked(wxCommandEvent& event) {
	OutputDebugString(L"New Button Clicked\n");
	ShowNewInterface();
}

void MyFrame::NewButtonClicked2(wxCommandEvent& event) {
	OutputDebugString(L"New Button2 Clicked\n");
	int h = wxAtoi(heightEntry->GetValue());
	int w = wxAtoi(widthEntry->GetValue());
	xhair = Crosshair(h, w);
	xhair.InitializeTest();
	ShowEditInterface();
}

void MyFrame::NewLayerButtonClicked(wxCommandEvent& event) {
	xhair.AddLayer(new Plus());
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane();
}

void MyFrame::DeleteLayerButtonClicked(wxCommandEvent& event) {
	xhair.DeleteLayer();
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane();
}

void SaveToFile(const std::wstring& filePath) {

	//getfilename
	std::string filePathStr(filePath.begin(), filePath.end());

	// Open a file for writing
	std::ofstream outFile(filePathStr.c_str());

	// Check if the file is opened successfully
	if (outFile.is_open()) {

		// Write xhair dimensions into file
		outFile << xhair.GetWidth() << std::endl;
		outFile << xhair.GetHeight() << std::endl;

		//Write layer info to file
		for (Component* c : xhair.layers) {
			int type = c->GetType();
			switch (type) {
			case PLUSLAYER: {
				Plus* plus = dynamic_cast<Plus*>(c);
				Pixel color = plus->GetColor();
				Pixel outcolor = plus->GetOutlineColor();

				outFile << type << std::endl;
				outFile << plus->GetName() << std::endl;

				outFile << (int)color.red << std::endl;
				outFile << (int)color.green << std::endl;
				outFile << (int)color.blue << std::endl;
				outFile << (int)color.alpha << std::endl;

				outFile << plus->GetSize() << std::endl;
				outFile << plus->GetWidth() << std::endl;
				outFile << plus->GetGap() << std::endl;

				outFile << plus->GetOutline() << std::endl;
				outFile << plus->GetOutlineThickness() << std::endl;
				outFile << (int)outcolor.red << std::endl;
				outFile << (int)outcolor.green << std::endl;
				outFile << (int)outcolor.blue << std::endl;
				outFile << (int)outcolor.alpha << std::endl;
				outFile << plus->GetOutlineType() << std::endl;

				//outFile << plus->GetSize() << std::endl;

				delete plus;
			}
			default: {
				break;

			}
			}

		}
		// Close the file
		outFile.close();
		//std::cout << "File saved successfully." << std::endl;
	}
	else {
		//std::cerr << "Error: Unable to open file for writing." << std::endl;
	}

	//return 0;

}

int LoadFromFile() {
	OPENFILENAME ofn;       // Structure for the file dialog
	wchar_t szFile[260];    // Buffer to store the file path
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;   // Handle to the parent window
	ofn.lpstrFile = szFile; // Buffer to store the selected file path
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"XHAIR Files(*.xhair)\0 * .xhair\0"; // Filter for file types
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Open the file dialog
	if (GetOpenFileName(&ofn) == TRUE) {
		std::wstring lfile = ofn.lpstrFile;
		std::ifstream inFile(lfile);

		if (inFile.is_open()) {
			std::string line;

			int w;
			int h;

			//read in height and width
			if (std::getline(inFile, line)) {
				w = std::stoi(line);
			}
			else {
				return 1;
			}
			if (std::getline(inFile, line)) {
				h = std::stoi(line);
			}
			else {
				return 2;
			}
			try {
				xhair = Crosshair(w, h);
			}
			catch (const std::exception& e) {
				return 3;
				//failed to create crosshair from file.
			}

			xhair.Initialize({ 255, 255, 255, 0 });
			//xhair.InitializeTest();
			//int selected = -1;
			while (std::getline(inFile, line)) {
				
				int type = std::stoi(line);
				std::string name;
				if (std::getline(inFile, line)) {
					name = line;
				}
				else {
					return 4;
				}
				switch (type) {
				case PLUSLAYER: {

					int d[16];
					for (int i = 0; i < 14; i++) { //16 pieces of data to get from 
						if (std::getline(inFile, line)) {
							d[i] = std::stoi(line);
						}
						else {
							return 5;
						}
					}
					Pixel col = { d[0], d[1], d[2], d[3] };
					Pixel ocol = { d[9], d[10],d[11], d[12] };
					int length = d[4];
					int width = d[5];
					int gap = d[6];
					bool outline = d[7];
					int out_thickness = d[8];
					bool out_type = false;
					if (d[13] > 0) {
						out_type = true;
					}


					Plus* p = new Plus(name, col, length, width, outline, ocol, out_thickness, out_type, gap);

					
					xhair.AddLayer(p);
					//xhair.selectedLayer++;
				}
				}
			}
			inFile.close();

			return 10;
		}

	}
	else {
		//return L""; // Return empty string if the user cancels the dialog
	}
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

void UpdateCrosshairPixels() {
	for (Component* c : xhair.layers) {
		int type = c->GetType();
		OutputDebugString(std::to_wstring(type).c_str());

		if (type == PLUSLAYER) {
			OutputDebugString(L"Drawing a plus\n");
			DrawPlus(c);
		}
	}
}

void MyFrame::SaveButtonClicked(wxCommandEvent& event) {
	OPENFILENAME ofn;       // Structure for the file dialog
	wchar_t szFile[260];    // Buffer to store the file path
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;   // Handle to the parent window
	ofn.lpstrFile = szFile; // Buffer to store the selected file path
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"XHAIR Files(*.xhair)\0 * .xhair\0"; // Filter for file types
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn)) {
		SaveToFile(szFile);
	}
	else {
		OutputDebugString(L"Save failed\n");
	}
}

void MyFrame::UpdateLayerListPane() {
	layersizer->Clear(true);

	wxPanel* llpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(200, 300));
	wxSizer* lsizer = new wxBoxSizer(wxVERTICAL);
	llpanel->SetBackgroundColour(wxColor(100, 100, 100));

	//Layer list
	layerlist = new ScrolledWidgetsPane(llpanel, wxID_ANY, &xhair);
	wxBoxSizer* llsizer = new wxBoxSizer(wxVERTICAL);
	llsizer->Add(layerlist, 1, wxEXPAND | wxALL, 5);

	//Layer Buttons
	wxBoxSizer* lbuttonsizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* deleteLayer = new wxButton(llpanel, BUTTON_DELETELAYER, "Delete");
	wxButton* newLayer = new wxButton(llpanel, BUTTON_NEWLAYER, "New Layer");
	lbuttonsizer->Add(deleteLayer, 1, wxEXPAND | wxALL, 5);
	lbuttonsizer->Add(newLayer, 1, wxEXPAND | wxALL, 5);

	lsizer->Add(llsizer, 1, wxEXPAND | wxALL, 5);
	lsizer->Add(lbuttonsizer, 1, wxEXPAND | wxALL, 5);

	llpanel->SetSizer(lsizer);

	//MyFrame private member layzersizer
	layersizer->Add(llpanel, 1, wxEXPAND | wxALL, 5);

	Layout();
	Refresh();
	Update();
}
void MyFrame::UpdateLayerListPane2() {
	layerListPane->Update();
	Layout();
	Refresh();
	Update();
}

void MyFrame::CreateLayerListPane() {
	layersizer->Clear(true);

	wxPanel* llpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(200, 300));
	wxSizer* lsizer = new wxBoxSizer(wxVERTICAL);
	llpanel->SetBackgroundColour(wxColor(100, 100, 100));

	layerListPane = new LayerListPane(llpanel, &xhair);

	lsizer->Add(layerListPane, 1, wxEXPAND | wxALL, 5);
	llpanel->SetSizer(lsizer);
	layersizer->Add(llpanel, 1, wxEXPAND | wxALL, 5);


	Layout();
	Refresh();
	Update();
}

void MyFrame::UpdateLayerControlPane() {
	OutputDebugString(L"Entering UpdateLayerControlPane\n");
	//controlsizer->Clear(true);
	wxLogDebug(wxT("mainpanel pointer: %p"), mainpanel);
	wxPanel* layercontrolpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(300, 300));
	layercontrolpanel->SetBackgroundColour(wxColor(255, 100, 100));
	wxBoxSizer* layercontrolsizer = new wxBoxSizer(wxVERTICAL);

	OutputDebugString(L"Attempting to Create control panel\n");

	if (xhair.selectedLayer > -1) {
		ControlPanel* control = new ControlPanel(layercontrolpanel, wxID_ANY, &xhair, xhair.selectedLayer);
		OutputDebugString(L"Adding control panel to sizer\n");
		layercontrolsizer->Add(control, 1, wxEXPAND | wxALL, 5);
	}
	OutputDebugString(L"setting sizer\n");
	layercontrolpanel->SetSizer(layercontrolsizer);

	OutputDebugString(L"adding to controlsizer\n");
	controlsizer->Add(layercontrolpanel, 1, wxEXPAND | wxALL, 5);

	OutputDebugString(L"Attempting Layout()\n");
	wxLogDebug(wxT("mainpanel pointer: %p"), mainpanel);
	Layout();
	OutputDebugString(L"Attempting Refresh()\n");
	Refresh();
	OutputDebugString(L"Attempting Update()\n");
	Update();
	OutputDebugString(L"Exiting UpdateLayerControlPane\n");
}

void MyFrame::UpdateLayerControlPane2() {
	controlPanelPane->Update();
	Layout();
	Refresh();
	Update();
}

void MyFrame::CreateLayerControlPane() {
	controlsizer->Clear(true);

	wxPanel* layercontrolpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(300, 300));
	//wxScrolledWindow* layercontrolpanel = new wxScrolledWindow(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(400, 200));
	layercontrolpanel->SetBackgroundColour(wxColor(90, 90, 90));

	controlPanelPane = new ControlPanelPane(layercontrolpanel, wxID_ANY, &xhair, xhair.selectedLayer);
	wxBoxSizer* layercontrolsizer = new wxBoxSizer(wxVERTICAL);
	layercontrolsizer->Add(controlPanelPane, 0, wxEXPAND | wxALL, 5);
	layercontrolpanel->SetSizer(layercontrolsizer);

	OutputDebugString(L"Entering UpdateLayerControlPane\n");

	controlsizer->Add(layercontrolpanel, 0, wxEXPAND | wxALL, 5);
	//layercontrolpanel->FitInside(); // ask the sizer about the needed size
	//layercontrolpanel->SetScrollRate(5, 5);

	Layout();
	Refresh();
	Update();
}

void MyFrame::UpdatePreviewPane() {
	previewsizer->Clear(true);

	wxPanel* parentpreviewpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	wxBoxSizer* pvsizer = new wxBoxSizer(wxVERTICAL);

	parentpreviewpanel->SetSizer(pvsizer);

	//panel for preview image
	wxPanel* previewimgpanel = new wxPanel(parentpreviewpanel, wxID_ANY, wxDefaultPosition, wxSize(100, 100));
	previewimgpanel->SetBackgroundColour(wxColor(90, 90, 90));
	previewimg = new ImagePanel(previewimgpanel, xhair);
	wxBoxSizer* previewimgsizer = new wxBoxSizer(wxHORIZONTAL);
	//previewimgsizer->AddStretchSpacer();
	previewimgsizer->Add(previewimg, 1, wxSHAPED | wxCENTER, 5);
	//previewimgsizer->AddStretchSpacer();
	previewimgpanel->SetSizer(previewimgsizer);

	//panel for preview buttons
	wxPanel* previewbuttonpanel = new wxPanel(parentpreviewpanel, wxID_ANY, wxDefaultPosition, wxSize(150, 150));
	previewbuttonpanel->SetBackgroundColour(wxColor(90, 90, 90));
	saveButton = new wxButton(previewbuttonpanel, BUTTON_SAVE, "Save");
	testButton = new wxButton(previewbuttonpanel, BUTTON_TEST, "Test");

	wxBoxSizer* previewbuttonsizer = new wxBoxSizer(wxHORIZONTAL);
	previewbuttonsizer->Add(saveButton, 1, 0, 5);
	previewbuttonsizer->Add(testButton, 1, 0, 5);
	previewbuttonpanel->SetSizer(previewbuttonsizer);

	pvsizer->Add(previewimgpanel, 1, wxEXPAND | wxALL, 5);
	pvsizer->Add(previewbuttonpanel, 1, wxEXPAND | wxALL, 5);

	previewsizer->Add(parentpreviewpanel, 1, wxEXPAND | wxALL, 5);

	Layout();
	Refresh();
	Update();
}

void MyFrame::TestButtonClicked(wxCommandEvent& event){
	wchar_t debugString[200]; // Buffer for the debug string
	swprintf(debugString, 100, L"%d\n", xhair.selectedLayer);
	OutputDebugString(debugString);
}

void MyFrame::LayerButtonClicked(wxCommandEvent& event) {
	OutputDebugString(L"LayerButtonClicked\n");
	UpdateLayerControlPane2();
}

void MyFrame::SliderChanged(wxCommandEvent& event) {
	OutputDebugString(L"Slider Changed\n");
	UpdatePreviewPane();
}

void MyFrame::OnNumericTextEnter(wxCommandEvent& event) {
	OutputDebugString(L"Text Changed\n");
	UpdatePreviewPane();
}

void MyFrame::OutlineCheckboxClicked(wxCommandEvent& event) {
	OutputDebugString(L"Outline Checkbox Clicked\n");
	UpdatePreviewPane();
}

void MyFrame::VisibilityButtonClicked(wxCommandEvent& event) {
	OutputDebugString(L"Visibility Button Clicked\n");
	UpdatePreviewPane();
}
