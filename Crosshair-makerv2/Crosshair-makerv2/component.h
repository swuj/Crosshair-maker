#pragma once
#include <vector>

//types of structures that a layer can be
#define DEFAULTLAYER 100
#define TEXTURELAYER 101
#define SHAPELAYER 102
#define CIRCLELAYER 103
#define PLUSLAYER 104

struct Pixel {
	uint8_t red, green, blue, alpha;
};

//Generic parent class
class Component {
protected:
	bool hidden;
	std::string name;
	int type;
	int id;
	int gap = -1;
	int width = -1;

public:
	Component(int t) : name("newcomponent"), hidden(false), type(t) {};

	Component(std::string n) : name(n), hidden(false) {};

	int GetType() {
		return type;
	}

	std::string GetName() {
		return name;
	}

	//PLUS class virtual functions
	virtual void ChangeOutlineType() {
	}

	virtual int& GetGap() {
		return gap;
	}

	virtual int& GetWidth() {
		return width;
	}

	virtual void SetGap(int g) {
	}

	void SetID(int i) {
		id = i;
	}

	int GetID() {
		return id;
	}
	//duplicate

};

//Collection of pixel data rather than a Shape defined by dimensions and other variables
//Can act as a mask for above layers
class Texture : public Component {
private: 
	bool mask;
	std::vector<std::vector<Pixel>> pixels;

public:
	//Texture() :{};

	bool IsMask() {
		return mask;
	}

	void ToggleMask() {
		if (mask) {
			mask = false;
		}
		else {
			mask = true;
		}
	}

};

//Generic class for shape type components
class Shape : public Component{
protected:
	int type = 1;
	Pixel shape_color;	//color of shape
	int size;			//size of shape
	bool outline;
	int outline_thickness;
	Pixel outline_color;

public:

	Shape(std::string n, Pixel c, int s, bool o, int ot, Pixel oc, int t) : Component(t), shape_color(c), size(s), outline(o), outline_thickness(ot), outline_color(oc) {}

	int& GetSize() {
		return size;
	}

	bool& GetOutline() {
		return outline;
	}

	void SetSize(int s) {
		size = s;
	}

	void ChangeColor(Pixel c) {
		shape_color = c;
	}

	void ToggleOutline() {
		if (outline) {
			outline = false;
		}
		else {
			outline = true;
		}
	}

	int& GetOutlineThickness() {
		return outline_thickness;
	}

	Pixel& GetOutlineColor() {
		return outline_color;
	}

	Pixel& GetColor() {
		return shape_color;
	}

	Pixel& GetColorStruct() {
		return shape_color;
	}

	void ChangeOutlineColor(Pixel c) {
		outline_color = c;
	}

	//TODO
	//Convert to Texture component, probably needs to be overloaded, maybe this doesnt even go in here actually
};

//Circle shaped crosshair
class Circle : public Shape {
protected:
	int type = 3;
	bool inner_outline;

public:
	Circle() : Shape("newCircle", {255, 255, 255, 255}, 2, true, 1, {0,0,0,255}, 3), inner_outline(true) {}

	Circle(std::string n, Pixel c, int r, bool o, Pixel oc, int ot, bool io) : Shape(n, c, r, o, ot, oc, 3), inner_outline(io) {}

	void ToggleInnerOutline() {
		if (inner_outline) {
			inner_outline = false;
		}
		else {
			inner_outline = true;
		}
	}

	int& GetRadius() {
		return GetSize();
	}

	void SetRadius(int r) {
		SetSize(r);
	}
};

//traditional plus/cross shaped crosshair
class Plus : public Shape {
private:
	bool outline_type; //0 for none, 1 for lazy, 2 for proper
	int width;
	int gap;

public:
	Plus() : Shape("NewPlus", { 255, 255, 255, 255 }, 4, true, 1, {0,0,0,255}, PLUSLAYER), width(2), outline_type(1), gap(0) {}

	Plus(std::string n, Pixel c, int l, int w, bool o, Pixel oc, int ot, bool otype, int g) : Shape(n, c, l, o, ot, oc, PLUSLAYER), outline_type(otype), width(w), gap(g) {}

	void ChangeOutlineType() override {
		outline_type = !outline_type;
	}

	bool& GetOutlineType() {
		return outline_type;
	}

	int& GetGap() override {
		return gap;
	}

	int& GetWidth() override {
		return width;
	}

	void SetGap(int g) override {
		gap = g;
	}
};
