#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>
#include "component.h"

//Structure representing a crosshair, has pixel dimensions and a vector containing all the shapes that make up the crosshair
//as well as a Pixel vector representing the final image, used for png export
class Crosshair {
private:
	//pixel dimensions
	int width;
	int height;

	//Final image representation
	std::vector<std::vector<Pixel>> pixels;

public:
	std::string name = "newCrosshair";

	//Collection of components that make up the crosshair
	std::vector<Component*> layers;

	//currently selected layer in the interface
	int selectedLayer = -1;
	
	//used by the AddLayer button in the layerlist interface, having it in the Crosshair class avoids some event binding stuff
	int typeToAdd = PLUSLAYER;

	//constructors
	Crosshair() : width(20), height(20), pixels(20, std::vector<Pixel>(20)) {}

	Crosshair(int w, int h) : width(w), height(h), pixels(w, std::vector<Pixel>(h)) {}

	//Inserts the given layer to the layers vector in the position after selectedLayer
	void AddLayer(Component* l) {

		OutputDebugString(L"Adding layer\n");
		auto insertPosition = layers.begin();
		if (selectedLayer >= 0 && selectedLayer < layers.size()) {
			insertPosition += selectedLayer + 1;
		}
		else {
			// If selectedLayer is out of range, simply insert at the end
			insertPosition = layers.end();
		}
		layers.insert(insertPosition, l);
		selectedLayer++;

		for (int i = selectedLayer; i < layers.size(); i++) {
			layers[i]->SetID(i);
		}
	}

	//Deletes the layer in the layers vector located at toDelete
	void DeleteLayer(int toDelete) {
		wchar_t debugString[200]; // Buffer for the debug string
		swprintf(debugString, 100, L"Attempting to delete layer: %d\n", toDelete);
		OutputDebugString(debugString);

		if (toDelete >= 0) {
			layers.erase(layers.begin() + toDelete);
			for (int i = toDelete; i < layers.size(); i++) {
				layers[i]->SetID(i);
			}
			if (toDelete <= selectedLayer) {
				selectedLayer--;
			}
		}
		OutputDebugString(L"Layer removed");
	}

	//set pixel color in the image representation vector
	void SetColor(int x, int y, Pixel c) {
		if (x >= 0 && x < width && y >= 0 && y < height) {
			pixels[x][y] = { c.red, c.green, c.blue, c.alpha };
		}
	}

	//sets every color to the given color
	void Initialize(Pixel bg) {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				pixels[x][y] = { bg.red, bg.green, bg.blue, bg.alpha};
			}
		}
	}

	void InitializeTest() {
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				pixels[x][y] = { 255,100,100,255 };
			}
		}
	}

	int GetWidth() const {
		return width;
	}
	int GetHeight() const {
		return height;
	}

	//save as png
	void SaveAsPng(const std::wstring& filePath) {
		std::vector<uint8_t> flatPixels;  // Flat array to hold the pixel data

		// Flatten the 2D vector into a 1D array
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				flatPixels.push_back(pixels[x][y].red);
				flatPixels.push_back(pixels[x][y].green);
				flatPixels.push_back(pixels[x][y].blue);
				flatPixels.push_back(pixels[x][y].alpha);
			}
		}
		std::string filePathStr(filePath.begin(), filePath.end());
		stbi_write_png(filePathStr.c_str(), width, height, 4, flatPixels.data(), width * 4);
	}

	const std::vector<std::vector<Pixel>>& GetPixels() const {
		return pixels;
	}

	std::string GetName() {
		return name;
	}

	void SetName(std::string n) {
		name = n;
	}
};