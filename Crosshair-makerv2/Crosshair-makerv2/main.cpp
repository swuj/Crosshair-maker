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
void SaveToFile(const std::wstring& filePath);
void DrawPlus(Component* c);
void UpdateCrosshairPixels();
void ExportToFile();

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
	void UpdatePreviewPane2();
	void CreatePreviewPane();
	void PopulateLayerButtons(Crosshair crosshair);
	void InsertButton(Crosshair crosshair);
	void RemoveButton(Crosshair crosshair);
	void LayerButtonClicked(wxCommandEvent& event);
	void SliderChanged(wxCommandEvent& event);
	void OnNumericTextEnter(wxCommandEvent& event);
	void OutlineCheckboxClicked(wxCommandEvent& event);
	void VisibilityButtonClicked(wxCommandEvent& event);
	void LayerNameChanged(wxCommandEvent& event);
	void LayerTypeChanged(wxCommandEvent& event);
	void ExportButtonClicked(wxCommandEvent& event);

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
		OutputDebugString(L"Creating LayerListPane\n");
		layersizer = new wxBoxSizer(wxHORIZONTAL);
		//UpdateLayerListPane();
		CreateLayerListPane();
		OutputDebugString(L"Created LayerListPane\n");
		
		mainsizer2->Add(layersizer, 1, wxEXPAND | wxALL, 2);

		/***************************************/
		//LAYER CONTROL PANE
		/***************************************/
		OutputDebugString(L"Creating LayerControlPane\n");
		controlsizer = new wxBoxSizer(wxHORIZONTAL);
		//UpdateLayerControlPane();
		CreateLayerControlPane();
		mainsizer2->Add(controlsizer, 1, wxEXPAND | wxALL, 2);
		OutputDebugString(L"Created LayerControlPane\n");

		/***************************************/
		//PREVIEW PANE
		/***************************************/
		OutputDebugString(L"Creating PreviewPane\n");
		previewsizer = new wxBoxSizer(wxHORIZONTAL);
		//UpdatePreviewPane();
		CreatePreviewPane();
		mainsizer2->Add(previewsizer, 1, wxEXPAND | wxALL, 2);
		OutputDebugString(L"Created PreviewPane\n");

		OutputDebugString(L"Edit interface successfully created\n");

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
	EVT_BUTTON(BUTTON_EXPORT, MyFrame::ExportButtonClicked)
	EVT_COMMAND(TEXT_UPDATE, wxEVT_COMMAND_TEXT_UPDATED, MyFrame::SliderChanged)
	EVT_COMMAND(SLIDER_UPDATE, wxEVT_COMMAND_TEXT_UPDATED, MyFrame::OnNumericTextEnter)
	EVT_COMMAND(CHECKBOX_HASOUTLINE, wxEVT_CHECKBOX, MyFrame::OutlineCheckboxClicked)
	EVT_COMMAND(NAME_UPDATE, wxEVT_COMMAND_TEXT_UPDATED, MyFrame::LayerNameChanged)
	EVT_COMBOBOX(LAYER_TYPE_DROPDOWN, MyFrame::LayerTypeChanged)
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


// INITIAL INTERFACE
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


// LAYER LIST PANE
void MyFrame::NewLayerButtonClicked(wxCommandEvent& event) {
	//xhair.AddLayer(new Plus());
	//xhair.AddLayer(new Plus());
	int type = xhair.typeToAdd;
	switch (type) {
	case PLUSLAYER: {
		xhair.AddLayer(new Plus());
		break;
	}
	case CIRCLELAYER: {
		xhair.AddLayer(new Circle());
		break;
	}
	case TEXTURELAYER: {
		xhair.AddLayer(new Circle());
		break;
	}
	}
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane2();
}

void MyFrame::DeleteLayerButtonClicked(wxCommandEvent& event) {
	xhair.DeleteLayer();
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane2();
}

void MyFrame::VisibilityButtonClicked(wxCommandEvent& event) {
	OutputDebugString(L"Visibility Button Clicked\n");
	UpdatePreviewPane2();
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
	//Refresh();
	//Update();
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

void MyFrame::LayerButtonClicked(wxCommandEvent& event) {
	OutputDebugString(L"LayerButtonClicked\n");
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
}


// CONTROL PANE
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
	//Refresh();
	//Update();
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

void MyFrame::SliderChanged(wxCommandEvent& event) {
	OutputDebugString(L"Slider Changed\n");
	UpdatePreviewPane2();
}

void MyFrame::OnNumericTextEnter(wxCommandEvent& event) {
	OutputDebugString(L"Text Changed\n");
	UpdatePreviewPane2();
}

void MyFrame::OutlineCheckboxClicked(wxCommandEvent& event) {
	OutputDebugString(L"Outline Checkbox Clicked\n");
	UpdatePreviewPane2();
}

void MyFrame::LayerNameChanged(wxCommandEvent& event) {
	OutputDebugString(L"Layer name changed\n");
	UpdateLayerListPane2();
}

// Deletes current layer and adds a layer of selected type
void MyFrame::LayerTypeChanged(wxCommandEvent& event) {
	xhair.DeleteLayer();
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane2();
	int type = xhair.typeToAdd;
	switch (type) {
	case PLUSLAYER: {
		xhair.AddLayer(new Plus());
		break;
	}
	case CIRCLELAYER: {
		xhair.AddLayer(new Circle());
		break;
	}
	case TEXTURELAYER: {
		xhair.AddLayer(new Circle());
		break;
	}
	}
	OutputDebugString(L"Layer type changed\n");

	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane2();

	OutputDebugString(L"Layer type changed2\n");
}


// PREVIEW PANE
void MyFrame::UpdatePreviewPane() {
	previewsizer->Clear(true);

	wxPanel* parentpreviewpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(400, 500));
	wxBoxSizer* pvsizer = new wxBoxSizer(wxVERTICAL);

	parentpreviewpanel->SetSizer(pvsizer);

	//panel for preview image
	wxPanel* previewimgpanel = new wxPanel(parentpreviewpanel, wxID_ANY, wxDefaultPosition, wxSize(100, 100));
	previewimgpanel->SetBackgroundColour(wxColor(90, 90, 90));
	previewimg = new ImagePanel(previewimgpanel, &xhair);
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

void MyFrame::UpdatePreviewPane2() {
	previewPanelPane->Update();
	Layout();
	Refresh();
	Update();
}

void MyFrame::CreatePreviewPane() {
	previewsizer->Clear(true);

	wxPanel* previewpanel = new wxPanel(mainpanel, wxID_ANY, wxDefaultPosition, wxSize(xhair.GetWidth(), xhair.GetHeight()));
	wxBoxSizer* pvsizer = new wxBoxSizer(wxVERTICAL);
	previewpanel->SetSizer(pvsizer);

	previewPanelPane = new PreviewPanelPane(previewpanel, &xhair);
	pvsizer->Add(previewPanelPane, 0, 0, 1);
	previewpanel->SetSizer(pvsizer);

	previewsizer->Add(previewpanel, 0, wxEXPAND | wxALL, 5);

	previewPanelPane->Update();

	Layout();
	Refresh();
	Update();
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

void MyFrame::ExportButtonClicked(wxCommandEvent& event){
	OutputDebugString(L"Export button clicked\n");
	UpdateCrosshairPixels();

	OPENFILENAME ofn;       // Structure for the file dialog
	wchar_t szFile[260];    // Buffer to store the file path
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;   // Handle to the parent window
	ofn.lpstrFile = szFile; // Buffer to store the selected file path
	ofn.lpstrFile[0] = L'\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"PNG Files(*.png)\0 * .png\0"; // Filter for file types
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn)) {
		xhair.SaveAsPng(szFile);
	}
	else {
		OutputDebugString(L"Save failed\n");
	}
}

void MyFrame::TestButtonClicked(wxCommandEvent& event) {
	xhair.DeleteLayer();
	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane2();
	int type = xhair.typeToAdd;
	switch (type) {
	case PLUSLAYER: {
		xhair.AddLayer(new Plus());
		break;
	}
	case CIRCLELAYER: {
		xhair.AddLayer(new Circle());
		break;
	}
	case TEXTURELAYER: {
		xhair.AddLayer(new Circle());
		break;
	}
	}
	OutputDebugString(L"Layer type changed\n");

	UpdateLayerListPane2();
	UpdateLayerControlPane2();
	UpdatePreviewPane2();

	OutputDebugString(L"Layer type changed2\n");
}


// File Handling
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
				outFile << plus->GetVisibility() << std::endl;

				//outFile << plus->GetSize() << std::endl;

				delete plus;
				break;
			}
			case CIRCLELAYER: {
				Circle* ptr = dynamic_cast<Circle*>(c);
				Pixel color = ptr->GetColor();
				Pixel outcolor = ptr->GetOutlineColor();
				Pixel inneroutcolor = ptr->GetInnerOutlineColor();

				outFile << type << std::endl;
				outFile << ptr->GetName() << std::endl;
				outFile << (int)color.red << std::endl;
				outFile << (int)color.green << std::endl;
				outFile << (int)color.blue << std::endl;
				outFile << (int)color.alpha << std::endl;
				outFile << ptr->GetRadius() << std::endl;
				outFile << ptr->GetInnerRadius() << std::endl;
				outFile << ptr->GetOutline() << std::endl;
				outFile << ptr->GetOutlineThickness() << std::endl;
				outFile << (int)outcolor.red << std::endl;
				outFile << (int)outcolor.green << std::endl;
				outFile << (int)outcolor.blue << std::endl;
				outFile << (int)outcolor.alpha << std::endl;
				outFile << ptr->GetInnerOutline() << std::endl;
				outFile << ptr->GetInnerOutlineThickness() << std::endl;
				outFile << (int)inneroutcolor.red << std::endl;
				outFile << (int)inneroutcolor.green << std::endl;
				outFile << (int)inneroutcolor.blue << std::endl;
				outFile << (int)inneroutcolor.alpha << std::endl;
				outFile << ptr->GetVisibility() << std::endl;
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

		OutputDebugString(L"Loading a crosshair from file\n");

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

			OutputDebugString(L"Initializing crosshair\n");

			xhair.Initialize({ 0, 0, 0, 0});
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
				case CIRCLELAYER: {
					OutputDebugString(L"Creating circle layer\n");
					int d[21];
					for (int i = 0; i < 19; i++) { //19 pieces of data to get from
						if (std::getline(inFile, line)) {
							d[i] = std::stoi(line);
						}
						else {
							return 5;
						}
					}
					OutputDebugString(L"Data read to array\n");
					Pixel col = { d[0], d[1], d[2], d[3] };
					OutputDebugString(L"Set color\n");
					Pixel ocol = { d[8], d[9],d[10], d[11] };
					OutputDebugString(L"Set outline color\n");
					int radius = d[4];
					OutputDebugString(L"Set radius\n");
					int innerradius = d[5];
					OutputDebugString(L"Set inner radius\n");
					bool outline = d[6];
					OutputDebugString(L"Set outline\n");
					int out_thickness = d[7];
					OutputDebugString(L"Set outline thickness\n");
					bool inner_outline = d[12];
					OutputDebugString(L"Set outline type\n");
					int inner_outline_thickness = d[13];
					OutputDebugString(L"Set inner outline thickness\n");
					Pixel iocol = { d[14], d[15], d[16], d[17] };
					bool visible = d[18];
					OutputDebugString(L"Set visibility\n");


					Circle* c = new Circle(name, col, radius, innerradius, outline, out_thickness, ocol, inner_outline, inner_outline_thickness, iocol, visible);
					OutputDebugString(L"Created circle\n");

					OutputDebugString(L"Adding circle to crosshair\n");
					xhair.AddLayer(c);
					/*xhair.selectedLayer++;
					OutputDebugString(L"setting ID\n");
					p->SetID(xhair.selectedLayer);*/
					OutputDebugString(L"Layer successfully added\n");
					break;
				}
				case PLUSLAYER: {
					OutputDebugString(L"Creating plus layer\n");
					int d[17];
					for (int i = 0; i < 15; i++) { //16 pieces of data to get from
						if (std::getline(inFile, line)) {
							d[i] = std::stoi(line);
						}
						else {
							return 5;
						}
					}
					OutputDebugString(L"Data read to array\n");
					Pixel col = { d[0], d[1], d[2], d[3] };
					OutputDebugString(L"Set color\n");
					Pixel ocol = { d[9], d[10],d[11], d[12] };
					OutputDebugString(L"Set outline color\n");
					int length = d[4];
					OutputDebugString(L"Set length\n");
					int width = d[5];
					OutputDebugString(L"Set width\n");
					int gap = d[6];
					OutputDebugString(L"Set gap\n");
					bool outline = d[7];
					OutputDebugString(L"Set outline\n");
					int out_thickness = d[8];
					OutputDebugString(L"Set outline thickness\n");
					bool out_type = false;
					if (d[13] > 0) {
						out_type = true;
					}
					OutputDebugString(L"Set outline type\n");
					bool visible = d[14];
					OutputDebugString(L"Set visibility\n");


					Plus* p = new Plus(name, col, length, width, gap, outline, out_thickness, ocol, out_type, visible);
					OutputDebugString(L"Created plus\n");

					OutputDebugString(L"Adding plus to crosshair\n");
					xhair.AddLayer(p);
					/*xhair.selectedLayer++;
					OutputDebugString(L"setting ID\n");
					p->SetID(xhair.selectedLayer);*/
					OutputDebugString(L"Layer successfully added\n");
				}
				}
			}
			inFile.close();
			OutputDebugString(L"Crosshair successfully read from file\n");

			return 10;
		}

	}
	else {
		//return L""; // Return empty string if the user cancels the dialog
	}
}

void DrawPlus(Component* c) {

	Plus* plus = dynamic_cast<Plus*>(c);

	int chwidth = xhair.GetWidth();

	int chheight = xhair.GetHeight();

	int xcenter = chwidth / 2;
	int ycenter = chheight / 2;


	//if 1 it will draw true size
	/*int pixelWidth = GetSize().GetWidth() / chwidth;
	int pixelHeight = GetSize().GetHeight() / chheight;*/
	int pixelWidth = 1;
	int pixelHeight = 1;


	int width = plus->GetWidth();
	int length = plus->GetSize();
	int gap = plus->GetGap();

	Pixel color = plus->GetColor();
	Pixel outline_color = plus->GetOutlineColor();

	int outline = plus->GetOutlineThickness() * plus->GetOutline();

	if (plus->GetOutlineType()) {
		//Each Loop draws one arm, if i or j are outside a certain boundry it draws the outline color instead of the shape color
		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				//pixel to be drawn
				int pixx = xcenter - (width / 2) + i;
				int pixy = ycenter + gap + j;
				//OutputDebugString(L"Trying to Draw a Pixel\n");
				if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
					//OutputDebugString(L"Drawing a pixel\n");
					if (i < 0 || i >= width || j < 0 || j >= length) {
						xhair.SetColor(pixx, pixy, outline_color);
					}
					else {
						xhair.SetColor(pixx, pixy, color);
					}
				}

			}
		}

		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				//pixel to be drawn
				int pixx = xcenter - (width / 2) + i;
				int pixy = ycenter - gap + j - length;

				if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
					if (i < 0 || i >= width || j < 0 || j >= length) {
						xhair.SetColor(pixx, pixy, outline_color);
					}
					else {
						xhair.SetColor(pixx, pixy, color);
					}
				}

			}
		}

		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				//pixel to be drawn
				int pixx = xcenter - length + j - gap;
				int pixy = ycenter - (width / 2) + i;

				if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
					if (i < 0 || i >= width || j < 0 || j >= length) {
						xhair.SetColor(pixx, pixy, outline_color);
					}
					else {
						xhair.SetColor(pixx, pixy, color);
					}
				}
			}
		}

		for (int i = 0 - outline; i < width + outline; i++) {
			for (int j = 0 - outline; j < length + outline; j++) {
				//pixel to be drawn
				int pixx = xcenter + j + gap;
				int pixy = ycenter - (width / 2) + i;

				if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
					if (i < 0 || i >= width || j < 0 || j >= length) {
						xhair.SetColor(pixx, pixy, outline_color);
					}
					else {
						xhair.SetColor(pixx, pixy, color);
					}
				}
			}
		}

	}
	else {
		Pixel col = plus->GetOutlineColor();
		for (int pass = 1; pass >= 0; pass--) {
			if (!pass) {
				col = plus->GetColor();
				
			}

			//Each Loop draws one arm, if i or j are outside a certain boundry it draws the outline color instead of the shape color
			for (int i = 0 - (outline * pass); i < width + (outline * pass); i++) {
				for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
					//pixel to be drawn
					int pixx = xcenter - (width / 2) + i;
					int pixy = ycenter + gap + j;
					//OutputDebugString(L"Trying to Draw a Pixel\n");
					if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
						//OutputDebugString(L"Drawing a pixel\n");
						xhair.SetColor(pixx, pixy, col);
					}

				}
			}

			for (int i = 0 - (outline * pass); i < width + (outline * pass); i++) {
				for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
					//pixel to be drawn
					int pixx = xcenter - (width / 2) + i;
					int pixy = ycenter - gap + j - length;

					if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
						xhair.SetColor(pixx, pixy, col);
					}

				}
			}

			for (int i = 0 - (outline * pass); i < width + (outline * pass); i++) {
				for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
					//pixel to be drawn
					int pixx = xcenter - length + j - gap;
					int pixy = ycenter - (width / 2) + i;

					if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
						xhair.SetColor(pixx, pixy, col);
					}
				}
			}

			for (int i = 0 - (outline * pass); i < width + (outline * pass); i++) {
				for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
					//pixel to be drawn
					int pixx = xcenter + j + gap;
					int pixy = ycenter - (width / 2) + i;

					if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
						xhair.SetColor(pixx, pixy, col);
					}
				}
			}
		}
	}
}

void circleBres(int r, std::vector<int>* vec)
{
	int x = 0, y = r;
	int d = 3 - 2 * r;
	while (y >= x)
	{
		x++;

		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
			d = d + 4 * x + 6;
		vec->push_back(y);
	}
}

void DrawCircle(Component* comp) {
	Circle* circle = dynamic_cast<Circle*>(comp);

	int chwidth = xhair.GetWidth();
	int chheight = xhair.GetHeight();

	int xcenter = chwidth / 2;
	int ycenter = chheight / 2;

	int radius = circle->GetRadius();
	int inner_radius = circle->GetInnerRadius();
	int gap = circle->GetGap();

	Pixel color = circle->GetColor();
	Pixel outline_color = circle->GetOutlineColor();
	//Pixel inner_outline_color = circle->GetInnerOutlineColor();
	Pixel inner_outline_color = circle->GetOutlineColor();

	int outline = circle->GetOutlineThickness() * circle->GetOutline();
	int inner_outline = std::min(inner_radius, circle->GetInnerOutlineThickness()) * circle->GetInnerOutline();

	//split into sub circles for each section: outline, main, inner outline, empty
	//based on the outermost edge of each section

	int total_radius = radius + outline;
	int empty_radius = gap - inner_outline;

	std::vector<int> olBoundry;
	std::vector<int> mainBoundry;
	std::vector<int> iolBoundry;
	std::vector<int> emptyBoundry;

	circleBres(total_radius, &olBoundry);
	circleBres(radius, &mainBoundry);
	circleBres(gap, &iolBoundry);
	circleBres(empty_radius, &emptyBoundry);

	for (int a = 0; a < olBoundry.size(); a++) {
		int i = a;
		if (a < emptyBoundry.size()) {
			for (i; i < emptyBoundry[a]; i++) {

			}
		}
		if (a < iolBoundry.size()) {
			for (i; i < iolBoundry[a]; i++) {
				xhair.SetColor((xcenter + a), (ycenter + i), inner_outline_color);
				xhair.SetColor((xcenter + a), (ycenter - i - 1), inner_outline_color);
				xhair.SetColor((xcenter - a - 1), (ycenter + i), inner_outline_color);
				xhair.SetColor((xcenter - a - 1), (ycenter - i - 1), inner_outline_color);
				xhair.SetColor((xcenter + i), (ycenter + a), inner_outline_color);
				xhair.SetColor((xcenter + i), (ycenter - a - 1), inner_outline_color);
				xhair.SetColor((xcenter - i - 1), (ycenter + a), inner_outline_color);
				xhair.SetColor((xcenter - i - 1), (ycenter - a - 1), inner_outline_color);
			}
		}
		if (a < mainBoundry.size()) {
			for (i; i < mainBoundry[a]; i++) {
					xhair.SetColor((xcenter + a), (ycenter + i), color);
					xhair.SetColor((xcenter + a), (ycenter - i - 1), color);
					xhair.SetColor((xcenter - a - 1), (ycenter + i), color);
					xhair.SetColor((xcenter - a - 1), (ycenter - i - 1), color);
					xhair.SetColor((xcenter + i), (ycenter + a), color);
					xhair.SetColor((xcenter + i), (ycenter - a - 1), color);
					xhair.SetColor((xcenter - i - 1), (ycenter + a), color);
					xhair.SetColor((xcenter - i - 1), (ycenter - a - 1), color);
			}
		}
		if (a < olBoundry.size()) {
			for (i; i < olBoundry[a]; i++) {
					xhair.SetColor((xcenter + a), (ycenter + i), outline_color);
					xhair.SetColor((xcenter + a), (ycenter - i - 1), outline_color);
					xhair.SetColor((xcenter - a - 1), (ycenter + i), outline_color);
					xhair.SetColor((xcenter - a - 1), (ycenter - i - 1), outline_color);
					xhair.SetColor((xcenter + i), (ycenter + a), outline_color);
					xhair.SetColor((xcenter + i), (ycenter - a - 1), outline_color);
					xhair.SetColor((xcenter - i - 1), (ycenter + a), outline_color);
					xhair.SetColor((xcenter - i - 1), (ycenter - a - 1), outline_color);
			}
		}
	}
}

void UpdateCrosshairPixels() {
	for (Component* c : xhair.layers) {
		int type = c->GetType();
		OutputDebugString(std::to_wstring(type).c_str());
		if (!c->GetVisibility()) {
			continue;
		}

		switch(type){
		case PLUSLAYER: {
			OutputDebugString(L"Drawing a plus\n");
			DrawPlus(c);
			break;
		}
		case CIRCLELAYER: {
			OutputDebugString(L"Drawing a circle\n");
			DrawCircle(c);
			break;
		}

			
		}
	}
}