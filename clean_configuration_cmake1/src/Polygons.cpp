#include "Polygons.h"
#include <iostream>
#include <algorithm> //for std::sort
#include <cstring>
#include <fstream>
#include<stb/stb_image.h>
#include <unordered_set>

namespace Polygons {
	unsigned char* convertRgbToBinaryImage(unsigned char* rgbImage, size_t width, size_t height, unsigned char redCircleVal, unsigned char greenCircleVal, unsigned char blueCircleVal) {
		size_t binaryImageSize = width * height;
		unsigned char* binaryImage = new unsigned char[binaryImageSize];
		if (!binaryImage)
			return NULL;

		size_t binaryIndex = 0;
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < 3 * width; j += 3) {
				size_t redIndex = i * 3 * width + j + 0;
				size_t greenIndex = i * 3 * width + j + 1;
				size_t blueIndex = i * 3 * width + j + 2;

				unsigned char redVal = rgbImage[redIndex];
				unsigned char greenVal = rgbImage[greenIndex];
				unsigned char blueVal = rgbImage[blueIndex];

				if (redVal == redCircleVal && greenVal == greenCircleVal && blueVal == blueCircleVal) {
					binaryImage[binaryIndex] = 255;
				}
				else {
					binaryImage[binaryIndex] = 0;
				}
				binaryIndex++;
			}
		}
		return binaryImage;
	}


	/*
* Description - Get the continuous boundary points
* Parameters
* InputImage    - Input image
* Width_i        - Width of the image
* Height_i        - Height of Image
* BoundaryPoints - Vector of boundary points (output)
*/
	void GetContinousBoundaryPoints(unsigned char* InputImage, int Width_i, int Height_i, std::vector<Point2D>& BoundaryPoints)
	{
		int nImageSize = Width_i * Height_i;
		if (NULL != InputImage)
		{
			int Offset[8][2] = {
									{ -1, -1 },       //  +----------+----------+----------+
									{ 0, -1 },        //  |          |          |          |
									{ 1, -1 },        //  |(x-1,y-1) | (x,y-1)  |(x+1,y-1) |
									{ 1, 0 },         //  +----------+----------+----------+
									{ 1, 1 },         //  |(x-1,y)   |  (x,y)   |(x+1,y)   |
									{ 0, 1 },         //  |          |          |          |
									{ -1, 1 },        //  +----------+----------+----------+
									{ -1, 0 }         //  |          | (x,y+1)  |(x+1,y+1) |
			};                    //  |(x-1,y+1) |          |          |
								  //  +----------+----------+----------+
			const int NEIGHBOR_COUNT = 8;
			Point2D BoundaryPixelToSave; //david
			Point2D BoundaryPixelCord;
			Point2D BoundaryStartingPixelCord;
			Point2D BacktrackedPixelCord;
			int BackTrackedPixelOffset[1][2] = { {0,0} };
			bool bIsBoundaryFound = false;
			bool bIsStartingBoundaryPixelFound = false;
			for (int Idx = 0; Idx < nImageSize; ++Idx) // getting the starting pixel of boundary
			{
				if (0 != InputImage[Idx])
				{
					BoundaryPixelCord.X = Idx % Width_i;
					BoundaryPixelCord.Y = Idx / Width_i;
					BoundaryStartingPixelCord = BoundaryPixelCord;
					BacktrackedPixelCord.X = (Idx - 1) % Width_i;
					BacktrackedPixelCord.Y = (Idx - 1) / Width_i;
					BackTrackedPixelOffset[0][0] = BacktrackedPixelCord.X - BoundaryPixelCord.X;
					BackTrackedPixelOffset[0][1] = BacktrackedPixelCord.Y - BoundaryPixelCord.Y;
					BoundaryPixelToSave.Y = BoundaryPixelCord.X;
					BoundaryPixelToSave.X = Height_i - BoundaryPixelCord.Y;
					BoundaryPoints.push_back(BoundaryPixelToSave);
					bIsStartingBoundaryPixelFound = true;
					break;
				}
			}
			Point2D CurrentBoundaryCheckingPixelCord;
			Point2D PrevBoundaryCheckingPixxelCord;
			if (!bIsStartingBoundaryPixelFound)
			{
				BoundaryPoints.pop_back();
			}
			while (true && bIsStartingBoundaryPixelFound)
			{
				int CurrentBackTrackedPixelOffsetInd = -1;
				for (int Ind = 0; Ind < NEIGHBOR_COUNT; ++Ind)
				{
					if (BackTrackedPixelOffset[0][0] == Offset[Ind][0] &&
						BackTrackedPixelOffset[0][1] == Offset[Ind][1])
					{
						CurrentBackTrackedPixelOffsetInd = Ind;// Finding the bracktracked 
															   // pixel's offset index
						break;
					}
				}
				int Loop = 0;
				while (Loop < (NEIGHBOR_COUNT - 1) && CurrentBackTrackedPixelOffsetInd != -1)
				{
					int OffsetIndex = (CurrentBackTrackedPixelOffsetInd + 1) % NEIGHBOR_COUNT;
					CurrentBoundaryCheckingPixelCord.X = BoundaryPixelCord.X + Offset[OffsetIndex][0];
					CurrentBoundaryCheckingPixelCord.Y = BoundaryPixelCord.Y + Offset[OffsetIndex][1];
					int ImageIndex = CurrentBoundaryCheckingPixelCord.Y * Width_i +
						CurrentBoundaryCheckingPixelCord.X;
					if (0 != InputImage[ImageIndex])// finding the next boundary pixel
					{
						BoundaryPixelCord = CurrentBoundaryCheckingPixelCord;
						BacktrackedPixelCord = PrevBoundaryCheckingPixxelCord;
						BackTrackedPixelOffset[0][0] = BacktrackedPixelCord.X - BoundaryPixelCord.X;
						BackTrackedPixelOffset[0][1] = BacktrackedPixelCord.Y - BoundaryPixelCord.Y;
						BoundaryPixelToSave.Y = BoundaryPixelCord.X;
						BoundaryPixelToSave.X = Height_i - BoundaryPixelCord.Y;
						BoundaryPoints.push_back(BoundaryPixelToSave);
						break;
					}
					PrevBoundaryCheckingPixxelCord = CurrentBoundaryCheckingPixelCord;
					CurrentBackTrackedPixelOffsetInd += 1;
					Loop++;
				}
				if (BoundaryPixelCord.X == BoundaryStartingPixelCord.X &&
					BoundaryPixelCord.Y == BoundaryStartingPixelCord.Y) // if the current pixel = 
																		 // starting pixel
				{
					BoundaryPoints.pop_back();
					bIsBoundaryFound = true;
					break;
				}
			}
			if (!bIsBoundaryFound) // If there is no connected boundary clear the list
			{
				BoundaryPoints.clear();
			}
		}
	}

	void fill_image_with_zeros(unsigned char*& image, size_t width, size_t height) {
		size_t numOfBytes = width * height;
		for (size_t i = 0; i < numOfBytes; i++)
		{
			image[i] = 0;
		}
	}

	bool checkIfPairInList(std::vector<std::vector<unsigned char>> listPairs, std::vector<unsigned char> pairLabels) {
		size_t numOfPairs = listPairs.size();

		for (int i = 0; i < numOfPairs; i++) {
			unsigned char value1 = listPairs[i][0];
			unsigned char value2 = listPairs[i][1];
			if (value1 == pairLabels[0] && value2 == pairLabels[1]) {
				return true;
			}
		}
		return false;
	}

	unsigned char getSecondValueOfPair(unsigned char sourceValue, std::vector<std::vector<unsigned char>> listPairs, size_t columnIndex, bool& isSingleValueInList) {
		unsigned char destValue = 0; //random value in case that isSingleValueInList=false
		isSingleValueInList = false;
		size_t numOfPairs = listPairs.size();
		size_t destIndex;

		if (columnIndex == 0) {
			destIndex = 1;
		}
		else {
			destIndex = 0;
		}

		for (int i = 0; i < numOfPairs; i++) {
			unsigned char currentVal = listPairs[i][columnIndex];
			if (sourceValue == currentVal) {
				isSingleValueInList = true;
				destValue = listPairs[i][destIndex];
			}
		}

		return destValue;
	}

	void updateTemp(std::vector<std::vector<unsigned char>>& temp, std::vector<std::vector<unsigned char>> listIdenticalLabels, size_t i, size_t j, int& counter)
	{
		unsigned char empty_value = 0;
		size_t num_of_pairs = listIdenticalLabels.size();
		size_t temp_size = 5 + 3 * num_of_pairs;
		std::vector<unsigned char> temp_current_val(temp_size, 0);
		for (int i = 0; i < temp_size; i++)
		{
			temp_current_val[i] = 0;
		}
		temp_current_val[0] = j + 1;
		temp_current_val[1] = i + 1;
		temp_current_val[2] = empty_value;
		temp_current_val[3] = num_of_pairs;
		temp_current_val[4] = empty_value;
		for (size_t j = 0; j < num_of_pairs; j++) {
			unsigned char val1 = listIdenticalLabels[j][0];
			unsigned char val2 = listIdenticalLabels[j][1];
			size_t index = 5 + 3 * j - 1;
			temp_current_val[index + 1] = val1;
			temp_current_val[index + 2] = val2;
			temp_current_val[index + 3] = empty_value;
		}

		counter = counter + 1;
		temp.push_back(temp_current_val);
	}


	void replaceMaxValueWithMinValueInSecondColumn(std::vector<std::vector<unsigned char>>& listIdenticalLabels, unsigned char newMinValue, unsigned char maxValue) {
		if (maxValue == 66 && newMinValue == 1) {
			int david = 4;
		}
		size_t numOfPairs = listIdenticalLabels.size();
		for (size_t i = 0; i < numOfPairs; i++) {				
			unsigned char currentValColumn2 = listIdenticalLabels[i][1];
			if (currentValColumn2 == maxValue) {
				if (i == 63)
				{
					int david = 4;
				}
				listIdenticalLabels[i][1] = newMinValue;
			}
		}
	}

	unsigned char calcNewMinValue(std::vector<std::vector<unsigned char>>& listIdenticalLabels, unsigned char minValue) {
		size_t numOfPairs = listIdenticalLabels.size();
		unsigned char newMinValue = minValue;

		for (size_t i = 0; i < numOfPairs; i++) {
			unsigned char currentValColumn1 = listIdenticalLabels[i][0];
			if (currentValColumn1 == minValue) {
				newMinValue = listIdenticalLabels[i][1];
			}
		}

		return newMinValue;
	}

	void insertToList(std::vector<std::vector<unsigned char>>& listIdenticalLabels, std::vector<unsigned char> pairLabels) {
		unsigned char value1 = pairLabels[0];
		unsigned char value2 = pairLabels[1];
		unsigned char minValue;
		unsigned char maxValue;

		std::vector<unsigned char> currentPair;
		currentPair.push_back(0);
		currentPair.push_back(0);

		std::vector<unsigned char> newPairLabels1;
		newPairLabels1.push_back(0);
		newPairLabels1.push_back(0);

		std::vector<unsigned char> newPairLabels2;
		newPairLabels2.push_back(0);
		newPairLabels2.push_back(0);

		if (value1 < value2) {
			minValue = value1;
			maxValue = value2;
		}
		else {
			minValue = value2;
			maxValue = value1;
		}

		pairLabels[0] = maxValue;
		pairLabels[1] = minValue;

		bool isPairInList = checkIfPairInList(listIdenticalLabels, pairLabels);
		if (isPairInList == true) {
			return;
		}

		size_t columnIndex = 1;
		bool isSingleValueInList1 = false;
		bool isSingleValueInList2 = false;
		unsigned char destVal1 = getSecondValueOfPair(maxValue, listIdenticalLabels, columnIndex, isSingleValueInList1);
		unsigned char destVal2 = getSecondValueOfPair(minValue, listIdenticalLabels, columnIndex, isSingleValueInList2);

		if (isSingleValueInList1 == true) {
			//  for example we want to insert(19, 12).
			//	the pair(19, 12) is not in the list.
			//	but the pair(25, 19) is in the list.
			//	therefore, we need to replace all occurances of 19 in the second
			//	column to 12.
			minValue = calcNewMinValue(listIdenticalLabels, minValue);
			replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, minValue, maxValue);			
			currentPair[0] = maxValue;
			currentPair[1] = minValue;
			listIdenticalLabels.push_back(currentPair);
			return;
		}

		if (isSingleValueInList2 == true) {
			//  for example we want to insert(19, 12).
			//	the pair(19, 12) is not in the list.
			//	but the pair(30, 12) is in the list.
			//	therefore, we insert(19, 12)
			currentPair[0] = maxValue;
			currentPair[1] = minValue;
			listIdenticalLabels.push_back(currentPair);
			return;
		}

		columnIndex = 0;
		bool isSingleValueInList3 = false;
		unsigned char destVal3 = getSecondValueOfPair(maxValue, listIdenticalLabels, columnIndex, isSingleValueInList3);
		if (isSingleValueInList3 == true) {
			//  for example we want to insert(19, 12).
			//	the pair(19, 12) is not in the list.
			//	if the pair(19, 8) is in the list.
			//	we need to replace all the occurances of 19 or 12 in the second
			//	column to 8.
			//	and if the pair(19, 16) is in the list.
			//	we need to replace all the occurances of 19 or 16 in the second
			//	column to 12.
			//	this is in order to make it consistent that the second column is
			//	always smaller than the first column.
			if (destVal3 < minValue) {
				destVal3 = calcNewMinValue(listIdenticalLabels, destVal3);
				replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, destVal3, minValue);
				replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, destVal3, maxValue);
				newPairLabels1[0] = minValue;
				newPairLabels1[1] = destVal3;
				bool isPairInList1 = checkIfPairInList(listIdenticalLabels, newPairLabels1);
				if (isPairInList1 == false) {
					listIdenticalLabels.push_back(newPairLabels1);
				}
				newPairLabels2[0] = maxValue;
				newPairLabels2[1] = destVal3;
				bool isPairInList2 = checkIfPairInList(listIdenticalLabels, newPairLabels2);
				if (isPairInList2 == false) {
					listIdenticalLabels.push_back(newPairLabels2);
				}
			}
			else {
				minValue = calcNewMinValue(listIdenticalLabels, minValue);
				replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, minValue, destVal3);
				replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, minValue, maxValue);
				newPairLabels1[0] = maxValue;
				newPairLabels1[1] = minValue;
				bool isPairInList1 = checkIfPairInList(listIdenticalLabels, newPairLabels1);
				if (isPairInList1 == false) {
					listIdenticalLabels.push_back(newPairLabels1);
				}
				newPairLabels2[0] = destVal3;
				newPairLabels2[1] = minValue;
				bool isPairInList2 = checkIfPairInList(listIdenticalLabels, newPairLabels2);
				if (isPairInList2 == false) {
					listIdenticalLabels.push_back(newPairLabels2);
				}
			}
			return;
		}

		bool isSingleValueInList4 = false;
		unsigned char destVal4 = getSecondValueOfPair(minValue, listIdenticalLabels, columnIndex, isSingleValueInList4);
		if (isSingleValueInList4 == true) {
			//  for example we want to insert(19, 12).
			//	the pair(19, 12) is not in the list.
			//	but the pair(12, 8) is in the list.
			//	therefore, we need to replace all occurances of 12 and 19 in the
			//	second column to 8.
			//	dest_val4 must be smaller than min_value
			destVal4 = calcNewMinValue(listIdenticalLabels, destVal4);
			replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, destVal4, minValue);
			replaceMaxValueWithMinValueInSecondColumn(listIdenticalLabels, destVal4, maxValue);
			newPairLabels1[0] = minValue;
			newPairLabels1[1] = destVal4;
			bool isPairInList1 = checkIfPairInList(listIdenticalLabels, newPairLabels1);
			if (isPairInList1 == false) {
				listIdenticalLabels.push_back(newPairLabels1);
			}
			newPairLabels2[0] = maxValue;
			newPairLabels2[1] = destVal4;
			bool isPairInList2 = checkIfPairInList(listIdenticalLabels, newPairLabels2);
			if (isPairInList2 == false) {
				listIdenticalLabels.push_back(newPairLabels2);
			}
			return;
		}

		if (isSingleValueInList1 == false && isSingleValueInList2 == false && isSingleValueInList3 == false && isSingleValueInList4 == false) {
			currentPair[0] = maxValue;
			currentPair[1] = minValue;
			listIdenticalLabels.push_back(currentPair);
		}
		
	}

	void writeTempToFile(std::vector<std::vector<unsigned char>> temp, std::string fileFullPath) {
		std::ofstream myfile;
		size_t numOfPixels = temp.size();
		myfile.open(fileFullPath);
		for (size_t i = 0; i < numOfPixels; i++)
		{
			if (i % 10000 == 0)
			{
				std::cout << "pixel " << i + 1 << " out of " << numOfPixels << std::endl;
			}			
			std::vector<unsigned char> dataForCurrentPixel = temp[i];
			size_t dataSize = dataForCurrentPixel.size();
			for (size_t j = 0; j < dataSize; j++) {
				unsigned char currentVal = dataForCurrentPixel[j];
				myfile << (int)currentVal << ", ";
			}
			for (size_t j = dataSize; j < 1199; j++) {
				myfile << (int)0 << ", ";
			}
			myfile << (int)0;

			myfile << std::endl;
		}
		myfile.close();
	}

	void bwLabelsFirstScan(unsigned char* binaryImage, size_t width, size_t height, int connectivity, std::vector<std::vector<unsigned char>>& listIdenticalLabels, unsigned char*& imageLabels) {
		size_t imageSize = width * height;
		imageLabels = new unsigned char[imageSize];
		fill_image_with_zeros(imageLabels, width, height);

		std::vector<std::vector<unsigned char>> temp;
		int counter = 0;

		unsigned char labelCounter = 0;		
		for (size_t i = 0; i < width; i++)
		{
			std::cout << "Col " << i+1 << " out of " << width << std::endl;
			for (size_t j = 0; j < height; j++)
			{
				//std::cout << "Row " << i << "/" << height << ", Col " << j << "/" << width << std::endl;

				size_t currentIndex = i * height + j;
				size_t indexFormerJ = i * height + (j - 1);
				size_t indexFormerI = (i - 1) * height + j;
				size_t indexFormerIJ = (i - 1) * height + (j - 1);

				unsigned char neighbor1;
				unsigned char neighbor2;
				unsigned char neighbor3;
				std::vector<unsigned char> neighbors;
				std::vector<unsigned char> identicalLabels;

				unsigned char pixelVal = binaryImage[currentIndex];
				if (pixelVal == 0) //background
				{
					updateTemp(temp, listIdenticalLabels, i, j, counter);
					continue;
				}

				if (i == 0 && j == 0)
				{
					labelCounter++;
					imageLabels[currentIndex] = labelCounter;
					updateTemp(temp, listIdenticalLabels, i, j, counter);
					continue;
				}

				if (i == 0 && j > 0) {
					neighbor1 = imageLabels[indexFormerJ];
					if (neighbor1 > 0) {
						imageLabels[currentIndex] = neighbor1;
					}
					else {
						labelCounter++;
						imageLabels[currentIndex] = labelCounter;
					}
					updateTemp(temp, listIdenticalLabels, i, j, counter);
					continue;
				}
				if (i > 0 && j == 0) {
					neighbor1 = imageLabels[indexFormerI];
					if (neighbor1 > 0) {
						imageLabels[currentIndex] = neighbor1;
					}
					else {
						labelCounter++;
						imageLabels[currentIndex] = labelCounter;
					}
					updateTemp(temp, listIdenticalLabels, i, j, counter);
					continue;
				}

				if (i > 0 && j > 0) {
					neighbor1 = imageLabels[indexFormerJ];
					neighbor2 = imageLabels[indexFormerI];
					if (connectivity == 8) {
						neighbor3 = imageLabels[indexFormerIJ];
					}

					if (connectivity == 8) {
						neighbors.push_back(neighbor1);
						neighbors.push_back(neighbor2);
						neighbors.push_back(neighbor3);
					}
					else {
						neighbors.push_back(neighbor1);
						neighbors.push_back(neighbor2);
					}

					std::sort(neighbors.begin(), neighbors.end());
					neighbors.erase(std::unique(neighbors.begin(), neighbors.end()), neighbors.end());
					//now neighbors is sorted and unique (no value appears twice)

					if (neighbors[0] == 0) {
						neighbors.erase(neighbors.begin());
					}

					size_t numOfLabeledNeighbors = neighbors.size();

					if (numOfLabeledNeighbors == 0) { //all neighbors are background					
						labelCounter++;
						imageLabels[currentIndex] = labelCounter;
					}
					else if (numOfLabeledNeighbors == 1) {
						imageLabels[currentIndex] = neighbors[0];
					}
					else { //there are two labels that are identival (to fix in the next scan)
						imageLabels[currentIndex] = neighbors[0];
						identicalLabels.push_back(neighbors[1]);
						identicalLabels.push_back(neighbors[0]);

						if (j == 492 && i == 730) {
							int david = 5;
						}
						insertToList(listIdenticalLabels, identicalLabels);
						if (numOfLabeledNeighbors == 3 && connectivity == 8) {
							identicalLabels[0] = neighbors[2];
							identicalLabels[1] = neighbors[1];
							insertToList(listIdenticalLabels, identicalLabels);
						}
					}
				}
				updateTemp(temp, listIdenticalLabels, i, j, counter);
			}
		}
		std::string fileFullPath = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/temp.csv";
		writeTempToFile(temp, fileFullPath);
	}



	void copyArrays(unsigned char* sourceArray, unsigned char* & destArray, size_t width, size_t height) {
		size_t numOfBytes = width * height;
		for (size_t i = 0; i < numOfBytes; i++) {
			destArray[i] = sourceArray[i];
		}
	}


	unsigned char* bwLabelsSecondScan(unsigned char* imageLabelsFirstScan, size_t width, size_t height, std::vector<std::vector<unsigned char>> listIdenticalLabels) {
		size_t numOfPairs = listIdenticalLabels.size();
		size_t nSize = width * height;
		unsigned char* imageLabels = new unsigned char[nSize];
		copyArrays(imageLabelsFirstScan, imageLabels, width, height);
		if (numOfPairs == 0) {
			//no second scan is needed. imageLabelsFirstScan is what we need.
			return imageLabels;
		}

		for (int k = 0; k < numOfPairs; k++) {
			std::cout << "second scan: pair " << k + 1 << " out of " << numOfPairs << "." << std::endl;
			unsigned char sourceVal = listIdenticalLabels[k][0];
			unsigned char destVal = listIdenticalLabels[k][1];
			for (int i = 0; i < height; i++) {
				for (int j = 0; j < width; j++) {
					size_t currentIndex = i * width + j;
					unsigned char currentVal = imageLabelsFirstScan[currentIndex];
					if (currentVal == sourceVal) {
						imageLabels[currentIndex] = destVal;
					}
				}
			}
		}
		return imageLabels;
	}

	void writeMatrixToFile(unsigned char* image, size_t width, size_t height, std::string fileFullPath) {
		std::ofstream myfile;
		myfile.open(fileFullPath);
		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++) {
				size_t currentIndex = i * width + j;
				unsigned char currentVal = image[currentIndex];
				if (j < width - 1)
				{
					myfile << (int)currentVal << ", ";
				}
				else
				{
					myfile << (int)currentVal;
				}
			}
			myfile << std::endl;
		}
		myfile.close();
	}

	void writeMatrixToFileAsSingleColumn(unsigned char* image, size_t width, size_t height, std::string fileFullPath) {
		std::ofstream myfile;
		myfile.open(fileFullPath);
		size_t numOfBytes = width * height;
		for (size_t i = 0; i < numOfBytes; i++)
		{
			unsigned char currentVal = image[i];
			if (i < numOfBytes - 1)
			{
				myfile << (int)currentVal << ", ";
			}
			else
			{
				myfile << (int)currentVal;
			}
		}
		myfile << std::endl;
		myfile.close();
	}

	void writePairsToFile(std::vector<std::vector<unsigned char>> listIdenticalLabels, std::string fileFullPath) {
		size_t numOfPairs = listIdenticalLabels.size();
		std::ofstream myfile;
		myfile.open(fileFullPath);
		for (size_t i = 0; i < numOfPairs; i++)
		{
			myfile << (int)listIdenticalLabels[i][0] << ", " << (int)listIdenticalLabels[i][1] << std::endl;
		}
		myfile.close();
	}
	

	unsigned char* convertBinaryImageToLabelsImage(unsigned char* binaryImage, size_t width, size_t height) {
		std::vector<std::vector<unsigned char>> listIdenticalLabels;
		int connectivity = 4;
		unsigned char* imageLabelsFirstScan;
		bwLabelsFirstScan(binaryImage, width, height, connectivity, listIdenticalLabels, imageLabelsFirstScan);

		std::string pairsCsvFullPath = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/pairs.csv";
		writePairsToFile(listIdenticalLabels, pairsCsvFullPath);

		std::string labelsImageFirstScanCsvFullPath = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/labelsImageFirstScan.csv";
		writeMatrixToFileAsSingleColumn(imageLabelsFirstScan, width, height, labelsImageFirstScanCsvFullPath);
		unsigned char* imageLabels = bwLabelsSecondScan(imageLabelsFirstScan, width, height, listIdenticalLabels);
		return imageLabels;
	}


	unsigned char* changeOrderOfBytes(unsigned char* originalArr, size_t width, size_t height) {
		unsigned char* destArr = new unsigned char[width * height];
		int counter = 0;
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				size_t new_index = width * (height - j - 1) + i;
				destArr[counter] = originalArr[new_index];
				counter++;
			}
		}
		return destArr;
	}

	unsigned char* reverseChangeOrderOfBytes(unsigned char* originalArr, size_t width, size_t height) {
		unsigned char* destArr = new unsigned char[width * height];
		int counter = 0;
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				size_t new_index = height * (j + 1) - i - 1;
				destArr[counter] = originalArr[new_index];
				counter++;
			}
		}
		return destArr;
	}

	std::unordered_set<unsigned char> getUniqueLabels(unsigned char* labelsImage, size_t width, size_t height) {
		size_t numOfPixels = width * height;
		std::unordered_set<unsigned char> uniqueVals(labelsImage, labelsImage + numOfPixels);
		auto backgroundIterator = uniqueVals.find(0);
		uniqueVals.erase(backgroundIterator);
		return uniqueVals;
	}

	void buildBinaryImageWithCurrentLabel(unsigned char*& binaryImage, unsigned char* labelsImage, unsigned char label, size_t width, size_t height) {
		size_t numOfPixels = width * height;
		for (int i = 0; i < numOfPixels; i++) {
			if (labelsImage[i] == label) {
				binaryImage[i] = 1;
			}
			else
			{
				binaryImage[i] = 0;
			}
		}
	}

	std::vector<std::vector<Point2D>> calcPolygons(size_t width, size_t height) {
		std::vector<std::vector<Point2D>> BoundaryPoints;
		size_t nSize = width * height * 3;
		unsigned char* rgbImage = new unsigned char[nSize];
		if (!rgbImage)
			return BoundaryPoints;

		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, rgbImage);

		unsigned char redCircleVal = 255;
		unsigned char greenCircleVal = 0;
		unsigned char blueCircleVal = 0;

		unsigned char* binaryImage = convertRgbToBinaryImage(rgbImage, width, height, redCircleVal, greenCircleVal, blueCircleVal);
		
		//copyArrays(binaryImage, binaryImage1, width, height);

		//int widthImg, heightImg, numColCh;
		//// Flips the image so it appears right side up
		//stbi_set_flip_vertically_on_load(true);
		//// Reads the image from a file and stores it in bytes
		//std::string imagePath = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/small_image.bmp";
		//unsigned char* binaryImage1 = stbi_load(imagePath.c_str(), &widthImg, &heightImg, &numColCh, 0);




		unsigned char* binaryImage1 = changeOrderOfBytes(binaryImage, width, height);
		unsigned char* binaryImage2 = reverseChangeOrderOfBytes(binaryImage1, width, height);






		//width = 10;
		//height = 10;
		//unsigned char* binaryImage = new unsigned char[width * height];
		//for (size_t i = 0; i < height; i++) {
		//	for (size_t j = 0; j < width; j++) {
		//		size_t currentIndex = i * width + j;
		//		if (i >= 1 && i <= 1 && j >= 1 && j <= 3) {
		//			binaryImage[currentIndex] = 1;
		//		}
		//		else if (i >= 1 && i <= 1 && j >= 6 && j <= 8) {
		//			binaryImage[currentIndex] = (unsigned char)1;
		//		}
		//		else if (i >= 2 && i <= 2 && j >= 0 && j <= width-1) {
		//			binaryImage[currentIndex] = (unsigned char)1;
		//		}
		//		else {
		//			binaryImage[currentIndex] = (unsigned char)0;
		//		}
		//	}
		//}

		std::string binaryImageCsvFullPath1 = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/binaryImage1.csv";
		writeMatrixToFileAsSingleColumn(binaryImage1, width, height, binaryImageCsvFullPath1);



		//stbi_image_free(binaryImage1);

		unsigned char* labelsImage = convertBinaryImageToLabelsImage(binaryImage1, width, height);
		unsigned char* labelsImage2 = reverseChangeOrderOfBytes(labelsImage, width, height);


		std::string labelsImageCsvFullPath = "C:/Users/David Cohn/Documents/Github/OpenGL/clean_configuration_cmake1/matlab/Boundary_tracing_using_the_Moore_neighbourhood/labelsImage.csv";
		writeMatrixToFileAsSingleColumn(labelsImage, width, height, labelsImageCsvFullPath);

		std::unordered_set<unsigned char> uniqueLabels = getUniqueLabels(labelsImage2, width, height);
		size_t numOfLabels = uniqueLabels.size();

		for (auto it = uniqueLabels.begin(); it != uniqueLabels.end(); it++) {
			std::vector<Point2D> BoundaryPointsForCurrentLabel;
			unsigned char currentLabel = *it;
			buildBinaryImageWithCurrentLabel(binaryImage, labelsImage2, currentLabel, width, height);
			GetContinousBoundaryPoints(binaryImage, width, height, BoundaryPointsForCurrentLabel);
			BoundaryPoints.push_back(BoundaryPointsForCurrentLabel);
		}
		

		delete[] rgbImage;
		delete[] binaryImage;
		delete[] binaryImage1;
		delete[] binaryImage2;
		delete[] labelsImage;
		delete[] labelsImage2;

		return BoundaryPoints;
	}
}