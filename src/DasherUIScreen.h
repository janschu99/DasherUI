#pragma once

#include <DasherScreen.h>

#include "DasherInput.h"
#include "imgui.h"

class DasherUIScreen : public Dasher::CDasherScreen, public Dasher::CScreenCoordInput
{
public:
	DasherUIScreen();

	std::pair<Dasher::screenint, Dasher::screenint> TextSize(Label* label, unsigned iFontSize) override;
	void DrawString(Label* label, Dasher::screenint x, Dasher::screenint y, unsigned iFontSize, int iColour) override;
	void DrawRectangle(Dasher::screenint x1, Dasher::screenint y1, Dasher::screenint x2, Dasher::screenint y2, int Colour, int iOutlineColour, int iThickness) override;
	void DrawCircle(Dasher::screenint iCX, Dasher::screenint iCY, Dasher::screenint iR, int iFillColour, int iLineColour, int iLineWidth) override;
	void Polyline(point* Points, int Number, int iWidth, int Colour) override;
	void Polygon(point* Points, int Number, int fillColour, int outlineColour, int lineWidth) override;
	void Display() override;
	void SetColourScheme(const Dasher::CColourIO::ColourInfo* pColourScheme) override;
	bool IsPointVisible(Dasher::screenint x, Dasher::screenint y) override;

	bool MultiSizeFonts() override { return true; }
	
	bool SetCanvasSize(ImVec2 CanvasPos, ImVec2 CanvasSize);

	bool GetScreenCoords(Dasher::screenint& iX, Dasher::screenint& iY, Dasher::CDasherView* pView) override;

private:
	const Dasher::CColourIO::ColourInfo* pColorScheme;
	ImVec2 CanvasPos;
	ImVec2 CanvasSize;

	ImFont* Font;
};
