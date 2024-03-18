// Start of wxWidgets "Hello World" Program
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include "wclasses.h"

Crosshair xhair;

enum
{
    BUTTON_Hello = wxID_HIGHEST + 1, // declares an id which will be used to call our button
    BUTTON_NEW = wxID_HIGHEST + 2,
    BUTTON_LOAD = wxID_HIGHEST + 3,
    BUTTON_SAVE = wxID_HIGHEST + 4,
    BUTTON_TEST = wxID_HIGHEST + 5,
	BUTTON_NEW2 = wxID_HIGHEST + 6
};


class MainApp : public wxApp // MainApp is the class for our application
{
    // MainApp just acts as a container for the window,
public: // or frame in MainFrame
    virtual bool OnInit();
};

class MyFrame : public wxFrame // MainFrame is the class for our window,
{
private:
    wxButton* NewCrosshairButton;
    wxButton* LoadCrosshairButton;
    wxBoxSizer* hbox;
    wxButton* saveButton;
    wxButton* testButton;
	ImagePanel* preview;
	NumericTextCtrl* widthEntry;
	NumericTextCtrl* heightEntry;
	wxButton* confirmNewButton;
	//wxPanel* mainpanel;
	//wxBoxSizer* mainsizer;
	int wwidth;
	int wheight;
	//wxButton*

    // It contains the window and all objects in it
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    

    void OnExit(wxCommandEvent& event);
    void LoadButtonClicked(wxCommandEvent& event);
	void LoadButtonClicked2(wxCommandEvent& event);
    void NewButtonClicked(wxCommandEvent& event);
	void NewButtonClicked2(wxCommandEvent& event);
    void TestButtonClicked(wxCommandEvent& event);

	void ShowInitialInterface() {


		NewCrosshairButton = new wxButton(this, BUTTON_NEW, ("New"));
		LoadCrosshairButton = new wxButton(this, BUTTON_LOAD, ("Load"));

		hbox->Add(NewCrosshairButton, 1, wxEXPAND | wxALL, 5);
		hbox->Add(LoadCrosshairButton, 1, wxEXPAND | wxALL, 5);

		SetSizer(hbox);
	}

    void ShowEditInterface() {
        // Clear current interface
        hbox->Clear(true);
		//mainsizer->Clear(true);

        wxPanel* layerpanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(150, 300));
        wxPanel* layercontrolpanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(150, 300));
        wxPanel* previewpanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(150, 300));
        //wxPanel* pvimg = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(150, 150));
		
		preview = new ImagePanel(this, xhair);
        wxPanel* pvbuttons = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(150, 150));

        wxBoxSizer* mainsizer = new wxBoxSizer(wxHORIZONTAL);
        wxBoxSizer* pvsizer = new wxBoxSizer(wxVERTICAL);
        wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
        

        mainsizer->Add(layerpanel, 1, wxEXPAND | wxALL, 2);
        mainsizer->Add(layercontrolpanel, 1, wxEXPAND | wxALL, 2);

        saveButton = new wxButton(pvbuttons, BUTTON_SAVE, "Save");
        testButton = new wxButton(pvbuttons, BUTTON_TEST, "Test");

        hbox2->Add(saveButton, 1, wxEXPAND | wxALL, 5);
        hbox2->Add(testButton, 1, wxEXPAND | wxALL, 5);
        pvbuttons->SetSizer(hbox2);
        pvsizer->Add(preview, 1, wxEXPAND | wxALL, 0);
        pvsizer->Add(pvbuttons, 1, wxEXPAND | wxALL, 2);

        mainsizer->Add(pvsizer, 1, wxEXPAND | wxALL, 2);

        this->SetSizerAndFit(mainsizer);
    }

	void ShowNewInterface() {
		hbox->Clear(true);
		//mainsizer->Clear(true);

		wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 200));

		//text boxes for height and width dimensions
		widthEntry = new NumericTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		heightEntry = new NumericTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
		confirmNewButton = new wxButton(panel, BUTTON_NEW2, "Save");

		wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
		vbox->Add(widthEntry, 1, wxEXPAND | wxALL, 5);
		vbox->Add(heightEntry, 1, wxEXPAND | wxALL, 5);
		vbox->Add(confirmNewButton, 1, wxEXPAND | wxALL, 5);
		panel->SetSizer(vbox);
		hbox->Add(panel);
		// Set the sizer for the panel
		//this->SetSizerAndFit(mainsizer);

		this->SetSizerAndFit(hbox);
	}

    void NewButtonClicked();

    DECLARE_EVENT_TABLE()
};


int LoadFromFile();
void DrawPlus(Component* c);
void UpdateCrosshairPixels();


wxIMPLEMENT_APP(MainApp);


bool MainApp::OnInit()
{
    MyFrame* frame = new MyFrame("Three Buttons Example", wxPoint(1, 1), wxSize(800, 500));
    frame->Show(true);
    SetTopWindow(frame);

    return TRUE;
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(BUTTON_LOAD, MyFrame::LoadButtonClicked) // Tell the OS to run MainFrame::OnExit when
    EVT_BUTTON(BUTTON_NEW, MyFrame::NewButtonClicked)
	EVT_BUTTON(BUTTON_NEW2, MyFrame::NewButtonClicked2)
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


	hbox = new wxBoxSizer(wxHORIZONTAL);

	ShowInitialInterface();
}

void MyFrame::LoadButtonClicked(wxCommandEvent& event) {
	if (LoadFromFile() == 10) {
		UpdateCrosshairPixels();
		ShowEditInterface();
	}

}


void MyFrame::LoadButtonClicked2(wxCommandEvent& event) {
    // Define file types to be displayed
    wxString wildcard = "xhair files (*xhair)|*.xhair";

    // Open a file dialog with the defined file types
    wxFileDialog openFileDialog(this, _("Open File"), "", "",
        wildcard, wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;     // the user changed their mind

    // Proceed loading the file chosen by the user;
    // this can be done with e.g. wxWidgets input streams:
    wxString filename = openFileDialog.GetPath();
    wxMessageBox(filename, "File Selected", wxOK | wxICON_INFORMATION);
}

void MyFrame::NewButtonClicked(wxCommandEvent& event) {

	//open window to ask for dimensions

	ShowNewInterface();
	//xhair = Crosshair(20, 20);
    //ShowEditInterface();
}
void MyFrame::NewButtonClicked2(wxCommandEvent& event) {
	int h = wxAtoi(heightEntry->GetValue());
	int w = wxAtoi(widthEntry->GetValue());
	xhair = Crosshair(h, w);
	xhair.InitializeTest();
	ShowEditInterface();

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
					int out_type = d[13];


					Plus* p = new Plus(name, col, length, width, outline, ocol, out_thickness, out_type, gap);

					xhair.AddLayer(p);
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




