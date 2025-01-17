#include "BitmapManager.h"

BitmapManager::BitmapManager(string basePath)
{
	m_basePath = basePath;

}

/// <summary>
/// Loads the bitmap.
/// </summary>
/// <param name="fileName">Name of the file.</param>
void BitmapManager::loadBitmap(string fileName)
{

	ifstream image{ getFullMap(fileName), ios::in | ios::binary };

	if (image.is_open())
	{
		image.read((char*)&m_fileHeader, sizeof(m_fileHeader));
		image.read((char*)&m_infoHeader, sizeof(m_infoHeader));
		image.read((char*)&m_colorHeader, sizeof(m_colorHeader));

		m_data.resize(m_infoHeader.size_image);

		image.seekg(m_fileHeader.offset, image.beg);
		image.read((char*)m_data.data(), m_data.size());

		image.close();
	}
	else
	{
		throw runtime_error("Could not open file");
	}
}

/// <summary>
/// Writes to file.
/// </summary>
/// <param name="fileName">Name of the file.</param>
void BitmapManager::writeToFile(string fileName)
{
	ofstream outImage{ getFullMap(fileName), ios_base::binary };
	if (outImage)
	{
		outImage.write((char*)&m_fileHeader, sizeof(m_fileHeader));
		outImage.write((char*)&m_infoHeader, sizeof(m_infoHeader));
		outImage.write((char*)&m_colorHeader, sizeof(m_colorHeader));
		outImage.write((char*)m_data.data(), m_data.size());
	}
}

/// <summary>
/// Generate a basic BMP
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
void BitmapManager::generateBasic(int width, int height)
{
	m_fileHeader = BMPFilheader();
	m_fileHeader.file_size = (width * height * 4) + 138;
	m_fileHeader.offset = 138;

	m_infoHeader = BMPInfoHeader();
	m_infoHeader.size = 124;
	m_infoHeader.bit_count = 32;
	m_infoHeader.width = width;
	m_infoHeader.height = height;
	m_infoHeader.size_image = width * height;

	m_colorHeader = BMPColorHeader();

	m_data = vector<unsigned char>(width * height * 4);
}

/// <summary>
/// Build a cercle
/// </summary>
/// <param name="rayon"></param>
/// <param name="offsetX"></param>
/// <param name="offsetY"></param>
void BitmapManager::buildCercle(int rayon, int offsetX, int offsetY)
{
	fillBackground();

	offsetX = (m_infoHeader.width / 2) + offsetX;
	offsetY = (m_infoHeader.height / 2) + offsetY;

	//build cercle
	for (int i = 0; i < m_data.size(); i += 4)
	{
		int x = i / 4 % m_infoHeader.width;
		int y = i / 4 / m_infoHeader.width;

		if (sqrt(pow(x - offsetX, 2) + pow(y - offsetY, 2)) < rayon)
		{
			m_data[i] = 0;
			m_data[i + 1] = 0;
			m_data[i + 2] = 0;
			m_data[i + 3] = 255;
		}
	}


}

/// <summary>
/// Build a rectangle
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="offsetX"></param>
/// <param name="offsetY"></param>
void BitmapManager::buildRectangle(int width, int height, int offsetX, int offsetY)
{
	fillBackground();

	int widthCenter = (m_infoHeader.width / 2) + offsetX;
	int heightCenter = (m_infoHeader.height / 2) + offsetY;

	for (int i = 0; i < m_data.size(); i += 4)
	{
		int x = i / 4 % m_infoHeader.width;
		int y = i / 4 / m_infoHeader.width;

		if ((x < widthCenter + width && x > widthCenter - width)
			&& (y < heightCenter + height && y > heightCenter - height))
		{
			m_data[i] = 0;
			m_data[i + 1] = 0;
			m_data[i + 2] = 0;
			m_data[i + 3] = 255;
		}
	}

}

/// <summary>
/// Fill de background
/// </summary>
void BitmapManager::fillBackground()
{
	// Fill the image to color
	for (int i = 0; i < m_data.size(); i += 4)
	{
		m_data[i] = 255;
		m_data[i + 1] = 0;
		m_data[i + 2] = 100;
		m_data[i + 3] = 255;
	}
}

void BitmapManager::blur()
{
	for (int i = 0; i < m_data.size(); i += 4)
	{
		int r = m_data[i];
		int g  = m_data[i + 1];
		int b = m_data[i + 2];
		int nbVal = 1;

		ExtractEachAdjacent(i, nbVal, r, g, b);

		m_data[i] = r / nbVal;
		m_data[i + 1] = g / nbVal;
		m_data[i + 2] = b / nbVal;
		m_data[i + 3] = 255;
	}
}

void BitmapManager::ExtractEachAdjacent(int i, int& nbVal, int& r, int& g, int& b)
{
	if (i - m_infoHeader.width * 4 > 0)
	{
		nbVal++;
		r += m_data[i - m_infoHeader.width * 4];
		g += m_data[i - m_infoHeader.width * 4 + 1];
		b += m_data[i - m_infoHeader.width * 4 + 2];
	}

	if (i + m_infoHeader.width * 4 < m_data.size())
	{
		nbVal++;
		r += m_data[i + m_infoHeader.width * 4];
		g += m_data[i + m_infoHeader.width * 4 + 1];
		b += m_data[i + m_infoHeader.width * 4 + 2];
	}

	if (i - 4 > 0)
	{
		nbVal++;
		r += m_data[i - 4];
		g += m_data[i - 4 + 1];
		b += m_data[i - 4 + 2];
	}

	if (i + 4 < m_data.size())
	{
		nbVal++;
		r += m_data[i + 4];
		g += m_data[i + 4 + 1];
		b += m_data[i + 4 + 2];
	}
}

void BitmapManager::negativeFilter()
{
	for (int i = 0; i < m_data.size(); i += 4)
	{
		m_data[i] = 255 - m_data[i];
		m_data[i + 1] = 255 - m_data[i + 1];
		m_data[i + 2] = 255 - m_data[i + 2];
	}

}

/// <summary>
/// Hide a message in Bmp
/// </summary>
/// <param name="message"></param>
void BitmapManager::hideMessage(string message)
{
	int index = 0;
	int step = 4 * m_infoHeader.width;

	m_data[step + 3] = message.length();

	for (int i = step * 2; i < m_data.size(); i += step)
	{
		if (index + 1 > message.length())
		{
			return;
		}

		m_data[i + 3] = int(message[index]);
		index++;
	}
}

/// <summary>
/// Read the hidden message
/// </summary>
/// <returns>The message</returns>
string BitmapManager::readMessage()
{
	std::string message = "";

	int index = 0;
	int step = 4 * m_infoHeader.width;

	int messageLenght = m_data[step + 3];

	for (int i = step * 2; i < m_data.size(); i += step)
	{
		if (index + 1 > messageLenght)
		{
			break;
		}

		message += (char)m_data[i + 3];
		index++;
	}

	return message;
}


/// <summary>
/// Gets the full path.
/// </summary>
/// <param name="path">The path.</param>
/// <returns></returns>
string BitmapManager::getFullMap(string path)
{
	return m_basePath + "/" + path;
}
