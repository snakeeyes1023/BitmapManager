#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "Headers.h"

using namespace std;

/// <summary>
/// Manage Bitmap image
/// </summary>
class BitmapManager
{
public:
		
	BitmapManager(string basePath);
		
	void loadBitmap(string fileName);

	void writeToFile(string fileName);

	void generateBasic(int width, int height);

	void buildCercle(int rayon, int offsetX, int offsetY);

	void buildRectangle(int width, int height, int offsetX, int offsetY);

	void fillBackground();

	void blur();

	void ExtractEachAdjacent(int i, int& nbVal, int& r, int& g, int& b);

	void negativeFilter();

	void hideMessage(string message);

	string readMessage();
	
private:
	string m_basePath;
	
	#pragma region Bitmap component

	vector<uint8_t> m_data{};
	BMPFilheader m_fileHeader;
	BMPInfoHeader m_infoHeader;
	BMPColorHeader m_colorHeader;
	
	#pragma endregion

	string getFullMap(string path);
};

