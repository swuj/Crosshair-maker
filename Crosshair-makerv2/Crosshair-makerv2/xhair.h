#include <vector>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <string>
#include "component.h"

//struct Pixel {
//	uint8_t red, green, blue, alpha;
//};

class Crosshair {
private:
	int width;
	int height;
	std::vector<std::vector<Pixel>> pixels;
	//std::vector<std::vector<Component*>> layers;

public:
	int typeToAdd = PLUSLAYER;
	std::vector<Component*> layers;
	int selectedLayer = -1;

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

	//std::vector<std::vector<
	Crosshair() : width(0), height(0) {}

	Crosshair(int w, int h) : width(w), height(h), pixels(w, std::vector<Pixel>(h)) {}

	//set pixel color
	void SetColor(int x, int y, Pixel c) {
		if (x >= 0 && x < width && y >= 0 && y < height) {
			pixels[x][y] = { c.red, c.green, c.blue, c.alpha };
		}
	}

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

	//save as file
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
};