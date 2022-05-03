#include <iostream>
#include <fstream>
#include <vector>
#include "BitmapManager.h"
#include "Headers.h"

using namespace std;


int main()
{
    BitmapManager bitmapManager =  BitmapManager("BitmapTest");

	bitmapManager.loadBitmap("Shapes.bmp");

	//bitmapManager.generateBasic(600, 400);

	bitmapManager.negativeFilter();

	bitmapManager.blur();
	
	//HIDE MESSAGE
	bitmapManager.hideMessage("Mon message est le suivant");

	string hiddenMessage = bitmapManager.readMessage();

	cout << "Message hidden : " << hiddenMessage << endl;

	//bitmapManager.writeToFile("createdBitmap.bmp");

	//bitmapManager.generateBasic(600, 400);
	
	//bitmapManager.buildCercle(50, 0, 0);

	//bitmapManager.buildRectangle(20, 30, 10, 0);
	
	bitmapManager.writeToFile("createdBitmapV3.bmp");
}
