#pragma once

auto drawmenu() -> void
{
	if (GetAsyncKeyState(VK_INSERT) & 1) { Settings::bMenu = !Settings::bMenu; }

	if (Settings::bMenu)
	{
		ImGui::Begin("i hate valorant", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

		ImGui::SetWindowSize("i hate valorant", ImVec2(300, 220));

		ImGui::Spacing();

		ImGui::Checkbox("team", &Settings::Visuals::team);
		ImGui::Checkbox("chams", &Settings::Visuals::chams);
		ImGui::Checkbox("glow", &Settings::Visuals::glow);

		ImGui::End();
	}
}