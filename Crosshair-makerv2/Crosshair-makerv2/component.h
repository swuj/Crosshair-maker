#pragma once
#include <vector>

//types of structures that a layer can be
#define DEFAULTLAYER 100
#define TEXTURELAYER 101
#define SHAPELAYER 102
#define CIRCLELAYER 103
#define PLUSLAYER 104
#define RECTLAYER 105
#define DIAMONDLAYER 106
#define XLAYER 107

struct Pixel {
	uint8_t red, green, blue, alpha;
};

//Abstract class that layer types derive from
class Component {
protected:
	bool visible;				//visibility of layer
	std::string name;			//name of layer
	int type;					//type of layer, for dynamic casting and accessing type specific members
	int id;						//id of layer, its position in Crosshair.layers/position in rendering order

public:
	Component(int t) : name("newcomponent"), visible(true), type(t) {};

	Component(std::string n, int type, bool visible) : name(n), visible(visible), type(type) {};

	int GetType() {
		return type;
	}
	void SetType(int t) {
		type = t;
	}

	std::string GetName() {
		return name;
	}

	void SetName(std::string n) {
		name = n;
	}

	//PLUS class virtual functions
	virtual void ChangeOutlineType() {
	}

	virtual int& GetGap() = 0;

	//virtual int& GetWidth() = 0;

	virtual void SetGap(int g) {
	}

	void SetID(int i) {
		id = i;
	}

	int GetID() {
		return id;
	}

	void ToggleVisibility() {
		visible = !visible;
	}

	bool& GetVisibility() {
		return visible;
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
		mask = !mask;
	}

};

//Generic class for shape type components
class Shape : public Component{
protected:
	int type = 1;
	Pixel shape_color;			// Color of shape
	int size;					// Size of shape, might rework this 
	bool outline;				// Visibility of outline
	int outline_thickness;		// Pixel width of outline
	Pixel outline_color;		// Color of outline
	int gap = 0;				// distance from center for plus, inner radius for circle

public:

	Shape(std::string name, Pixel color, int size, int gap, bool outline, int outline_thickness, Pixel outline_color, int type, bool visible) : 
		Component(name, type, visible), shape_color(color), size(size), outline(outline), outline_thickness(outline_thickness), outline_color(outline_color), gap(gap) {}

	int& GetSize() {
		return size;
	}

	int& GetGap() override {
		return gap;
	}

	void SetGap(int g) override {
		gap = g;
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
		outline = !outline;
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

//Circle shaped layer
class Circle : public Shape {
protected:
	bool inner_outline = true;			// does the opening have an outline
	int inner_outline_thickness = 1;
	Pixel inner_outline_color = {0, 0, 0, 255};


public:
	Circle() : Shape("newCircle", {255, 255, 255, 255}, 20, 3, true, 1, {0,0,0,255}, CIRCLELAYER, true), inner_outline(true) {}

	Circle(std::string name, Pixel color, int radius, int gap, bool outline, int outline_thickness, Pixel outline_color, bool inner_outline, int inner_outline_thickness, Pixel iocol, bool visible) :
		Shape(name, color, radius, gap, outline, outline_thickness, outline_color, CIRCLELAYER, visible), inner_outline(inner_outline), inner_outline_thickness(inner_outline_thickness), inner_outline_color(iocol) {}

	void ToggleInnerOutline() {
		inner_outline = !inner_outline;
	}

	int& GetRadius() {
		return GetSize();
	}

	bool& GetInnerOutline() {
		return inner_outline;
	}

	int& GetInnerOutlineThickness() {
		return inner_outline_thickness;
	}

	Pixel& GetInnerOutlineColor() {
		return inner_outline_color;
	}

	void SetRadius(int r) {
		SetSize(r);
	}

	int& GetInnerRadius() {
		return GetGap();
	}

	void SetInnerRadius(int r) {
		SetGap(r);
	}
};

class xhRectangle : public Shape {
private:
	int width;					// width of outline, odd values will be off center
	int x_offset = 0;					// arm offsets from center
	int y_offset = 0;

public:
	xhRectangle() : Shape("NewRect", { 255, 255, 255, 255 }, 2, 0, true, 1, { 0,0,0,255 }, RECTLAYER, true), width(2) {}

	xhRectangle(std::string name, Pixel color, int length, int width, int x_offset, int y_offset, bool outline, int outline_thickness, Pixel outline_color, bool visible) :
		Shape(name, color, length, 0, outline, outline_thickness, outline_color, RECTLAYER, visible), width(width), x_offset(x_offset), y_offset(y_offset) {}

	int& GetWidth() {
		return width;
	}

	int& GetXOffset() {
		return x_offset;
	}

	int& GetYOffset() {
		return y_offset;
	}
};

//Same as rectangle but has different render algorithm
class xhDiamond : public xhRectangle {
private:
public:
	xhDiamond() : xhRectangle() {
		SetType(DIAMONDLAYER);
	}

	xhDiamond(std::string name, Pixel color, int length, int width, int x_offset, int y_offset, bool outline, int outline_thickness, Pixel outline_color, bool visible) :
		xhRectangle(name, color, length, width, x_offset, y_offset, outline, outline_thickness, outline_color, visible)
	{
		SetType(DIAMONDLAYER);
	}
};

//traditional plus/cross shape
class Plus : public Shape {
private:
	bool outline_type;			// true is lazy(overwatch) style, false renders outlines for all 4 arms before the main color portion
	int width;					// width of outline, odd values will be off center
	int gap;					// arm distance from center

public:
	Plus() : Shape("NewPlus", { 255, 255, 255, 255 }, 4, 0, true, 1, {0,0,0,255}, PLUSLAYER, true), width(2), outline_type(false) {}

	Plus(std::string name, Pixel color, int length, int width, int gap, bool outline, int outline_thickness, Pixel outline_color, bool outline_type, bool visible) :
		Shape(name, color, length, gap, outline, outline_thickness, outline_color, PLUSLAYER, visible), outline_type(outline_type), width(width) {}

	void ChangeOutlineType() override {
		outline_type = !outline_type;
	}

	bool& GetOutlineType() {
		return outline_type;
	}

	int& GetWidth() {
		return width;
	}
};

class xhX : public Plus {
public:
	xhX() : Plus() {
		SetName("newX");
		SetType(XLAYER);
	}

	xhX(std::string name, Pixel color, int length, int width, int gap, bool outline, int outline_thickness, Pixel outline_color, bool outline_type, bool visible) :
		Plus(name, color, length, width, gap, outline, outline_thickness, outline_color, outline_type, visible) {
		SetType(XLAYER);
	}
};
