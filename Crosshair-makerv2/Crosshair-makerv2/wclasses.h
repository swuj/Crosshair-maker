#pragma once
#include <wx/wx.h>
#include <wx/wxprec.h>
#include "xhair.h"

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
        int width = crosshair.GetWidth();
        if (width == 0) return;

        int height = crosshair.GetHeight();

        // Calculate the size of each pixel in the panel
        int pixelWidth = GetSize().GetWidth() / width;
        int pixelHeight = GetSize().GetHeight() / height;

        for (size_t i = 0; i < width; ++i) {
            for (size_t j = 0; j < height; ++j) {
                const Pixel& pixel = crosshair.GetPixels()[i][j];
                // Calculate position and size of the rectangle representing the pixel
                int x = i * pixelWidth;
                int y = j * pixelHeight;
                int w = pixelWidth;
                int h = pixelHeight;

                // Create a wxColour based on the pixel's color
                wxColour color(pixel.red, pixel.green, pixel.blue, pixel.alpha);
                wxColour color2(0,0,0,0);

                // Set the brush color to the pixel's color
                dc.SetBrush(wxBrush(color));
                dc.SetPen(wxPen(color2, 0, wxPENSTYLE_TRANSPARENT));

                // Draw the filled rectangle representing the pixel
                dc.DrawRectangle(x, y, w, h);
            }
        }
    }
    DECLARE_EVENT_TABLE()
};


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


class ScrolledWidgetsPane : public wxScrolledWindow
{

private:
    wxBoxSizer* sizer;

public:
    ScrolledWidgetsPane(wxWindow* parent, wxWindowID id) : wxScrolledWindow(parent, id)
    {
        // the sizer will take care of determining the needed scroll size
        // (if you don't use sizers you will need to manually set the viewport size)
        sizer = new wxBoxSizer(wxVERTICAL);

        // add a series of widgets
        for (int w = 1; w <= 120; w++)
        {
            wxButton* b = new wxButton(this, wxID_ANY, wxString::Format(wxT("Button %i"), w));
            sizer->Add(b, 0, wxALL, 3);
        }

        this->SetSizer(sizer);

        // this part makes the scrollbars show up
        this->FitInside(); // ask the sizer about the needed size
        this->SetScrollRate(5, 5);
    }

    void PopulateList(Crosshair crosshair) {
        sizer->Clear(true);

        for (int w = 1; w <= 120; w++)
        {
            wxButton* b = new wxButton(this, wxID_ANY, wxString::Format(wxT("Button %i"), w));
            sizer->Add(b, 0, wxALL, 3);
        }
    }
};
