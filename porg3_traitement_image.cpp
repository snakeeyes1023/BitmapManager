#include <iostream>
#include <fstream>
#include <vector>
#include "BitmapManager.h"
#include "Headers.h"

using namespace std;


int main()
{
    BitmapManager bitmapManager =  BitmapManager("BitmapTest");

	//bitmapManager.loadBitmap("Shapes.bmp");

	//bitmapManager.writeToFile("createdBitmap.bmp");

	bitmapManager.generateBasic(600, 400);
	
	//bitmapManager.buildCercle(3, 2);

	bitmapManager.buildRectangle(20, 30, 10, 0);
	
	bitmapManager.writeToFile("createdBitmapCercle.bmp");
}
