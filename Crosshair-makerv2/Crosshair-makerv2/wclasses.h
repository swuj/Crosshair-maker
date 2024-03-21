#pragma once
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/slider.h>
#include "xhair.h"
#include "definitions.h"
#include <wx/collpane.h>


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

        int outline = plus->GetOutlineThickness()*plus->GetOutline();

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
    int min; 
    int max;


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

        wxStaticText* clabel = new wxStaticText(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
        wxSlider* cslider = new wxSlider(this, SLIDER_UPDATE, *color, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator);
        NumericTextCtrl* cvalue = new NumericTextCtrl(this, TEXT_UPDATE, wxString::Format(wxT("%d"), *color), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);


        //Slider and text box update eachother then tell preview window to update
        cslider->Bind(wxEVT_SCROLL_CHANGED, [this, color, cvalue, cslider](wxScrollEvent& event) {
            *color = cslider->GetValue();
            cvalue->SetValue(wxString::Format(wxT("%d"), cslider->GetValue()));

            wxCommandEvent evt(wxEVT_SCROLL_CHANGED, SLIDER_UPDATE);
            ProcessEvent(evt);
            //event.Skip();
            });
            
        cvalue->Bind(wxEVT_TEXT_ENTER, [this, color, cvalue, cslider](wxCommandEvent& event) {
            cslider->SetValue(wxAtoi(cvalue->GetValue()));
            *color = cslider->GetValue();

            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, TEXT_UPDATE);
            ProcessEvent(evt);
            });

        sizer->Add(clabel, 1, wxEXPAND | wxALL, 5);
        sizer->Add(cslider, 1, wxEXPAND | wxALL, 5);
        sizer->Add(cvalue, 1, wxEXPAND | wxALL, 5);
    }
};

class IntSlider : public wxPanel {
private:
    wxBoxSizer* sizer;
    int* val;
    int max;

public:
    IntSlider(wxWindow* parent, int* val, wxString label, int x, int y) : wxPanel(parent), val(val) {
        max = std::max(x, y);

        sizer = new wxBoxSizer(wxHORIZONTAL);
        this->SetSizer(sizer);

        wxStaticText* clabel = new wxStaticText(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
        wxSlider* cslider = new wxSlider(this, SLIDER_UPDATE, *val, 0, max, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator);
        NumericTextCtrl* cvalue = new NumericTextCtrl(this, TEXT_UPDATE, wxString::Format(wxT("%d"), *val), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);


        //Slider and text box update eachother
        cslider->Bind(wxEVT_SCROLL_CHANGED, [this, val, cvalue, cslider](wxScrollEvent& event) {
            *val = cslider->GetValue();
            cvalue->SetValue(wxString::Format(wxT("%d"), cslider->GetValue()));

            wxCommandEvent evt(wxEVT_SCROLL_CHANGED, SLIDER_UPDATE);
            ProcessEvent(evt);
            //event.Skip();
            });

        cvalue->Bind(wxEVT_TEXT_ENTER, [this, val, cvalue, cslider](wxCommandEvent& event) {
            cslider->SetValue(wxAtoi(cvalue->GetValue()));
            *val = cslider->GetValue();

            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, TEXT_UPDATE);
            ProcessEvent(evt);
            });

        sizer->Add(clabel, 1, wxEXPAND | wxALL, 5);
        sizer->Add(cslider, 1, wxEXPAND | wxALL, 5);
        sizer->Add(cvalue, 1, wxEXPAND | wxALL, 5);
    }
};

class ColorControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Pixel* color;
    wxWindow* parent;

public:
    ColorControl(wxWindow* parent, Pixel* color, wxString label) : wxPanel(parent), color(color), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize);
        wxWindow* win = pane->GetPane();

        wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

        ColorSlider* redSlider = new ColorSlider(win, &(color->red), "R");
        sizer2->Add(redSlider, 1, wxEXPAND | wxALL, 5);

        ColorSlider* greenSlider = new ColorSlider(win, &(color->green), "G");
        sizer2->Add(greenSlider, 1, wxEXPAND | wxALL, 5);

        ColorSlider* blueSlider = new ColorSlider(win, &(color->blue), "B");
        sizer2->Add(blueSlider, 1, wxEXPAND | wxALL, 5);

        ColorSlider* alphaSlider = new ColorSlider(win, &(color->alpha), "A");
        sizer2->Add(alphaSlider, 1, wxEXPAND | wxALL, 5);
        
        win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &ColorControl::OnPaneChanged, this);
    }

    void OnPaneChanged(wxCollapsiblePaneEvent& event) {
        // Get the collapsible pane
        wxCollapsiblePane* pane = dynamic_cast<wxCollapsiblePane*>(event.GetEventObject());
        if (!pane)
            return;

        // Get the parent sizer
        wxSizer* parentSizer = parent->GetSizer();
        if (!parentSizer)
            return;

        // Adjust the parent sizer's layout based on whether the collapsible pane is collapsed or expanded
        if (pane->IsCollapsed()) {
            parentSizer->Hide(pane->GetPane());
        }
        else {
            parentSizer->Show(pane->GetPane());
        }

        // Recalculate the layout
        parentSizer->Layout();
    }
};

class PlusControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Plus* p;
    wxWindow* parent;

public:
    PlusControl(wxWindow* parent, Plus* plus, int x, int y) : wxPanel(parent), p(plus), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, "Dimensions", wxDefaultPosition, wxDefaultSize);
        wxWindow* win = pane->GetPane();

        wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

        wxBoxSizer* lengths = new wxBoxSizer(wxHORIZONTAL);
        sizer2->Add(new IntSlider(win, &(p->GetSize()), "Length", x, y), 1, wxEXPAND | wxALL, 5);

        wxBoxSizer* widths = new wxBoxSizer(wxHORIZONTAL);
        sizer2->Add(new IntSlider(win, &(p->GetWidth()), "Width", x, y), 1, wxEXPAND | wxALL, 5);

        wxBoxSizer* gaps = new wxBoxSizer(wxHORIZONTAL);
        sizer2->Add(new IntSlider(win, &(p->GetGap()), "Gap", x, y), 1, wxEXPAND | wxALL, 5);

        win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &PlusControl::OnPaneChanged, this);
    }

    void OnPaneChanged(wxCollapsiblePaneEvent& event) {
        // Get the collapsible pane
        wxCollapsiblePane* pane = dynamic_cast<wxCollapsiblePane*>(event.GetEventObject());
        if (!pane)
            return;

        // Get the parent sizer
        wxSizer* parentSizer = parent->GetSizer();
        if (!parentSizer)
            return;

        // Adjust the parent sizer's layout based on whether the collapsible pane is collapsed or expanded
        if (pane->IsCollapsed()) {
            parentSizer->Hide(pane->GetPane());
        }
        else {
            parentSizer->Show(pane->GetPane());
        }

        // Recalculate the layout
        parentSizer->Layout();
    }

};

class OutLineControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Plus* p;
    wxWindow* parent;

public:
    OutLineControl(wxWindow* parent, Plus* plus, int x, int y) : wxPanel(parent), p(plus), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        wxCollapsiblePane* pane = new wxCollapsiblePane(this, CHECKBOX_HASOUTLINE, "OutLine", wxDefaultPosition, wxDefaultSize);
        wxWindow* win = pane->GetPane();
        wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);


        wxCheckBox* check = new wxCheckBox(win, wxID_ANY, "Show outline");
        check->SetValue(&(plus->GetOutline()));
        check->Bind(wxEVT_CHECKBOX, [this, plus, check](wxCommandEvent& event) {
            // Toggle the value of showoutline
            plus->ToggleOutline();

            // Update the state of the checkbox
            check->SetValue(plus->GetOutline());
            wxCommandEvent evt(wxEVT_CHECKBOX, CHECKBOX_HASOUTLINE);
            ProcessEvent(evt);
            });


        sizer2->Add(check, 1, wxEXPAND | wxALL, 5);

        sizer2->Add(new wxStaticText(win, wxID_ANY, "Thickness"));
        sizer2->Add(new IntSlider(win, &(p->GetOutlineThickness()), "", x, y), 1, wxEXPAND | wxALL, 5);

        Pixel* color = &(plus->GetOutlineColor());
        sizer2->Add(new wxStaticText(win, wxID_ANY, "Color"));
        sizer2->Add(new ColorSlider(win, &(color->red), "R"), 1, wxEXPAND | wxALL, 5);
        sizer2->Add(new ColorSlider(win, &(color->green), "G"), 1, wxEXPAND | wxALL, 5);
        sizer2->Add(new ColorSlider(win, &(color->blue), "B"), 1, wxEXPAND | wxALL, 5);
        sizer2->Add(new ColorSlider(win, &(color->alpha), "A"), 1, wxEXPAND | wxALL, 5);

        win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &OutLineControl::OnPaneChanged, this);
    }

    void OnPaneChanged(wxCollapsiblePaneEvent& event) {
        // Get the collapsible pane
        wxCollapsiblePane* pane = dynamic_cast<wxCollapsiblePane*>(event.GetEventObject());
        if (!pane)
            return;

        // Get the parent sizer
        wxSizer* parentSizer = parent->GetSizer();
        if (!parentSizer)
            return;

        // Adjust the parent sizer's layout based on whether the collapsible pane is collapsed or expanded
        if (pane->IsCollapsed()) {
            parentSizer->Hide(pane->GetPane());
        }
        else {
            parentSizer->Show(pane->GetPane());
        }

        // Recalculate the layout
        parentSizer->Layout();
    }

};
//Controls for all the layer attributes
class ControlPanel : public wxPanel {
private:
    int type;
    wxBoxSizer* sizer;
    int x;
    int y;
    //Component* componentPtr;

public:
    ControlPanel(wxWindow* parent, int id, Crosshair* c, int layer) : wxPanel(parent){
        sizer = new wxBoxSizer(wxVERTICAL);

        //wxScrolledWindow* controlWindow = new wxScrolledWindow(this, wxID_ANY);
        //controlWindow->SetSizer(sizer);
        x = c->GetWidth();
        y = c->GetHeight();

        //componentPtr = c;

        Component* component = c->layers[layer];
        type = component->GetType();

        switch (type) {
        case PLUSLAYER: {
            CreateCrossControl(component);
        }

        }
        //this->FitInside(); // ask the sizer about the needed size
        //this->SetScrollRate(5, 5);
    }

    void CreateCrossControl(Component* c) {
        sizer->Clear(true);

        OutputDebugString(L"Updating Controls\n");

        //Cast component to cross to access Plus members
        Plus* plus = dynamic_cast<Plus*>(c);

        //Main Color
        ColorControl* mainColorControl = new ColorControl(this, &(plus->GetColor()), "Color");
        sizer->Add(mainColorControl, 1, wxEXPAND | wxALL, 5);

        //Dimensions
        PlusControl* dimensionControl = new PlusControl(this, plus, x, y);
        sizer->Add(dimensionControl, 1, wxEXPAND | wxALL, 5);

        //outline checkbox
        OutLineControl* outlineControl = new OutLineControl(this, plus, x, y);
        sizer->Add(outlineControl, 1, wxEXPAND | wxALL, 5);

        SetSizer(sizer);

        //return sizer;
    }
};