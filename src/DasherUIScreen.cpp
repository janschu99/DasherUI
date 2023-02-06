#include "DasherUIScreen.h"
#include "imgui.h"
#include "MainWindow.h"
#include "imgui/imgui_internal.h"

DasherUIScreen::DasherUIScreen(): Dasher::CDasherScreen(100,100), CScreenCoordInput(0, _("Mouse Input"))
{
	Font = MainWindow::LoadFonts(50.0f);
}

std::pair<Dasher::screenint, Dasher::screenint> DasherUIScreen::TextSize(Label* Label, unsigned iFontSize)
{
	const ImVec2 Size = Font->CalcTextSizeA(iFontSize, FLT_MAX, -1, Label->m_strText.c_str());
	return {Size.x, Size.y};
}

void DasherUIScreen::DrawString(Label* label, Dasher::screenint x, Dasher::screenint y, unsigned iFontSize, int iColour)
{
	const Dasher::CColourIO::ColourInfo::PaletteColor Color = this->pColorScheme->Colors[iColour];
	const ImVec4 ImColor = { static_cast<float>(Color.Red) / 255.0f, static_cast<float>(Color.Green) / 255.0f, static_cast<float>(Color.Blue) / 255.0f, 1.0f };
	const ImVec2 Pos = { static_cast<float>(x),static_cast<float>(y)};
	ImGui::GetWindowDrawList()->AddText(Font, static_cast<float>(iFontSize), Pos + CanvasPos, ImGui::ColorConvertFloat4ToU32(ImColor), label->m_strText.c_str());

	if(SaveNextToSVG)
	{
		SVGDocument->add_child<SVG::Text>(Pos.x, Pos.y, label->m_strText)
			->set_attr("fill", "rgb(" + std::to_string(Color.Red) + "," + std::to_string(Color.Green) + "," + std::to_string(Color.Blue) + ")")
			.set_attr("font-size", iFontSize)
			.set_attr("font-family", "Arial, Helvetica, sans-serif")
			.set_attr("dominant-baseline", "hanging");
	}
}

void DasherUIScreen::DrawRectangle(Dasher::screenint x1, Dasher::screenint y1, Dasher::screenint x2, Dasher::screenint y2, int Colour, int iOutlineColour, int iThickness)
{
	const Dasher::CColourIO::ColourInfo::PaletteColor FillColor = this->pColorScheme->Colors[Colour];
	const ImVec4 ImFillColor = { static_cast<float>(FillColor.Red) / 255.0f, static_cast<float>(FillColor.Green) / 255.0f, static_cast<float>(FillColor.Blue) / 255.0f, 1.0f };
	const ImVec2 p1 = { static_cast<float>(x1), static_cast<float>(y1)};
	const ImVec2 p2 = { static_cast<float>(x2), static_cast<float>(y2)};

	ImGui::GetWindowDrawList()->AddRectFilled(CanvasPos + p1, CanvasPos + p2, ImGui::ColorConvertFloat4ToU32(ImFillColor));

	if(iThickness > 0)
	{
		const Dasher::CColourIO::ColourInfo::PaletteColor OutlineColor = this->pColorScheme->Colors[Colour];
		const ImVec4 ImOutlineColor = { static_cast<float>(OutlineColor.Red) / 255.0f, static_cast<float>(OutlineColor.Green) / 255.0f, static_cast<float>(OutlineColor.Blue) / 255.0f, 1.0f };

		ImGui::GetWindowDrawList()->AddRect(CanvasPos + p1, CanvasPos + p2, ImGui::ColorConvertFloat4ToU32(ImOutlineColor),0,0, static_cast<float>(iThickness));
	}

	if (SaveNextToSVG)
	{
		const Dasher::CColourIO::ColourInfo::PaletteColor OutlineColor = this->pColorScheme->Colors[Colour];
		SVGDocument->add_child<SVG::Rect>(p1.x, p1.y, p2.x - p1.x, p2.y - p1.y)
			->set_attr("fill", "rgb(" + std::to_string(FillColor.Red) + "," + std::to_string(FillColor.Green) + "," + std::to_string(FillColor.Blue) + ")")
			.set_attr("stroke", "rgb(" + std::to_string(OutlineColor.Red) + "," + std::to_string(OutlineColor.Green) + "," + std::to_string(OutlineColor.Blue) + ")")
			.set_attr("stroke-width", iThickness);
	}
}

void DasherUIScreen::DrawCircle(Dasher::screenint iCX, Dasher::screenint iCY, Dasher::screenint iR, int iFillColour, int iLineColour, int iLineWidth)
{
	const Dasher::CColourIO::ColourInfo::PaletteColor FillColor = this->pColorScheme->Colors[iFillColour];
	const ImVec4 ImFillColor = { static_cast<float>(FillColor.Red) / 255.0f, static_cast<float>(FillColor.Green) / 255.0f, static_cast<float>(FillColor.Blue) / 255.0f, 1.0f };

	ImGui::GetWindowDrawList()->AddCircleFilled(CanvasPos + ImVec2(static_cast<float>(iCX), static_cast<float>(iCY)), static_cast<float>(iR), ImGui::ColorConvertFloat4ToU32(ImFillColor));

	if (iLineWidth > 0)
	{
		const Dasher::CColourIO::ColourInfo::PaletteColor OutlineColor = this->pColorScheme->Colors[iLineColour];
		const ImVec4 ImOutlineColor = { static_cast<float>(OutlineColor.Red) / 255.0f, static_cast<float>(OutlineColor.Green) / 255.0f, static_cast<float>(OutlineColor.Blue) / 255.0f, 1.0f };

		ImGui::GetWindowDrawList()->AddCircle(CanvasPos + ImVec2(static_cast<float>(iCX), static_cast<float>(iCY)), static_cast<float>(iR), ImGui::ColorConvertFloat4ToU32(ImFillColor),0, static_cast<float>(iLineWidth));
	}

	if (SaveNextToSVG)
	{
		const Dasher::CColourIO::ColourInfo::PaletteColor OutlineColor = this->pColorScheme->Colors[iLineColour];
		SVGDocument->add_child<SVG::Circle>(iCX, iCY, iR)
			->set_attr("fill", "rgb(" + std::to_string(FillColor.Red) + "," + std::to_string(FillColor.Green) + "," + std::to_string(FillColor.Blue) + ")")
			.set_attr("stroke", "rgb(" + std::to_string(OutlineColor.Red) + "," + std::to_string(OutlineColor.Green) + "," + std::to_string(OutlineColor.Blue) + ")")
			.set_attr("stroke-width", iLineWidth);
	}
}

void DasherUIScreen::Polyline(point* Points, int Number, int iWidth, int Colour)
{
	const Dasher::CColourIO::ColourInfo::PaletteColor LineColor = this->pColorScheme->Colors[Colour];
	const ImVec4 ImLineColor = { static_cast<float>(LineColor.Red) / 255.0f, static_cast<float>(LineColor.Green) / 255.0f, static_cast<float>(LineColor.Blue) / 255.0f, 1.0f };

	std::vector<ImVec2> points;

	for(unsigned int i = 0; i < Number; i++)
	{
		points.push_back(CanvasPos + ImVec2(static_cast<float>(Points[i].x), static_cast<float>(Points[i].y)));
	}

	ImGui::GetWindowDrawList()->AddPolyline(points.data(), Number, ImGui::ColorConvertFloat4ToU32(ImLineColor), 0, static_cast<float>(iWidth));

	if (SaveNextToSVG && Number >= 2)
	{
		const auto Polyline = SVGDocument->add_child<SVG::Path>();

		Polyline->start(Points[0].x, Points[0].y);
		for (unsigned int i = 1; i < Number; i++)
		{
			Polyline->line_to(Points[i].x, Points[i].y);
		}
		
		Polyline->set_attr("stroke", "rgb(" + std::to_string(LineColor.Red) + "," + std::to_string(LineColor.Green) + "," + std::to_string(LineColor.Blue) + ")");
		Polyline->set_attr("stroke-width", iWidth);
	}
}

void DasherUIScreen::Polygon(point* Points, int Number, int fillColour, int outlineColour, int lineWidth)
{
	const Dasher::CColourIO::ColourInfo::PaletteColor FillColor = this->pColorScheme->Colors[fillColour];
	const ImVec4 ImFillColor = { static_cast<float>(FillColor.Red) / 255.0f, static_cast<float>(FillColor.Green) / 255.0f, static_cast<float>(FillColor.Blue) / 255.0f, 1.0f };

	std::vector<ImVec2> points;

	for (unsigned int i = 0; i < Number; i++)
	{
		points.push_back(CanvasPos + ImVec2(static_cast<float>(Points[i].x), static_cast<float>(Points[i].y)));
	}

	ImGui::GetWindowDrawList()->AddConvexPolyFilled(points.data(), Number, ImGui::ColorConvertFloat4ToU32(ImFillColor));

	if(lineWidth > 0 && !SaveNextToSVG)
	{
		Polyline(Points, Number, lineWidth, outlineColour);
	}

	if (SaveNextToSVG)
	{
		std::vector<SVG::Point> points;
		const Dasher::CColourIO::ColourInfo::PaletteColor LineColor = this->pColorScheme->Colors[outlineColour];

		for (unsigned int i = 0; i < Number; i++)
		{
			points.push_back(SVG::Point(static_cast<double>(Points[i].x), static_cast<double>(Points[i].y)));
		}

		auto Polygon = SVGDocument->add_child<SVG::Polygon>(points);
		Polygon->set_attr("fill", "rgb(" + std::to_string(FillColor.Red) + "," + std::to_string(FillColor.Green) + "," + std::to_string(FillColor.Blue) + ")");
		Polygon->set_attr("stroke", "rgb(" + std::to_string(LineColor.Red) + "," + std::to_string(LineColor.Green) + "," + std::to_string(LineColor.Blue) + ")");
		Polygon->set_attr("stroke-width", lineWidth);
	}
}

void DasherUIScreen::Display()
{
	if(SaveNextToSVG)
	{
		SVGDocument->autoscale();
		std::ofstream outfile("DasherView.svg");
		outfile << std::string(*SVGDocument);
		SaveNextToSVG = false;
		SVGDocument.reset();
	}

}

void DasherUIScreen::SetColourScheme(const Dasher::CColourIO::ColourInfo* pColourScheme)
{
	this->pColorScheme = pColourScheme;
}

bool DasherUIScreen::IsPointVisible(Dasher::screenint, Dasher::screenint){
	return true;
}

bool DasherUIScreen::SetCanvasSize(const ImVec2 position, const ImVec2 size)
{
	if(fabs(this->CanvasSize.x - size.x) > std::numeric_limits<float>::epsilon() || fabs(this->CanvasSize.y - size.y) > std::numeric_limits<float>::epsilon())
	{
		resize(static_cast<Dasher::screenint>(size.x), static_cast<Dasher::screenint>(size.y));
		this->CanvasSize = size;
		this->CanvasPos = position;
		return true;
	}

	this->CanvasPos = position;
	return false;
}

bool DasherUIScreen::GetScreenCoords(Dasher::screenint& iX, Dasher::screenint& iY, Dasher::CDasherView* pView)
{
	const ImVec2 MousePos = ImGui::GetMousePos();
	if (MousePos.x < 0 || MousePos.y < 0) return false;
	iX = static_cast<Dasher::screenint>(MousePos.x - CanvasPos.x);
	iY = static_cast<Dasher::screenint>(MousePos.y - CanvasPos.y);
	return true;
}
