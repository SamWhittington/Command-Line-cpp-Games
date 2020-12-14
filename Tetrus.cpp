#include <iostream>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono_literals;

#include <Windows.h>
#include <vector>
#include <wchar.h>

// These are the types of blocks
wstring tetblocks[7];

// Defining the game zone
int nZoneWidth = 12;
int nZoneHeight = 18;
unsigned char *pZone = nullptr; // Dynamically allocating 

int nScreenWidth = 80; // The screen size of the console (80 columns)
int nScreenHeight = 30; // The screen size of the console (30 rows) 

// A function to index rotations of the tetrus pieces
// We have an indexed 4*4 block and this are the results 
//after a roation r 

int Rotate(int px, int py, int r) {
	switch (r % 4) {
	case 0: return py * 4 + px;		   // 0 degree rotation
	case 1: return 12 + py - (px * 4); // 90 degree rotation
	case 2: return 15 - (py * 4) - px; // 180 degree rotation 
	case 3: return 3 - py + (py * 4);  // 270 degree rotation 
	}
	return 0; 
}

bool DoesPieceFit(int ntetblocks, int nRotation, int nPosX, int nPosY) {
	
	// Nested for loop to iterate across the space a peice can occupy
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++) {
			
			//Get array index of tetblock in the peice-space
			int pi = Rotate(px, py, nRotation);

			// Index into the playing zone (based on where the peice currently is) 
			int fi = (nPosY + py) * nZoneWidth + (nPosX + px);

			// Ensure we're not going outside of alotted memory when doing array checks (ie. out of alotted array)

			if (nPosX + px >= 0 && nPosX + px < nZoneWidth) {
				if (nPosY + py >= 0 && nPosY + py < nZoneHeight) {

					if (tetblocks[ntetblocks][pi] == L'X' && pZone[fi] != 0)
						return false; // i.e. a situation in which the peice doesn't fit 
				}
			}
		}
	
	return true;
}


int main() {

	// Creating the assets 
	tetblocks[0].append(L"..X.");
	tetblocks[0].append(L"..X.");
	tetblocks[0].append(L"..X.");
	tetblocks[0].append(L"..X.");
	
	tetblocks[1].append(L"..X.");
	tetblocks[1].append(L".XX.");
	tetblocks[1].append(L".X..");
	tetblocks[1].append(L"....");

	tetblocks[2].append(L".X..");
	tetblocks[2].append(L".XX.");
	tetblocks[2].append(L"..X.");
	tetblocks[2].append(L"....");

	tetblocks[3].append(L"....");
	tetblocks[3].append(L".XX.");
	tetblocks[3].append(L".XX.");
	tetblocks[3].append(L"....");

	tetblocks[4].append(L"..X.");
	tetblocks[4].append(L".XX.");
	tetblocks[4].append(L"..X.");
	tetblocks[4].append(L"....");

	tetblocks[5].append(L"....");
	tetblocks[5].append(L".XX.");
	tetblocks[5].append(L"..X.");
	tetblocks[5].append(L"..X..");

	tetblocks[6].append(L"....");
	tetblocks[6].append(L".XX.");
	tetblocks[6].append(L".X..");
	tetblocks[6].append(L".X..");


	// Everything is initialized to be zero unless at the side of the array or bottom

	pZone = new unsigned char[nZoneWidth * nZoneHeight]; // Creates the play zone buffer
	for (int x = 0; x < nZoneWidth; x++) // Outer boundary
		for (int y = 0; y < nZoneHeight; y++)
			pZone[y * nZoneWidth + x] = (x == 0 || x == nZoneWidth - 1 || y == nZoneHeight - 1) ? 9 : 0;


	// Some code I've reused to make the command line as a screen buffer 

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Game Logic:
	bool bGameOver = false;

	int nCurrentPiece = 0;
	int nCurrentRoation = 0;
	int nCurrentX = nZoneWidth / 2;
	int nCurrentY = 0;

	// Imput keys:
	bool bKey[4];
	bool bRotateHold = false;

	// Game speeds and scores
	int nSpeed = 20;
	int nSpeedCounter = 0;
	bool bForceDown = false;
	int nPeiceCount = 0;
	int nScore = 0;


	vector<int> vLines;

	// Simple Game Loop (if game not over then keep outputting the game screen (the screen buffer array)) 

	while (!bGameOver) {

		// Timing for the game ------------------------
		this_thread::sleep_for(50ms); // One game clock tick 
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);


		// Input --------------------------------------
		for (int k = 0; k < 4; k++)
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0; // Keys are \x27\x25\x28Z == R L D arrows and Z key
			// GetAsync is function to see whether key is currently pressed or not

		// Game Logic ---------------------------------

		// Keys to be pressed / Player input movement:
		if (bKey[1]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRoation, nCurrentX - 1, nCurrentY)) {
				nCurrentX = nCurrentX - 1; // If the peice fits update the current X var - for left key
			}
		}
		
		if (bKey[0]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRoation, nCurrentX + 1, nCurrentY)) {
				nCurrentX = nCurrentX + 1; // If the peice fits update the current X var - for right key
			}
		}

		if (bKey[2]) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRoation, nCurrentX, nCurrentY + 1)) {
				nCurrentY = nCurrentY + 1; // If the peice fits update the current X var - for right key
			}
		}

		if (bKey[3]) {
			if (!bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRoation + 1, nCurrentX, nCurrentY)) {
				nCurrentRoation = nCurrentRoation + 1; // Rotation key (Z) - checks peice fits !!!!!Kinda buggy, should probs fix !!!!
				bRotateHold = true; // This line is attempting to fix the above (Still jumping around so further attention required) 
			}
			
		}
		else bRotateHold = false;


		// Game movement

		if (bForceDown) {
			if (DoesPieceFit(nCurrentPiece, nCurrentRoation, nCurrentX, nCurrentY + 1)) {
				nCurrentY++; // If the peice fits update the current X var - for right key
			}
			else {
				// Force current peice into the game play area
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (tetblocks[nCurrentPiece][Rotate(px, py, nCurrentRoation)] == L'X') // Same as described below 
							pZone[(nCurrentY + py) * nZoneWidth + (nCurrentX + px)] = nCurrentPiece + 1;

				nPeiceCount++;
				if (nPeiceCount % 10 == 0)
					if (nSpeed >= 10) nSpeed--;
				
				// Created horizontle lines? 
				for (int py = 0; py < 4; py++) // Only need to check for lines within space of the peice
					if (nCurrentY + py < nZoneHeight - 1) {
						bool bLine = true;
						for (int px = 1; px < nZoneWidth - 1; px++)
							bLine &= (pZone[(nCurrentY + py) * nZoneWidth + px]) != 0;

						if (bLine) {
							// Remove the horizontle lines we've created
							for (int px = 1; px < nZoneWidth - 1; px++)
								pZone[(nCurrentY + py) * nZoneWidth + px] = 8;

							vLines.push_back(nCurrentY + py);
						}
					}

				nScore += 25;
				if (!vLines.empty()) nScore += (1 << vLines.size()) * 100; // Big bounuses for big lines!

				// Next peice 
				nCurrentX = nZoneWidth / 2;
				nCurrentY = 0;
				nCurrentRoation = 0;
				nCurrentPiece = rand() % 7;

				// Peice doesn't fit 
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRoation, nCurrentX, nCurrentY);
			}

			nSpeedCounter = 0;
		}


		// Render Output ------------------------------

		// Draw Zone
		for (int x = 0; x < nZoneWidth; x++)
			for (int y = 0; y < nZoneHeight; y++)
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pZone[y * nZoneWidth + x]]; 

		// Draw Falling Tetrus peice
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (tetblocks[nCurrentPiece][Rotate(px, py, nCurrentRoation)] == L'X') // Getting index according to current rotation (if = X then draw to screen)
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65; 

		// Display score
		swprintf_s(&screen[2 * nScreenWidth + nZoneWidth + 6], 16, L"SCORE: %8d", nScore);
		
		if (!vLines.empty()) {

			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms); // Delay


			// Moves all peices down if a rows are completed
			for (auto &v : vLines)
				for (int px = 1; px < nZoneWidth - 1; px++) {
					for (int py = v; py > 0; py--)
						pZone[py * nZoneWidth + px] = pZone[(py - 1) * nZoneWidth + px]; // Replaces positions below with thoses above them
					pZone[px] = 0;
				}

			vLines.clear();

		}
		
		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);

	}
	
	// GAME OVER MESSAGE
	CloseHandle(hConsole);
	cout << "!!GAME OVER!! You scored: " << nScore << endl;
	system("pause");


	return 0; 
}
