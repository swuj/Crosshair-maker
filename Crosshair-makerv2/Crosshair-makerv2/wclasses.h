#pragma once
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/slider.h>
#include "xhair.h"
#include "definitions.h"
#include <wx/collpane.h>
#include <numeric>


//Panel to show a preview of the crosshair, renders the layers
class ImagePanel : public wxPanel {
public:
    ImagePanel(wxWindow* parent, Crosshair *c)
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
    Crosshair* crosshair;
    //std::vector<std::vector<Pixel>>& pixels;
    

    void RenderPixels(wxDC& dc) {

        for (Component* c : crosshair->layers) {
            if (!c->GetVisibility()) {
                OutputDebugString(L"layer not visible\n");
                continue;
            }
            int type = c->GetType();
            OutputDebugString(L"Entering Render switch\n");
            switch (type) {
            case PLUSLAYER: {
                OutputDebugString(L"Rendering a Plus\n");
                RenderPlus(c, dc);
                break;
            }
            case XLAYER: {
                OutputDebugString(L"Rendering an X\n");
                RenderX(c, dc);
                break;
            }
            case CIRCLELAYER: {
                OutputDebugString(L"Rendering a Circle\n");
                RenderCirlce(c, dc);
                break;
            }
            case RECTLAYER: {
                OutputDebugString(L"Rendering a Rectangle\n");
                RenderRectangle(c, dc);
                break;
            }
            case DIAMONDLAYER: {
                OutputDebugString(L"Rendering a Rectangle\n");
                RenderDiamond(c, dc);
                break;
            }
            }
        }
        RenderCenterline(dc);
    }
    /********************/
    //Rendering Functions
    /********************/
    void RenderCenterline(wxDC& dc) {
        int chwidth = crosshair->GetWidth();
        int chheight = crosshair->GetHeight();

        int xcenter = chwidth / 2;
        int ycenter = chheight / 2;

        wxColour wcolor(255, 0, 0, 255);
        dc.SetBrush(wxBrush(wcolor));

        for (int i = 0; i < chwidth; i++) {
            dc.DrawRectangle(i, ycenter , 1, 1);

        }
        for (int i = 0; i < chheight; i++) {
            dc.DrawRectangle(xcenter, i, 1, 1);

        }

    }
    void RenderCirlce(Component* comp, wxDC& dc) {

        Circle* circle = dynamic_cast<Circle*>(comp);

        int chwidth = crosshair->GetWidth();
        int chheight = crosshair->GetHeight();

        int xcenter = chwidth / 2;
        int ycenter = chheight / 2;
        //if 1 it will draw true size
        /*int pixelWidth = GetSize().GetWidth() / chwidth;
        int pixelHeight = GetSize().GetHeight() / chheight;
        */
        int pixelWidth = 1;
        int pixelHeight = 1;

        int radius = circle->GetRadius();
        int inner_radius = circle->GetInnerRadius();
        int gap = circle->GetGap();

        Pixel color = circle->GetColor();
        Pixel outline_color = circle->GetOutlineColor();
        //Pixel inner_outline_color = circle->GetInnerOutlineColor();
        Pixel inner_outline_color = circle->GetOutlineColor();

        wxColour wcolor(color.red, color.green, color.blue, color.alpha);
        wxColour wcolor2(outline_color.red, outline_color.green, outline_color.blue, outline_color.alpha);
        wxColour wcolor3(inner_outline_color.red, inner_outline_color.green, inner_outline_color.blue, inner_outline_color.alpha);

        int outline = circle->GetOutlineThickness() * circle->GetOutline();
        int inner_outline = std::min(inner_radius, circle->GetInnerOutlineThickness()) * circle->GetInnerOutline();

        //split into sub circles for each section: outline, main, inner outline, empty
        //based on the outermost edge of each section

        int total_radius = radius + outline;
        int empty_radius = gap - inner_outline;

        /*std::vector<int> olBoundry(total_radius);
        std::vector<int> mainBoundry(radius);
        std::vector<int> iolBoundry(gap);
        std::vector<int> emptyBoundry(empty_radius);*/

        std::vector<int> olBoundry;
        std::vector<int> mainBoundry;
        std::vector<int> iolBoundry;
        std::vector<int> emptyBoundry;

        circleBres(total_radius, &olBoundry);
        circleBres(radius, &mainBoundry);
        circleBres(gap, &iolBoundry);
        circleBres(empty_radius, &emptyBoundry);

        dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));
        //dc.SetBrush(wxBrush(wcolor2, 0, wxBRUSHSTYLE_TRANSPARENT));

        //see center pixel

        //dc.SetBrush(wxBrush({255, 0, 255, 255}));
        //dc.DrawRectangle((xcenter) * pixelWidth, (ycenter) * pixelHeight, pixelWidth, pixelHeight);


        for (int a = 0; a < olBoundry.size(); a++) {
            int i = a;
            if (a<emptyBoundry.size()) {
                for (i; i < emptyBoundry[a]; i++) {
                    //dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                }
            }
            if (a < iolBoundry.size()) {
                dc.SetBrush(wxBrush(wcolor3));
                for (i; i < iolBoundry[a]; i++) {
                    if (a == 0) {

                    }
                    if (i == 0) {

                    }
                    dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter - i - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - a - 1) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - a - 1) * pixelWidth, (ycenter - i - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + i) * pixelWidth, (ycenter + a) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + i) * pixelWidth, (ycenter - a - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - i - 1) * pixelWidth, (ycenter + a) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - i - 1) * pixelWidth, (ycenter - a - 1) * pixelHeight, pixelWidth, pixelHeight);
                }
            }
            if (a < mainBoundry.size()) {
                dc.SetBrush(wxBrush(wcolor));
                for (i; i < mainBoundry[a]; i++) {
                    dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter - i - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - a - 1) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - a - 1) * pixelWidth, (ycenter - i - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + i) * pixelWidth, (ycenter + a) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + i) * pixelWidth, (ycenter - a - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - i - 1) * pixelWidth, (ycenter + a) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - i - 1) * pixelWidth, (ycenter - a - 1) * pixelHeight, pixelWidth, pixelHeight);
                }
            }
            if (a < olBoundry.size()) {
                dc.SetBrush(wxBrush(wcolor2));
                for (i; i < olBoundry[a]; i++) {
                    dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + a) * pixelWidth, (ycenter - i - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - a - 1) * pixelWidth, (ycenter + i) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - a - 1) * pixelWidth, (ycenter - i - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + i) * pixelWidth, (ycenter + a) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter + i) * pixelWidth, (ycenter - a - 1) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - i - 1) * pixelWidth, (ycenter + a) * pixelHeight, pixelWidth, pixelHeight);
                    dc.DrawRectangle((xcenter - i - 1) * pixelWidth, (ycenter - a - 1) * pixelHeight, pixelWidth, pixelHeight);
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

    void RenderTexture(Component* c, wxDC& dc) {

    }

    void RenderRectangle(Component* c, wxDC& dc) {
        xhRectangle* rect = dynamic_cast<xhRectangle*>(c);

        int chwidth = crosshair->GetWidth();

        int chheight = crosshair->GetHeight();

        int xcenter = chwidth / 2;
        int ycenter = chheight / 2;


        //if 1 it will draw true size
        /*int pixelWidth = GetSize().GetWidth() / chwidth;
        int pixelHeight = GetSize().GetHeight() / chheight;*/
        int pixelWidth = 1;
        int pixelHeight = 1;


        int width = rect->GetWidth();
        int length = rect->GetSize();
        //int gap = rect->GetGap();
        int xoff = rect->GetXOffset();
        int yoff = rect->GetYOffset();

        Pixel color = rect->GetColor();
        Pixel outline_color = rect->GetOutlineColor();

        wxColour wcolor(color.red, color.green, color.blue, color.alpha);
        wxColour wcolor2(outline_color.red, outline_color.green, outline_color.blue, outline_color.alpha);

        int outline = rect->GetOutlineThickness() * rect->GetOutline();

        //OutputDebugString(L"Rendering a plus\n");

        dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));

        //Each Loop draws one arm, if i or j are outside a certain boundry it draws the outline color instead of the shape color
        for (int i = 0 - outline - (width/2); i < (width/2) + outline; i++) {
            for (int j = 0 - outline - (length/2); j < (length/2) + outline; j++) {
                //pixel to be drawn
                int pixx = xcenter + xoff + i;
                int pixy = ycenter + yoff + j;
                //OutputDebugString(L"Trying to Draw a Pixel\n");
                if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                    //OutputDebugString(L"Drawing a pixel\n");
                    if (i < 0 - (width / 2) || i >= width/2 || j < -(length / 2) || j >= length/2) {
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

    void RenderDiamond(Component* c, wxDC& dc) {
        xhDiamond* rect = dynamic_cast<xhDiamond*>(c);

        int chwidth = crosshair->GetWidth();

        int chheight = crosshair->GetHeight();

        int xcenter = chwidth / 2;
        int ycenter = chheight / 2;


        //if 1 it will draw true size
        /*int pixelWidth = GetSize().GetWidth() / chwidth;
        int pixelHeight = GetSize().GetHeight() / chheight;*/
        int pixelWidth = 1;
        int pixelHeight = 1;


        int width = rect->GetWidth();
        int length = rect->GetSize();

        //find gcd
        int g = std::gcd(width, length);

        int swidth = width / g;
        int slength = length / g;

        //int gap = rect->GetGap();
        int xoff = rect->GetXOffset();
        int yoff = rect->GetYOffset();

        Pixel color = rect->GetColor();
        Pixel outline_color = rect->GetOutlineColor();

        wxColour wcolor(color.red, color.green, color.blue, color.alpha);
        wxColour wcolor2(outline_color.red, outline_color.green, outline_color.blue, outline_color.alpha);

        int outline = rect->GetOutlineThickness() * rect->GetOutline();

        //OutputDebugString(L"Rendering a plus\n");

        dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));

        //Each Loop draws one arm, if i or j are outside a certain boundry it draws the outline color instead of the shape color
        for (int i = 0 - outline - (width / 2); i < (width / 2) + outline; i++) {
            int a = ( ((width / 2) - abs(i)) * slength) / swidth;
            for (int j = 0 - outline - a; j < outline + a; j++) {
                //pixel to be drawn
                int pixx = xcenter + xoff + i;
                int pixy = ycenter + yoff + j;

                //Bounds of frame
                if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                    //OutputDebugString(L"Drawing a pixel\n");
                    if (j < - a || j >= a || i < -width / 2|| i>=width/2) {
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

    void RenderPlus(Component* c, wxDC& dc) {
        Plus* plus = dynamic_cast<Plus*>(c);

        int chwidth = crosshair->GetWidth();

        int chheight = crosshair->GetHeight();

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

        wxColour wcolor(color.red, color.green, color.blue, color.alpha);
        wxColour wcolor2(outline_color.red, outline_color.green, outline_color.blue, outline_color.alpha);

        int outline = plus->GetOutlineThickness()*plus->GetOutline();

        //OutputDebugString(L"Rendering a plus\n");

        dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));

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
        else {
            for (int pass = 1; pass >= 0; pass--) {
                if (pass) {
                    dc.SetBrush(wxBrush(wcolor2));
                }
                else {
                    dc.SetBrush(wxBrush(wcolor));
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
                            dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                        }

                    }
                }

                for (int i = 0 - (outline * pass); i < width + (outline * pass); i++) {
                    for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
                        //pixel to be drawn
                        int pixx = xcenter - (width / 2) + i;
                        int pixy = ycenter - gap + j - length;

                        if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                            dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                        }

                    }
                }

                for (int i = 0 - (outline * pass); i < width + (outline * pass); i++) {
                    for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
                        //pixel to be drawn
                        int pixx = xcenter - length + j - gap;
                        int pixy = ycenter - (width / 2) + i;

                        if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                            dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                        }
                    }
                }

                for (int i = 0 - (outline*pass); i < width + (outline * pass); i++) {
                    for (int j = 0 - (outline * pass); j < length + (outline * pass); j++) {
                        //pixel to be drawn
                        int pixx = xcenter + j + gap;
                        int pixy = ycenter - (width / 2) + i;

                        if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                            dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                        }
                    }
                }

            }
        }   

    }

    void RenderX(Component* c, wxDC& dc) {
        xhX* plus = dynamic_cast<xhX*>(c);

        int chwidth = crosshair->GetWidth();

        int chheight = crosshair->GetHeight();

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

        wxColour wcolor(color.red, color.green, color.blue, color.alpha);
        wxColour wcolor2(outline_color.red, outline_color.green, outline_color.blue, outline_color.alpha);

        int outline = plus->GetOutlineThickness() * plus->GetOutline();

        //OutputDebugString(L"Rendering a plus\n");

        dc.SetPen(wxPen(wcolor2, 0, wxPENSTYLE_TRANSPARENT));

        if (plus->GetOutlineType()) {

            //starts at the inner corner and goes diagonally to top outer corner
            //each loop then determines how many pixels to draw straight below it
            for (int i = 0 - outline; i < length + outline; i++) {
                int lim = std::min(i, width);//how many main color pixels to draw down before outline, if<0 draw outline-lim pixels of outline color
                for (int j = 0; j <= lim + outline; j++) {
                    //pixel to be drawn
                    int pixx = xcenter - i;
                    int pixy = ycenter - i + j;
                    if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                        if (i < 0 || i >= length || j < 0 || j >= lim) {
                            dc.SetBrush(wxBrush(wcolor2));
                            //crosshair.SetColor(pixx, pixy, outline_color);
                        }
                        else {
                            dc.SetBrush(wxBrush(wcolor));
                            //crosshair.SetColor(pixx, pixy, color);
                        }
                        //dc.DrawRectangle(pixx * pixelWidth, pixy * pixelHeight, pixelWidth, pixelHeight);
                        dc.DrawRectangle((xcenter - i - gap) * pixelWidth, (ycenter - i + j - gap) * pixelHeight, pixelWidth, pixelHeight);
                        dc.DrawRectangle((xcenter - i + j - gap) * pixelWidth, (ycenter - i - gap) * pixelHeight, pixelWidth, pixelHeight);

                        dc.DrawRectangle((xcenter + i + gap - 1) * pixelWidth, (ycenter - i + j - gap) * pixelHeight, pixelWidth, pixelHeight);
                        dc.DrawRectangle((xcenter + i - j + gap - 1) * pixelWidth, (ycenter - i - gap) * pixelHeight, pixelWidth, pixelHeight);

                        dc.DrawRectangle((xcenter - i - gap) * pixelWidth, (ycenter + i - j + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                        dc.DrawRectangle((xcenter - i + j - gap) * pixelWidth, (ycenter + i + gap - 1) * pixelHeight, pixelWidth, pixelHeight);

                        dc.DrawRectangle((xcenter + i + gap - 1) * pixelWidth, (ycenter + i - j + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                        dc.DrawRectangle((xcenter + i - j + gap - 1) * pixelWidth, (ycenter + i + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                    }

                }
            }

        }
        else {
            for (int i = 0 - outline; i < length + outline; i++) {
                int lim = std::min(i, width);//how many main color pixels to draw down before outline, if<0 draw outline-lim pixels of outline color
                for (int j = 0; j <= lim + outline; j++) {
                    //pixel to be drawn
                    int pixx = xcenter - i;
                    int pixy = ycenter - i + j;
                    if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                        if (i < 0 || i >= length || j < 0 || j >= lim) {
                            dc.SetBrush(wxBrush(wcolor2));
                            //crosshair.SetColor(pixx, pixy, outline_color);
                            dc.DrawRectangle((xcenter - i - gap) * pixelWidth, (ycenter - i + j - gap) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter - i + j - gap) * pixelWidth, (ycenter - i - gap) * pixelHeight, pixelWidth, pixelHeight);

                            dc.DrawRectangle((xcenter + i + gap - 1) * pixelWidth, (ycenter - i + j - gap) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter + i - j + gap - 1) * pixelWidth, (ycenter - i - gap) * pixelHeight, pixelWidth, pixelHeight);

                            dc.DrawRectangle((xcenter - i - gap) * pixelWidth, (ycenter + i - j + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter - i + j - gap) * pixelWidth, (ycenter + i + gap - 1) * pixelHeight, pixelWidth, pixelHeight);

                            dc.DrawRectangle((xcenter + i + gap - 1) * pixelWidth, (ycenter + i - j + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter + i - j + gap - 1) * pixelWidth, (ycenter + i + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                        }
                        
                    }

                }
            }
            for (int i = 0 - outline; i < length + outline; i++) {
                int lim = std::min(i, width);//how many main color pixels to draw down before outline, if<0 draw outline-lim pixels of outline color
                for (int j = 0; j <= lim + outline; j++) {
                    //pixel to be drawn
                    int pixx = xcenter - i;
                    int pixy = ycenter - i + j;
                    if (pixx >= 0 && pixx < chwidth && pixy >= 0 && pixy < chheight) {
                        if (i < 0 || i >= length || j < 0 || j >= lim) {
                            
                        }
                        else {
                            dc.SetBrush(wxBrush(wcolor));
                            //crosshair.SetColor(pixx, pixy, outline_color);
                            dc.DrawRectangle((xcenter - i - gap) * pixelWidth, (ycenter - i + j - gap) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter - i + j - gap) * pixelWidth, (ycenter - i - gap) * pixelHeight, pixelWidth, pixelHeight);

                            dc.DrawRectangle((xcenter + i + gap - 1) * pixelWidth, (ycenter - i + j - gap) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter + i - j + gap - 1) * pixelWidth, (ycenter - i - gap) * pixelHeight, pixelWidth, pixelHeight);

                            dc.DrawRectangle((xcenter - i - gap) * pixelWidth, (ycenter + i - j + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter - i + j - gap) * pixelWidth, (ycenter + i + gap - 1) * pixelHeight, pixelWidth, pixelHeight);

                            dc.DrawRectangle((xcenter + i + gap - 1) * pixelWidth, (ycenter + i - j + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                            dc.DrawRectangle((xcenter + i - j + gap - 1) * pixelWidth, (ycenter + i + gap - 1) * pixelHeight, pixelWidth, pixelHeight);
                        }
                    }
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

//Scrolling list for layer selection, order, and visibility
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
            //Background darker if selected
            wxPanel* background = new wxPanel(this, wxID_ANY);
            if (crosshair->selectedLayer == id) {
                background->SetBackgroundColour(wxColor(50, 50, 50));
            }
            else {
                background->SetBackgroundColour(wxColor(90, 90, 90));
            }

            wxBoxSizer* s = new wxBoxSizer(wxHORIZONTAL);
            wxString n = c->GetName();
            wxButton* b = new wxButton(background, BUTTON_LAYER, wxString::Format(n));

            background->SetSizer(s);

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

            wxButton* vb = new wxButton(background, BUTTON_VISIBLE, "Visible");

            auto lambdaEventHandler2 = [this, c](wxCommandEvent& event) {
                OutputDebugString(L"Button Clicked2\n");
                c->ToggleVisibility();
                event.Skip();
                };

            vb->Bind(wxEVT_BUTTON, lambdaEventHandler2);

            id++;

            s->Add(b, 0, wxALL, 3);
            s->Add(vb, 0, wxALL, 3);
            sizer->Add(background, 0, wxALL, 3);
        }

        this->FitInside(); // ask the sizer about the needed size
        this->SetScrollRate(5, 5);
    }
};

//Linked slider and text box for uint8_t
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

        sizer->Add(clabel, 1, 0, 1);
        sizer->Add(cslider, 2, 0, 1);
        sizer->Add(cvalue, 1, 0, 1);
    }
};

//Linked slider and text box for ints, more customizable
class IntSlider : public wxPanel {
private:
    wxBoxSizer* sizer;
    int* val;
    int max;

public:
    IntSlider(wxWindow* parent, int* val, wxString label, int x, int y) : wxPanel(parent), val(val) {
        //max = std::max(x, y);

        sizer = new wxBoxSizer(wxHORIZONTAL);
        this->SetSizer(sizer);

        wxStaticText* clabel = new wxStaticText(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, 0);
        wxSlider* cslider = new wxSlider(this, SLIDER_UPDATE, *val, x, y, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL, wxDefaultValidator);
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

        sizer->Add(clabel, 1, 0, 1);
        sizer->Add(cslider, 2, 0, 1);
        sizer->Add(cvalue, 1, 0, 1);
    }
};

//Color picker
class ColorControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Pixel* color;
    wxWindow* parent;

public:
    ColorControl(wxWindow* parent, Pixel* color, wxString label) : wxPanel(parent), color(color), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        //wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, label, wxDefaultPosition, wxDefaultSize);
        //wxWindow* win = pane->GetPane();

        wxWindow* win = this;
        

        //wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Color"), 0, 0, 5);

        ColorSlider* redSlider = new ColorSlider(win, &(color->red), "R");
        sizer->Add(redSlider, 0, 0, 1);

        ColorSlider* greenSlider = new ColorSlider(win, &(color->green), "G");
        sizer->Add(greenSlider, 0, 0, 1);

        ColorSlider* blueSlider = new ColorSlider(win, &(color->blue), "B");
        sizer->Add(blueSlider, 0, 0, 1);

        ColorSlider* alphaSlider = new ColorSlider(win, &(color->alpha), "A");
        sizer->Add(alphaSlider, 0, 0, 1);
        
        //win->SetSizer(sizer2);
        //sizer->Add(pane, 0);

        //pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &ColorControl::OnPaneChanged, this);
        //pane->Collapse(false);
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

//Container for Plus shape specific controls
class PlusControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Plus* p;
    wxWindow* parent;

public:
    PlusControl(wxWindow* parent, Plus* plus, int x, int y) : wxPanel(parent), p(plus), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        //wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, "Dimensions", wxDefaultPosition, wxDefaultSize);
        //wxWindow* win = pane->GetPane();
        wxWindow* win = this;
        int max = std::max(x, y);

        //wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Dimensions"), 0, 0, 1);
        wxBoxSizer* lengths = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new IntSlider(win, &(p->GetSize()), "Length", 0, max), 0, 0, 1);

        wxBoxSizer* widths = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new IntSlider(win, &(p->GetWidth()), "Width", 0, max), 0, 0, 1);

        wxBoxSizer* gaps = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new IntSlider(win, &(p->GetGap()), "Gap", 0, max), 0, 0, 1);

        /*win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &PlusControl::OnPaneChanged, this);
        pane->Collapse(false);*/
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

//Container for Plus shape specific controls
class RectangleControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    xhRectangle* p;
    wxWindow* parent;

public:
    RectangleControl(wxWindow* parent, xhRectangle* rect, int x, int y) : wxPanel(parent), p(rect), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        //wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, "Dimensions", wxDefaultPosition, wxDefaultSize);
        //wxWindow* win = pane->GetPane();
        wxWindow* win = this;

        int max = std::max(x, y);
        //wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Dimensions"), 0, 0, 1);
        wxBoxSizer* lengths = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new IntSlider(win, &(p->GetSize()), "Length", 0, max), 0, 0, 1);

        wxBoxSizer* widths = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new IntSlider(win, &(p->GetWidth()), "Width", 0, max), 0, 0, 1);

        wxBoxSizer* xoffs = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new IntSlider(win, &(p->GetXOffset()), "X offset", -(x/2), x/2), 0, 0, 1);

        wxBoxSizer* yoffs = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new IntSlider(win, &(p->GetYOffset()), "Y offset", -(y/2), y/2), 0, 0, 1);

        /*win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &PlusControl::OnPaneChanged, this);
        pane->Collapse(false);*/
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

//Container for Circle shape specific controls
class CircleControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Circle* c;
    wxWindow* parent;

public:
    CircleControl(wxWindow* parent, Circle* circle, int x, int y) : wxPanel(parent), c(circle), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        //wxCollapsiblePane* pane = new wxCollapsiblePane(this, wxID_ANY, "Dimensions", wxDefaultPosition, wxDefaultSize);
        //wxWindow* win = pane->GetPane();
        wxWindow* win = this;

        int max = std::max(x, y);

        //wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);

        sizer->Add(new wxStaticText(this, wxID_ANY, "Dimensions"), 0, 0, 1);
        wxBoxSizer* radiuss = new wxBoxSizer(wxHORIZONTAL);

        sizer->Add(new IntSlider(win, &(c->GetSize()), "Radius", 0, max), 0, 0, 1);

        //wxBoxSizer* widths = new wxBoxSizer(wxHORIZONTAL);
        //sizer->Add(new IntSlider(win, &(c->GetWidth()), "Width", x, y), 1, 0, 1);

        wxBoxSizer* gaps = new wxBoxSizer(wxHORIZONTAL);
        sizer->Add(new IntSlider(win, &(c->GetGap()), "Gap", 0, max), 0, 0, 1);

        /*win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &PlusControl::OnPaneChanged, this);
        pane->Collapse(false);*/
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

//Container for Outline controls
class OutLineControl : public wxPanel {
private:
    wxBoxSizer* sizer;
    Shape* s;
    wxWindow* parent;

public:
    OutLineControl(wxWindow* parent, Shape* shape, int x, int y) : wxPanel(parent), s(shape), parent(parent) {
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        /*wxCollapsiblePane* pane = new wxCollapsiblePane(this, CHECKBOX_HASOUTLINE, "OutLine", wxDefaultPosition, wxDefaultSize);
        wxWindow* win = pane->GetPane();
        wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);*/

        int max = std::max(x, y);

        wxWindow* win = this;
        sizer->Add(new wxStaticText(this, wxID_ANY, "Outline"), 1, wxEXPAND | wxALL, 5);

        wxCheckBox* check = new wxCheckBox(win, wxID_ANY, "Show outline");
        check->SetValue(&(shape->GetOutline()));

        check->Bind(wxEVT_CHECKBOX, [this, shape, check](wxCommandEvent& event) {
            // Toggle the value of showoutline
            shape->ToggleOutline();

            // Update the state of the checkbox
            check->SetValue(shape->GetOutline());
            wxCommandEvent evt(wxEVT_CHECKBOX, CHECKBOX_HASOUTLINE);
            ProcessEvent(evt);
            });

        sizer->Add(check, 1, wxEXPAND | wxALL, 5);

        if (shape->GetType()==PLUSLAYER|| shape->GetType() == XLAYER) {
            Plus* plus = dynamic_cast<Plus*>(shape);

            wxCheckBox* check2 = new wxCheckBox(win, wxID_ANY, "Lazy Outline");
            check2->SetValue(&(plus->GetOutlineType()));

            check2->Bind(wxEVT_CHECKBOX, [this, plus, check2](wxCommandEvent& event) {
                // Toggle the value of showoutline
                plus->ChangeOutlineType();

                // Update the state of the checkbox
                check2->SetValue(plus->GetOutline());
                wxCommandEvent evt(wxEVT_CHECKBOX, CHECKBOX_LAZYOUTLINE);
                ProcessEvent(evt);
                });

            sizer->Add(check2, 1, wxEXPAND | wxALL, 5);
        }
        else if (shape->GetType() == CIRCLELAYER) {
            Circle* circle = dynamic_cast<Circle*>(shape);

            wxCheckBox* check2 = new wxCheckBox(win, wxID_ANY, "Inner Outline");
            check2->SetValue(&(circle->GetInnerOutline()));

            check2->Bind(wxEVT_CHECKBOX, [this, circle, check2](wxCommandEvent& event) {
                // Toggle the value of showoutline
                circle->ToggleInnerOutline();

                // Update the state of the checkbox
                check2->SetValue(circle->GetInnerOutline());
                wxCommandEvent evt(wxEVT_CHECKBOX, CHECKBOX_HASINNEROUTLINE);
                ProcessEvent(evt);
                });

            sizer->Add(check2, 1, wxEXPAND | wxALL, 5);
            sizer->Add(new wxStaticText(win, wxID_ANY, "Inner Outline Thickness"));
            sizer->Add(new IntSlider(win, &(circle->GetInnerOutlineThickness()), "", 0, max), 1, 0, 1);
        }

        sizer->Add(new wxStaticText(win, wxID_ANY, "Outline Thickness"));
        sizer->Add(new IntSlider(win, &(s->GetOutlineThickness()), "", 0, max), 1, 0, 1);

        Pixel* color = &(s->GetOutlineColor());
        sizer->Add(new wxStaticText(win, wxID_ANY, "Outline Color"));
        sizer->Add(new ColorSlider(win, &(color->red), "R"), 1, 0, 1);
        sizer->Add(new ColorSlider(win, &(color->green), "G"), 1, 0, 1);
        sizer->Add(new ColorSlider(win, &(color->blue), "B"), 1, 0, 1);
        sizer->Add(new ColorSlider(win, &(color->alpha), "A"), 1, 0, 1);

        /*win->SetSizer(sizer2);
        sizer->Add(pane, 0);

        pane->Bind(wxEVT_COLLAPSIBLEPANE_CHANGED, &OutLineControl::OnPaneChanged, this);
        pane->Collapse(false);*/
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

//Parent container for all relevant layer control containers
class ControlPanel : public wxPanel {
private:
    int type;
    wxBoxSizer* sizer;
    wxBoxSizer* sizer2;
    wxScrolledWindow* scrolledWindow;
    int x;
    int y;
    //Component* componentPtr;

public:
    ControlPanel(wxWindow* parent, int id, Crosshair* c, int layer) : wxPanel(parent){
        OutputDebugString(L"Constructing ControlPanel\n");
        sizer = new wxBoxSizer(wxVERTICAL);
        SetSizer(sizer);
        x = c->GetWidth();
        y = c->GetHeight();

        //componentPtr = c;

        OutputDebugString(L"Getting component\n");
        Component* component = c->layers[layer];
        type = component->GetType();

        //wxBoxSizer* topsizer = new wxBoxSizer(wxHORIZONTAL);
        wxTextCtrl* name = new wxTextCtrl(this, NAME_UPDATE, component->GetName(), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
        name->Bind(wxEVT_TEXT_ENTER, [this, name, component](wxCommandEvent& event) {
            component->SetName(name->GetValue().ToStdString());

            wxCommandEvent evt(wxEVT_COMMAND_TEXT_UPDATED, NAME_UPDATE);
            ProcessEvent(evt);
            });


        //topsizer->Add(name,1, 0, 1);

        sizer->Add(name, 0, 0, 1);

        OutputDebugString(L"Entering switch\n");
        switch (type) {
        case PLUSLAYER: {
            CreateCrossControl(component);
            break;
        }
        case XLAYER: {
            CreateCrossControl(component);
            break;
        }
        case CIRCLELAYER: {
            CreateCircleControl(component);
            break;
        }
        case RECTLAYER: {
            CreateRectangleControl(component);
            break;
        }
        case DIAMONDLAYER: {
            CreateRectangleControl(component);
            break;
        }


        }
        OutputDebugString(L"Constructed ControlPanel\n");
    }

    void CreateCrossControl(Component* c) {
        //sizer->Clear(true);

        OutputDebugString(L"Updating Controls\n");

        //Cast component to cross to access Plus members
        Plus* plus = dynamic_cast<Plus*>(c);

        //Main Color
        OutputDebugString(L"Creating Color Control\n");
        ColorControl* mainColorControl = new ColorControl(this, &(plus->GetColor()), "Color");
        sizer->Add(mainColorControl,  1, 0, 1);

        //Dimensions
        OutputDebugString(L"Creating Cross Control\n");
        PlusControl* dimensionControl = new PlusControl(this, plus, x, y);
        sizer->Add(dimensionControl, 1, 0, 1);

        //outline checkbox
        OutputDebugString(L"Creating Outline Control\n");
        OutLineControl* outlineControl = new OutLineControl(this, plus, x, y);
        sizer->Add(outlineControl, 1, 0, 1);

        //SetSizer(sizer);
        OutputDebugString(L"Controls Updated\n");

        //return sizer;
    }

    void CreateRectangleControl(Component* c) {
        //sizer->Clear(true);

        OutputDebugString(L"Updating Controls\n");

        //Cast component to cross to access Plus members
        xhRectangle* plus = dynamic_cast<xhRectangle*>(c);

        //Main Color
        OutputDebugString(L"Creating Color Control\n");
        ColorControl* mainColorControl = new ColorControl(this, &(plus->GetColor()), "Color");
        sizer->Add(mainColorControl, 1, 0, 1);

        //Dimensions
        OutputDebugString(L"Creating Plus Control\n");
        RectangleControl* dimensionControl = new RectangleControl(this, plus, x, y);
        sizer->Add(dimensionControl, 1, 0, 1);

        //outline checkbox
        OutputDebugString(L"Creating Outline Control\n");
        OutLineControl* outlineControl = new OutLineControl(this, plus, x, y);
        sizer->Add(outlineControl, 1, 0, 1);

        //SetSizer(sizer);
        OutputDebugString(L"Controls Updated\n");

        //return sizer;
    }

    void CreateCircleControl(Component* c) {
        Circle* circle = dynamic_cast<Circle*>(c);

        //Main Color
        OutputDebugString(L"Creating Color Control\n");
        ColorControl* mainColorControl = new ColorControl(this, &(circle->GetColor()), "Color");
        sizer->Add(mainColorControl, 1, 0, 1);

        //Dimensions
        OutputDebugString(L"Creating Circle Control\n");
        CircleControl* dimensionControl = new CircleControl(this, circle, x, y);
        sizer->Add(dimensionControl, 1, 0, 1);

        //outline checkbox
        OutputDebugString(L"Creating Outline Control\n");
        OutLineControl* outlineControl = new OutLineControl(this, circle, x, y);
        sizer->Add(outlineControl, 1, 0, 1);

        //SetSizer(sizer);
        OutputDebugString(L"Controls Updated\n");
    }
};

/****************************************/
//These classes make up the 3 main panels
/****************************************/
class LayerListPane : public wxPanel {
private:
    wxSizer* sizer;
    wxBoxSizer* sizer2;
    wxBoxSizer* llsizer;
    wxBoxSizer* buttonsizer;
    Crosshair* crosshair;
    wxPanel* panel;

public:
    LayerListPane(wxWindow* parent, Crosshair* c) :wxPanel(parent), crosshair(c){
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        sizer2 = new wxBoxSizer(wxVERTICAL);

        //Layer list
        ScrolledWidgetsPane* layerlist = new ScrolledWidgetsPane(this, wxID_ANY, crosshair);
        llsizer = new wxBoxSizer(wxVERTICAL);
        llsizer->Add(layerlist, 1, wxEXPAND | wxALL, 5);

        //Layer Buttons
        buttonsizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* deleteLayer = new wxButton(this, BUTTON_DELETELAYER, "Delete");
        wxButton* newLayer = new wxButton(this, BUTTON_NEWLAYER, "New Layer");

        wxString choices[] = { wxT("Plus"), wxT("Circle"), wxT("Rectangle"), wxT("Diamond"), wxT("X") };
        wxArrayString arrChoices(5, choices);
        wxComboBox* typeselect = new wxComboBox(this, LAYER_TYPE_DROPDOWN, wxT("Type"), wxDefaultPosition, wxDefaultSize, arrChoices);

        typeselect->Bind(wxEVT_COMBOBOX, [this, typeselect, c](wxCommandEvent& event) {
            switch (typeselect->GetSelection()) {
            case 0: {
                c->typeToAdd = PLUSLAYER;
                break;
            }
            case 1: {
                c->typeToAdd = CIRCLELAYER;
                break;
            }
            case 2: {
                c->typeToAdd = RECTLAYER;
                break;
            }
            case 3: {
                c->typeToAdd = DIAMONDLAYER;
                break;
            }
            case 4: {
                c->typeToAdd = XLAYER;
                break;
            }
            }

            //wxCommandEvent evt(wxEVT_COMBOBOX, LAYER_TYPE_DROPDOWN);
            //ProcessEvent(evt);
            //event.Skip();
            });
        typeselect->SetSelection(0);
        buttonsizer->Add(deleteLayer, 1, 0, 5);
        buttonsizer->Add(typeselect, 1, 0, 5);
        buttonsizer->Add(newLayer, 1, 0, 5);

        sizer2->Add(llsizer, 1, wxEXPAND | wxALL, 5);
        sizer2->Add(buttonsizer, 1, wxEXPAND | wxALL, 5);

        //panel->SetSizer(sizer2);
        sizer->Add(sizer2, 1, wxEXPAND | wxALL, 5);
    }

    void Update() {
        OutputDebugString(L"Updating Layer Pane\n");
        llsizer->Clear(true);
        ScrolledWidgetsPane* layerlist = new ScrolledWidgetsPane(this, wxID_ANY, crosshair);
        //llsizer = new wxBoxSizer(wxVERTICAL);
        llsizer->Add(layerlist, 1, wxEXPAND | wxALL, 5);
        OutputDebugString(L"Updated Layer Pane\n");
    }

};

class ControlPanelPane : public wxPanel {
private:
    wxSizer* sizer;
    wxBoxSizer* sizer2;
    wxBoxSizer* llsizer;
    wxBoxSizer* buttonsizer;
    Crosshair* crosshair;
    wxPanel* panel;
public:
    ControlPanelPane(wxWindow* parent, int id, Crosshair* c, int layer) : wxPanel(parent), crosshair(c){
        OutputDebugString(L"Constructing ControlPanelPane\n");

        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        if (crosshair->selectedLayer > -1) {
            OutputDebugString(L"Selected Layer > -1\n");
            ControlPanel* control = new ControlPanel(this, wxID_ANY, crosshair, crosshair->selectedLayer);
            sizer->Add(control, 0,0,1);
        }

        OutputDebugString(L"Constructed ControlPanelPane\n");
    }
    void Update() {
        OutputDebugString(L"Updating Control Pane\n");
        sizer->Clear(true);

        if (crosshair->selectedLayer > -1) {
            ControlPanel* control = new ControlPanel(this, wxID_ANY, crosshair, crosshair->selectedLayer);
            sizer->Add(control, 0, 0, 1);
        }
        OutputDebugString(L"Updated Control Pane\n");
    }
};

class PreviewPanelPane : public wxPanel {
private:
    wxSizer* sizer;
    wxBoxSizer* sizer2;
    wxBoxSizer* llsizer;
    wxBoxSizer* buttonsizer;
    Crosshair* crosshair;
    wxPanel* panel;

public:
    PreviewPanelPane(wxWindow* parent, Crosshair* c) :wxPanel(parent), crosshair(c){
        OutputDebugString(L"Constructing PreviewPanelPane\n");
        sizer = new wxBoxSizer(wxVERTICAL);
        this->SetSizer(sizer);

        // Preview window
        panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(c->GetWidth(), c->GetHeight()));
        panel->SetBackgroundColour(wxColor(100, 90, 90));
        sizer2 = new wxBoxSizer(wxHORIZONTAL);
        panel->SetSizer(sizer2);

        ImagePanel* previewimg = new ImagePanel(panel, c);
        sizer2->Add(previewimg, 1, wxEXPAND | wxALL, 5);
        sizer->Add(panel, 1, 0, 5);

        // Save and Test buttons
        wxBoxSizer* previewbuttonsizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* saveButton = new wxButton(this, BUTTON_SAVE, "Save");
        wxButton* exportButton = new wxButton(this, BUTTON_EXPORT, "Export");
        wxButton* testButton = new wxButton(this, BUTTON_TEST, "Test");
        previewbuttonsizer->Add(saveButton, 1, 0, 5);
        previewbuttonsizer->Add(exportButton, 1, 0, 5);
        previewbuttonsizer->Add(testButton, 1, 0, 5);

        sizer->Add(previewbuttonsizer, 1, 0, 5);
    }

    void Update() {
        sizer2->Clear(true);
        ImagePanel* previewimg = new ImagePanel(panel, crosshair);
        sizer2->Add(previewimg, 1, wxSHAPED | wxCENTER, 5);
    }
};

