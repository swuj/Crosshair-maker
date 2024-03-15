#pragma once
#include <vector>

struct Pixel {
	uint8_t red, green, blue, alpha;
};

class Shape {
protected:
	int type = 0;
	Pixel shape_color;	//color of shape
	int size;			//size of shape
	bool outline;
	int outline_thickness;
	Pixel outline_color;

public:

	Shape(Pixel c, int s, bool o, int ot, Pixel oc) : shape_color(c), size(s), outline(o), outline_thickness(ot), outline_color(oc) {}

	int GetSize() {
		return size;
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

	

	void ChangeOutlineColor(Pixel c) {
		outline_color = c;
	}

};

class Circle : public Shape {
private:
	int type = 1;
	bool inner_outline;

public:
	Circle() : Shape({255,255,255,255}, 2, true, 1, { 0,0,0,255 }), inner_outline(true) {}

	Circle(Pixel c, int r, bool o, Pixel oc, int ot, bool io) : Shape(c, r, o, ot, oc), inner_outline(io) {}

	void ToggleInnerOutline() {
		if (inner_outline) {
			inner_outline = false;
		}
		else {
			inner_outline = true;
		}
	}
};

class Plus : public Shape {
private:
	int outline_type; //0 for none, 1 for lazy, 2 for proper
	int width;

public:
	Plus() : Shape({ 255,255,255,255 }, 4, true, 1, { 0,0,0,255 }), width(2), outline_type(1) {}

	Plus(Pixel c, int l, int w, bool o, Pixel oc, int ot, int otype) : Shape(c, l, o, ot, oc), outline_type(otype), width(w) {}

	void ChangeOutlineType(int t) {
		outline_type = t;
	}
};
