#pragma once
#include <wx/wx.h>
#include <wx/wxprec.h>
#include "xhair.h"
#include "definitions.h"


//Panel to show a preview of the crosshair
class ImagePanel : public wxPanel {
public:
    ImagePanel(wxWindow* parent, Crosshair c)
        : wxPanel(parent), crosshair(c){
    }

    void OnPaint(wxPaintEvent& event) {
        wxPaintDC dc(this);
        RenderPixels(dc);
    }

    void DoPaint() {
        wxPaintDC dc(this);
        RenderPixels(dc);
    }


private:
    //std::vector<std::vector<Pixel>>& pixels;
    Crosshair crosshair;

    void RenderPixels(wxDC& dc) {

        for (Component* c : crosshair.layers) {
            switch (c->GetType()) {
            case PLUSLAYER: {
                RenderPlus(c, dc);
            }
            case CIRCLELAYER: {
                RenderCirlce(c, dc);
            }
            case TEXTURELAYER: {
                RenderTexture(c, dc);
            }
            }
        }
    }

    void RenderCirlce(Component* c, wxDC& dc) {

    }
    void RenderTexture(Component* c, wxDC& dc) {

    }

    void RenderPlus(Component* c, wxDC& dc) {
        Plus* plus = dynamic_cast<Plus*>(c);

        int chwidth = crosshair.GetWidth();

        int chheight = crosshair.GetHeight();

        int xcenter = chwidth / 2;
        int ycenter = chheight / 2;

        int pixelWidth = GetSize().GetWidth() / chwidth;
        int pixelHeight = GetSize().GetHeight() / chheight;

        int width = plus->GetWidth();
        int length = plus->GetSize();
        int gap = plus->GetGap();

        Pixel color = plus->GetColor();
        Pixel outline_color = plus->GetOutlineColor();

        wxColour wcolor(color.red, color.green, color.blue, color.alpha);
        wxColour wcolor2(outline_color.red, outline_color.green, outline_color.blue, outline_color.alpha);

        int outline = plus->GetOutlineThickness();

        //OutputDebugString(L"Rendering a plus\n");

        //Each Loop draws one arm, if i or j are outside a certain boundry it draws the outline color instead of the shape color
        for (int i = 0 - outline; i < width + outline; i++) {
            for (int j = 0 - outline; j < length + outline; j++) {
                //pixel to be drawn
                int pixx = xcenter - (width / 2) + i;
                int pixy = ycenter + gap + j;
                //OutputDebugString(L"Trying to Draw a Pixel\n");
                if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                    //OutputDebugString(L"Drawing a pixel\n");
                    dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));
                    if (i < 0 || i >= width || j < 0 || j >= length) {
                        dc.SetBrush(wxBrush(wcolor2));
                        //crosshair.SetColor(pixx, pixy, outline_color);
                    }
                    else {
                        dc.SetBrush(wxBrush(wcolor));
                        //crosshair.SetColor(pixx, pixy, color);
                    }
                    dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                }

            }
        }

        for (int i = 0 - outline; i < width + outline; i++) {
            for (int j = 0 - outline; j < length + outline; j++) {
                //pixel to be drawn
                int pixx = xcenter - (width / 2) + i;
                int pixy = ycenter - gap + j - length;

                if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                    dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));
                    if (i < 0 || i >= width || j < 0 || j >= length) {
                        dc.SetBrush(wxBrush(wcolor2));
                        //crosshair.SetColor(pixx, pixy, outline_color);
                    }
                    else {
                        dc.SetBrush(wxBrush(wcolor));
                        //crosshair.SetColor(pixx, pixy, color);
                    }
                    dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                }

            }
        }

        for (int i = 0 - outline; i < width + outline; i++) {
            for (int j = 0 - outline; j < length + outline; j++) {
                //pixel to be drawn
                int pixx = xcenter - length + j - gap;
                int pixy = ycenter - (width / 2) + i;

                if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                    dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));
                    if (i < 0 || i >= width || j < 0 || j >= length) {
                        dc.SetBrush(wxBrush(wcolor2));
                        //crosshair.SetColor(pixx, pixy, outline_color);
                    }
                    else {
                        dc.SetBrush(wxBrush(wcolor));
                        //crosshair.SetColor(pixx, pixy, color);
                    }
                    dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                }
            }
        }

        for (int i = 0 - outline; i < width + outline; i++) {
            for (int j = 0 - outline; j < length + outline; j++) {
                //pixel to be drawn
                int pixx = xcenter + j + gap;
                int pixy = ycenter - (width / 2) + i;

                if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                    dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));
                    if (i < 0 || i >= width || j < 0 || j >= length) {
                        dc.SetBrush(wxBrush(wcolor2));
                        //crosshair.SetColor(pixx, pixy, outline_color);
                    }
                    else {
                        dc.SetBrush(wxBrush(wcolor));
                        //crosshair.SetColor(pixx, pixy, color);
                    }
                    dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                }
            }
        }
    }
    DECLARE_EVENT_TABLE()
};

//Text box that only takes digits
class NumericTextCtrl : public wxTextCtrl {
public:
    NumericTextCtrl(wxWindow* parent, wxWindowID id = wxID_ANY,
        const wxString& value = wxEmptyString,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = wxTextCtrlNameStr)
        : wxTextCtrl(parent, id, value, pos, size, style | wxTE_RIGHT, validator, name) {
        Bind(wxEVT_CHAR, &NumericTextCtrl::OnChar, this);
    }

    void OnChar(wxKeyEvent& event) {
        int key = event.GetKeyCode();
        if (key < WXK_SPACE || key == WXK_DELETE || key > 127 || (key >= '0' && key <= '9')) {
            event.Skip();
            return;
        }
        if (key == '.' || key == ',') {
            SetValue(GetValue() + wxString::FromAscii("."));
        }
    }
};

//Scrolling list container for the layers
class ScrolledWidgetsPane : public wxScrolledWindow
{

private:
    wxBoxSizer* sizer;
    Crosshair* crosshairptr;
    //std::vector< wxButton*> buttons;

public:
    ScrolledWidgetsPane(wxWindow* parent, wxWindowID id, Crosshair* crosshair) : wxScrolledWindow(parent, id)
    {
        // the sizer will take care of determining the needed scroll size
        // (if you don't use sizers you will need to manually set the viewport size)
        sizer = new wxBoxSizer(wxVERTICAL);

        this->SetSizer(sizer);

        crosshairptr = crosshair;
        PopulateList(crosshairptr);
       

        // this part makes the scrollbars show up
        this->FitInside(); // ask the sizer about the needed size
        this->SetScrollRate(5, 5);
    }

    void PopulateList(Crosshair* crosshair) {
        sizer->Clear(true);

        int id = 0;
        for (Component* c : crosshair->layers)
        {
            wxString n = c->GetName();
            wxButton* b = new wxButton(this, BUTTON_LAYER, wxString::Format(n));

            c->SetID(id);
            
            auto lambdaEventHandler = [this, c](wxCommandEvent& event) {
                OutputDebugString(L"Button Clicked2\n");
                this->crosshairptr->selectedLayer = c->GetID();

                wchar_t debugString[200]; // Buffer for the debug string
                swprintf(debugString, 100, L"%d\n", this->crosshairptr->selectedLayer);
                OutputDebugString(debugString);

                event.Skip();
                };

            b->Bind(wxEVT_BUTTON, lambdaEventHandler);

            id++;

            sizer->Add(b, 0, wxALL, 3);
        }

        this->FitInside(); // ask the sizer about the needed size
        this->SetScrollRate(5, 5);
    }
};

class ColorSlider : public wxPanel {
private:
    wxBoxSizer* sizer;
    uint8_t* color;

public:
    ColorSlider(wxWindow* parent, uint8_t* color, wxString label) : wxPanel(parent), color(color) {

        sizer = new wxBoxSizer(wxHORIZONTAL);
        this->SetSizer(sizer);
        BuildSlider(label);
    }

    void BuildSlider(wxString label) {
        sizer->Clear(true);
        //wxBoxSizer* mainColorSizerRed = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText* clabel = new wxStaticText(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
        wxSlider* cslider = new wxSlider(this, wxID_ANY, *color, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator);
        NumericTextCtrl* cvalue = new NumericTextCtrl(this, wxID_ANY, wxString::Format(wxT("%d"), *color), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        sizer->Add(clabel, 1, wxEXPAND | wxALL, 5);
        sizer->Add(cslider, 1, wxEXPAND | wxALL, 5);
        sizer->Add(cvalue, 1, wxEXPAND | wxALL, 5);
    }
};

class ColorControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Pixel* color;

public:
    ColorControl(wxWindow* parent, Pixel* color) : wxPanel(parent), color(color) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        Build();
    }

    void Build() {
        sizer->Clear(true);
        wxBoxSizer* rs = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new ColorSlider(this, &(color->red), "R"), 1, wxEXPAND | wxALL, 5);

        wxBoxSizer* gs = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new ColorSlider(this, &(color->green), "G"), 1, wxEXPAND | wxALL, 5);

        wxBoxSizer* bs = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new ColorSlider(this, &(color->blue), "B"), 1, wxEXPAND | wxALL, 5);

        wxBoxSizer* as = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new ColorSlider(this, &(color->alpha), "A"), 1, wxEXPAND | wxALL, 5);

    }

};



//Controls for all the layer attributes
class ControlPanel : public wxPanel {
private:
    int type;
    wxBoxSizer* sizer;
    //Component* componentPtr;

public:
    ControlPanel(wxWindow* parent, int t, Component* c) : wxPanel(parent), type(t){
        sizer = new wxBoxSizer(wxVERTICAL);

        //componentPtr = c;

        switch (type) {
        case PLUSLAYER: {
            CreateCrossControl(c);
        }

        }
    }

    void CreateCrossControl(Component* c) {
        sizer->Clear(true);

        OutputDebugString(L"Updating Controls\n");

        //Cast component to cross to access Plus members
        Plus* plus = dynamic_cast<Plus*>(c);

        //Main Color
        wxBoxSizer* parentMainColorSizer = new wxBoxSizer(wxVERTICAL);
        ColorControl* mainColorControl = new ColorControl(this, &(plus->GetColor()));
        sizer->Add(mainColorControl, 1, wxEXPAND | wxALL, 5);

        //length
        wxSlider* lengthslider = new wxSlider(this, wxID_ANY, plus->GetSize(), 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "length");

        sizer->Add(lengthslider, 1, wxEXPAND | wxALL, 5);

        //width
        wxSlider* widthslider = new wxSlider(this, wxID_ANY, plus->GetWidth(), 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "width");

        sizer->Add(widthslider, 1, wxEXPAND | wxALL, 5);

        //gap
        wxSlider* gapslider = new wxSlider(this, wxID_ANY, plus->GetGap(), 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "gap");

        sizer->Add(gapslider, 1, wxEXPAND | wxALL, 5);

        //outline checkbox

        //lazy outline checkbox

        //outline color
        wxSlider* olredslider = new wxSlider(this, wxID_ANY, plus->GetOutlineColor().red, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "Red");
        wxSlider* olgreenslider = new wxSlider(this, wxID_ANY, plus->GetOutlineColor().green, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "Green");
        wxSlider* olblueslider = new wxSlider(this, wxID_ANY, plus->GetOutlineColor().blue, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "Blue");

        sizer->Add(olredslider, 1, wxEXPAND | wxALL, 5);
        sizer->Add(olgreenslider, 1, wxEXPAND | wxALL, 5);
        sizer->Add(olblueslider, 1, wxEXPAND | wxALL, 5);

        //outline thickness
        wxSlider* oltslider = new wxSlider(this, wxID_ANY, plus->GetOutlineThickness(), 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator, "Outline Thickness");

        sizer->Add(oltslider, 1, wxEXPAND | wxALL, 5);

        SetSizer(sizer);

        //return sizer;
    }


};