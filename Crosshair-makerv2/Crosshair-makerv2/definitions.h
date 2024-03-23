#pragma once
enum
{
    BUTTON_Hello = wxID_HIGHEST + 1, // declares an id which will be used to call our button
    BUTTON_NEW = wxID_HIGHEST + 2,
    BUTTON_LOAD = wxID_HIGHEST + 3,
    BUTTON_SAVE = wxID_HIGHEST + 4,
    BUTTON_TEST = wxID_HIGHEST + 5,
    BUTTON_NEW2 = wxID_HIGHEST + 6,
    BUTTON_NEWLAYER = wxID_HIGHEST + 7,
    BUTTON_DELETELAYER = wxID_HIGHEST + 8,
    BUTTON_LAYER = wxID_HIGHEST + 9,
    SLIDER_UPDATE = wxID_HIGHEST + 10,
    TEXT_UPDATE = wxID_HIGHEST + 11,
    CHECKBOX_HASOUTLINE = wxID_HIGHEST + 12,
    BUTTON_VISIBLE = wxID_HIGHEST + 13,
    NAME_UPDATE = wxID_HIGHEST + 14,
    TYPE_UPDATE = wxID_HIGHEST + 15,
    CHECKBOX_HASINNEROUTLINE = wxID_HIGHEST + 16,
    CHECKBOX_LAZYOUTLINE = wxID_HIGHEST + 17,
    BUTTON_EXPORT = wxID_HIGHEST +18, 
    LAYER_TYPE_DROPDOWN = wxID_HIGHEST + 19
};

wxDECLARE_EVENT(myEVT_PROGRESS, wxCommandEvent);
wxDEFINE_EVENT(myEVT_PROGRESS, wxCommandEvent);