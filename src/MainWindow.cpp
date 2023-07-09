#include "MainWindow.h"

#include <filesystem>

#include "FileUtils.h"
#include "nfd.h"
#include "XmlSettingsStore.h"

MainWindow::MainWindow()
{
	ImGui::SetCurrentFont(LoadFonts(14.0f));
	ImGui::SetNextFrameWantCaptureKeyboard(true);

	Settings = make_unique<Dasher::XmlSettingsStore>("Settings.xml", this);
	Settings->Load();
	Settings->Save();

	Controller = make_unique<DasherController>(Settings.get());
	Controller->Initialize();

	Controller->GetPermittedValues(SP_ALPHABET_ID, Alphabets);

}

bool MainWindow::render(float DeltaTime)
{
    static ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground ;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
	const ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    if (ImGui::Begin("MainWindow", nullptr, flags))
    {
	    if(ImGui::BeginMainMenuBar())
	    {
            if (ImGui::BeginMenu("File"))
	        {
	            if (ImGui::MenuItem("Load TrainingData", "CTRL+T"))
	            {
					nfdu8char_t *outPath;
					if(NFD_OpenDialogU8(&outPath, nullptr, 0, nullptr) == NFD_OKAY)
					{
						Controller->ImportTrainingText(outPath);
						NFD_FreePathU8(outPath);
					}
	            }
				if (ImGui::MenuItem("Quit", "CTRL+Q"))
	            {
		            return false;
	            }
	            ImGui::EndMenu();
	        }
			if (ImGui::BeginMenu("Edit"))
	        {
	            if (ImGui::MenuItem("Copy", "CTRL+C"))
	            {
					CopyTextToClipboard(*Controller->GetBufferRef());
	            }
				if (ImGui::MenuItem("Clear", "CTRL+R"))
	            {
					ClearBuffer();
	            }
				if (ImGui::MenuItem("Save to SVG"))
				{
					Controller->SaveToSVG();
				}
				static bool save_svg_pressed = false;
				if(ImGui::IsKeyPressed(ImGuiKey_ModCtrl) && ImGui::IsKeyPressed(ImGuiKey_S) && !save_svg_pressed)
				{
					Controller->SaveToSVG();
					save_svg_pressed = true;
				}
				else
				{
					save_svg_pressed = false;
				}

				int speed = Controller->GetLongParameter(LP_MAX_BITRATE);
				if(ImGui::SliderInt ("Speed", &speed, 1, 2000))
				{
					Controller->SetLongParameter(LP_MAX_BITRATE, speed);
				}

				static int item_current = 0;
				if(ImGui::Combo("Alphabet", &item_current, [](void* data, int idx, const char** out_text) { *out_text = static_cast<const std::vector<std::string>*>(data)->at(idx).c_str(); return true; }, (void*)&Alphabets, static_cast<int>(Alphabets.size()), 10))
				{
					Controller->SetStringParameter(SP_ALPHABET_ID, Alphabets[item_current]);
				}

	            ImGui::EndMenu();
	        }
	    	if (ImGui::BeginMenu("Help"))
	        {
	            if (ImGui::MenuItem("Demo Window"))
	            {
					DisplayDemoWindow = !DisplayDemoWindow;
	            }
	            ImGui::EndMenu();
	        }

			ImGui::EndMainMenuBar();
	    }


    	ImVec2 MainWindowSize = ImGui::GetContentRegionMax();
        ImGui::InputTextMultiline("##DasherOutput", Controller->GetBufferRef(), ImVec2(-(MainWindowSize.x*0.1f + spacing.x), MainWindowSize.y * 0.1f));
		ImGui::SameLine();
		ImGui::BeginGroup();
			float CursorPosXGroup = ImGui::GetCursorScreenPos().x;
			ImVec2 ButtonSize = ImVec2(-FLT_MIN, MainWindowSize.y*0.05f - spacing.y * 0.5f);
			if(ImGui::Button("Copy", ButtonSize))
			{
				CopyTextToClipboard(*Controller->GetBufferRef());
			}
			ImGui::SetCursorPosX(CursorPosXGroup);
			if(ImGui::Button("Clear", ButtonSize))
			{
				ClearBuffer();
			}
		ImGui::EndGroup();

	    const ImVec2 canvasPos = ImGui::GetCursorScreenPos();
	    const ImVec2 canvasSize = ImGui::GetContentRegionAvail();

		ImGui::PushClipRect(canvasPos, canvasPos + canvasSize, false);

	    ImDrawList* WindowDrawList = ImGui::GetWindowDrawList();
		WindowDrawList->AddRectFilled(
			canvasPos,
			ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y),
			ImGui::ColorConvertFloat4ToU32({0,1,0,1})
		);

		Controller->Render(DeltaTime * 1000.0f, canvasPos, canvasSize); //convert to millis

		ImGui::PopClipRect();
    }
    ImGui::End();

	if(DisplayDemoWindow) ImGui::ShowDemoWindow(&DisplayDemoWindow);
	return true;
}

void MainWindow::CopyTextToClipboard(const std::string& text) const
{
	Controller->CopyToClipboard(text);
}

void MainWindow::ClearBuffer() const
{
	Controller->GetBufferRef()->clear();
	Controller->SetBuffer(0);
}

ImFont* MainWindow::LoadFonts(float pixel_size)
{
	ImVector<ImWchar> ranges;
	ImFontGlyphRangesBuilder builder;
	builder.AddRanges(ImGui::GetIO().Fonts->GetGlyphRangesJapanese());
	builder.AddText("\xE2\x96\xA1"); // box
	builder.BuildRanges(&ranges);

	ImFontConfig config;
	config.MergeMode = true;
	ImGuiIO& io = ImGui::GetIO();
	ImFont* Font = io.Fonts->AddFontFromFileTTF("./Resources/NotoSans-Medium.ttf", pixel_size, nullptr, io.Fonts->GetGlyphRangesDefault());
	io.Fonts->AddFontFromFileTTF("./Resources/NotoSansJP-Medium.otf", pixel_size, &config, ranges.Data); //merge with first one
	io.Fonts->Build();

	return Font;
}
