#include "DebuggerWidget.h"
#include "Add-ons/imgui_memory_editor.h"
#include <imgui.h>
#include <imgui-SFML.h>


VramWidget::VramWidget(std::shared_ptr<Bus> b)
	: bus(b)
{
	scale = 1.5f;
	posX = 0;
	posY = 0;
	tileIndex = 0;

	texture = new sf::Texture();
	texture->create(16 * 8, 16 * 8);
	sprite.setTexture(*texture);
	sprite.setScale(scale, scale);

	uint8_t white_pixels[8 * 8 * 4] = { 0xFF };

	for (uint16_t i = 0; i < 256; i++)
	{
		sf::Texture tileTex;
		tileTex.create(8, 8);
		tileTex.update(white_pixels);

		texture->update(tileTex, posX, posY);

		posX += 8;
		if (posX == 8 * 16)
		{
			posX = 0;
			posY += 8;
		}
	}

	posX = 0;
	posY = 0;
}

VramWidget::~VramWidget()
{
	delete texture;
}

void VramWidget::OnUpdate()
{
	uint8_t pixels[8 * 8 * 4];
	std::array<uint8_t, 0x2000>* vram = bus->GetVRAM();

	for (int i = 0; i < 8; i++)
	{
		uint8_t upper = vram->at(tileIndex * 16 + i * 2);
		uint8_t lower = vram->at(tileIndex * 16 + i * 2 + 1);

		for (int j = 0; j < 8; j++)
		{
			bool upperBit = upper & 128;
			bool lowerBit = lower & 128;
			uint32_t color = 0x00000000;

			if (upperBit && lowerBit)
			{
				//Color 4
				color = 0xFF000000;
			}
			else if (upperBit && !lowerBit)
			{
				//Color 3
				color = 0xFFA9A9A9;
			}
			else if (!upperBit && lowerBit)
			{
				//Color 2
				color = 0xFF545454;
			}
			else
			{
				//Color 1
				color = 0xFFFFFFFF;
			}


			pixels[i * 32 + j * 4] = color & 0xFF;
			pixels[i * 32 + j * 4 + 1] = (color & 0xFF00) >> 8;
			pixels[i * 32 + j * 4 + 2] = (color & 0xFF0000) >> 16;
			pixels[i * 32 + j * 4 + 3] = (color & 0xFF000000) >> 24;

			upper <<= 1;
			lower <<= 1;
		}
	}


	texture->update(pixels, 8, 8, posX, posY);

	tileIndex++;
	if (tileIndex > 255)
	{
		tileIndex = 0;
	}

	posX += 8;
	if (posX == 8 * 16)
	{
		posX = 0;
		posY += 8;
		if (posY == 8 * 16)
		{
			posY = 0;
		}
	}
}

#define IM_MAX(A, B) (((A) >= (B)) ? (A) : (B))
static void Square(ImGuiSizeCallbackData* data)
{
	data->DesiredSize.x = data->DesiredSize.y = IM_MAX(data->DesiredSize.x, data->DesiredSize.y);
}

void VramWidget::Show()
{
	OnUpdate();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowPos(ImVec2(320.f, ImGui::GetFrameHeight()), ImGuiCond_Once);
	float usableHeight = 720.f - (144.f * 3.f) - ImGui::GetFrameHeight() - ImGui::GetTextLineHeightWithSpacing() - 1.f;
	ImGui::SetNextWindowSize(ImVec2(usableHeight, usableHeight), ImGuiCond_Once);
	ImGui::SetNextWindowSizeConstraints(ImVec2(128.f, 128.f), ImVec2(FLT_MAX, FLT_MAX), Square);
	ImGui::Begin("Tile Viewer", NULL, ImGuiWindowFlags_NoScrollbar);
	ImVec2 p1 = ImGui::GetCursorScreenPos();
	ImVec2 winSize = ImGui::GetItemRectSize();
	winSize = { winSize.x, winSize.y - p1.y };
	float scale = ((float)winSize.x / 128.f) * 1.f;
	sprite.setScale(scale, scale);
	ImGui::Image(sprite);
	ImGui::End();
	ImGui::PopStyleVar();
}

DebuggerWidget::DebuggerWidget(std::shared_ptr<Debugger> d, bool& r)
	: debugger(d), running(r), map(d->GetBus()->GetMemoryDump()), vramWidget(d->GetBus())
{
	window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
	table_flags = ImGuiTableFlags_ScrollY;
	column_flags = ImGuiTableColumnFlags_WidthFixed;

	addBreakpointPopup = false;
	address_index = 0;
	focus_address = false;
	selected = -1;

	RefreshDisassembly();
	RefreshBreakpoints();
	FocusCurrentAddress();
}

DebuggerWidget::~DebuggerWidget()
{
}

bool DebuggerWidget::BreakExecution()
{
	if (debugger->HitBreakpoint())
	{
		debugger->Break();
		FocusCurrentAddress();
		return true;
	}

	return false;
}

void DebuggerWidget::RefreshDisassembly()
{
	Disassembly disasm = debugger->GetDisassembly();
	addressToIndex = debugger->GetAddressTable();

	disasmData.clear();

	for (auto &i : disasm)
	{
		InsData data;
		data.address = i.addressToStr();
		data.opcode = i.opcodeToStr();
		data.mnemonic = i.mnemonicToStr();
		data.operand = i.operandsToStr();
		data.comment = i.commentToStr();

		disasmData.push_back(data);
	}
}

void DebuggerWidget::RefreshBreakpoints()
{
	bpData.clear();

	for (auto& i : debugger->GetBreakpoints())
	{
		BreakpointData data;
		data.enabled = true;
		data.address = i.addressToStr();
		data.instruction = i.mnemonicToStr() + " " + i.operandsToStr();

		bpData.push_back(data);
	}

	BreakpointData data;
	data.enabled = false;
	data.address = "";
	data.instruction = "add_button";

	bpData.push_back(data);
}

void DebuggerWidget::FocusCurrentAddress()
{
	FocusAddress(debugger->regState.cpu.pc);
}

void DebuggerWidget::FocusAddress(uint16_t a)
{
	address_index = addressToIndex[a];
	focus_address = true;
}

void DebuggerWidget::Show()
{
	vramWidget.Show();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.f, 2.f));
	ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320.f, 720.f - ImGui::GetFrameHeight()), ImGuiCond_Once);
	ImGui::Begin("##DebuggerWindow", NULL, window_flags);

	if (ImGui::CollapsingHeader("Registers", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ShowRegisterCtrl();
	}

	if (ImGui::CollapsingHeader("Debugger", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ShowDebuggerCtrl();
	}

	if (ImGui::BeginTabBar("##tabBar"))
	{
		if (ImGui::BeginTabItem("Breakpoints"))
		{
			ShowBreakpointCtrl();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Memory Viewer"))
		{
			ShowMemoryCtrl();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();
	ImGui::PopStyleVar();
}

void DebuggerWidget::ShowRegisterCtrl()
{
	ImGui::BeginGroup();
	ImGui::Text("CPU");
	ImGui::Separator();
	ImGui::Text("PC: %04X", debugger->regState.cpu.pc);
	ImGui::Text("SP: %04X", debugger->regState.cpu.sp);
	ImGui::Text("AF: %04X", debugger->regState.cpu.af);
	ImGui::Text("BC: %04X", debugger->regState.cpu.bc);
	ImGui::Text("DE: %04X", debugger->regState.cpu.de);
	ImGui::Text("HL: %04X", debugger->regState.cpu.hl);

	ImGui::TextUnformatted("");

	ImGui::Text("Interrupt");
	ImGui::Text("IME: %d", debugger->regState.cpu.ime);
	ImGui::Text("REQ: %04X", debugger->regState.mem.int_req);
	ImGui::Text("ENB: %04X", debugger->regState.mem.int_ie);
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("Timer");
	ImGui::Separator();
	ImGui::Text("DIV:  %04X", debugger->regState.mem.div);
	ImGui::Text("TIMA: %04X", debugger->regState.mem.tima);
	ImGui::Text("TMA:  %04X", debugger->regState.mem.tma);
	ImGui::Text("TAC:  %04X", debugger->regState.mem.tac);

	ImGui::TextUnformatted("");

	ImGui::Text("IO");
	ImGui::Text("P1: %04X", debugger->regState.mem.p1);
	ImGui::Text("SB: %04X", debugger->regState.mem.sb);
	ImGui::Text("SC: %04X", debugger->regState.mem.sc);
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("LCD");
	ImGui::Separator();
	ImGui::Text("LCDC: %04X", debugger->regState.mem.lcdc);
	ImGui::Text("STAT: %04X", debugger->regState.mem.stat);
	ImGui::Text("LY:   %04X", debugger->regState.mem.ly);
	ImGui::Text("LYC:  %04X", debugger->regState.mem.lyc);
	ImGui::Text("SCY:  %04X", debugger->regState.mem.scy);
	ImGui::Text("SCX:  %04X", debugger->regState.mem.scx);
	ImGui::Text("BGP:  %04X", debugger->regState.mem.bgp);
	ImGui::Text("OBP0: %04X", debugger->regState.mem.obp0);
	ImGui::Text("OBP1: %04X", debugger->regState.mem.obp1);
	ImGui::Text("WY:   %04X", debugger->regState.mem.wy);
	ImGui::Text("WX:   %04X", debugger->regState.mem.wx);
	ImGui::EndGroup();

	ImGui::SameLine();

	ImGui::BeginGroup();
	ImGui::Text("Sound");
	ImGui::Separator();
	ImGui::BeginChild("##soundList", ImVec2(90.f, 185.f));
	ImGui::Text("NR10: %04X", debugger->regState.mem.nr10);
	ImGui::Text("NR11: %04X", debugger->regState.mem.nr11);
	ImGui::Text("NR12: %04X", debugger->regState.mem.nr12);
	ImGui::Text("NR13: %04X", debugger->regState.mem.nr13);
	ImGui::Text("NR14: %04X", debugger->regState.mem.nr14);
	ImGui::Text("NR21: %04X", debugger->regState.mem.nr21);
	ImGui::Text("NR22: %04X", debugger->regState.mem.nr22);
	ImGui::Text("NR23: %04X", debugger->regState.mem.nr23);
	ImGui::Text("NR24: %04X", debugger->regState.mem.nr24);
	ImGui::Text("NR30: %04X", debugger->regState.mem.nr30);
	ImGui::Text("NR31: %04X", debugger->regState.mem.nr31);
	ImGui::Text("NR32: %04X", debugger->regState.mem.nr32);
	ImGui::Text("NR33: %04X", debugger->regState.mem.nr33);
	ImGui::Text("NR34: %04X", debugger->regState.mem.nr34);
	ImGui::Text("NR41: %04X", debugger->regState.mem.nr41);
	ImGui::Text("NR42: %04X", debugger->regState.mem.nr42);
	ImGui::Text("NR43: %04X", debugger->regState.mem.nr43);
	ImGui::Text("NR44: %04X", debugger->regState.mem.nr44);
	ImGui::Text("NR50: %04X", debugger->regState.mem.nr50);
	ImGui::Text("NR51: %04X", debugger->regState.mem.nr51);
	ImGui::Text("NR52: %04X", debugger->regState.mem.nr52);
	ImGui::EndChild();
	ImGui::EndGroup();
}

void DebuggerWidget::ShowDebuggerCtrl()
{
	if (ImGui::Button("Step"))
	{
		debugger->Step();
		FocusCurrentAddress();
	}

	ImGui::SameLine();

	if (running)
	{
		if (ImGui::Button("Break"))
		{
			debugger->Break();
			FocusCurrentAddress();
			running = false;
		}
	}
	else
	{
		if (ImGui::Button("Run"))
		{
			running = true;
		}
	}

	if (ImGui::BeginTable("##dis_table", 5, table_flags, ImVec2(-1.f, 200)))
	{
		ImGui::TableSetupColumn("Address", column_flags, 25.f);
		ImGui::TableSetupColumn("Opcode", column_flags, 45.f);
		ImGui::TableSetupColumn("Instruction", column_flags, 35.f);
		ImGui::TableSetupColumn("Operands", column_flags, 70.f);
		ImGui::TableSetupColumn("Comment", column_flags, 45.f);

		ImGuiListClipper clipper;
		clipper.Begin(disasmData.size());
		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				// Set attributes of current row
				OnGetItemAttr(i);
				bool item_is_selected = (i == selected);
				if (ImGui::Selectable(
					disasmData[i].address.c_str(),
					item_is_selected,
					ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick))
				{
					selected = i;

					if (ImGui::GetIO().MouseDoubleClicked[0])
					{
						OnToggleBreakpoint(i);
					}
				}

				ImGui::TableNextColumn();
				ImGui::TextUnformatted(disasmData[i].opcode.c_str());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(disasmData[i].mnemonic.c_str());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(disasmData[i].operand.c_str());
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(disasmData[i].comment.c_str());

				ImGui::PopID();
			}
		}

		if (focus_address)
		{
			ImGui::SetScrollY(clipper.ItemsHeight * address_index);
			focus_address = false;
		}

		ImGui::EndTable();
	}
}

void DebuggerWidget::ShowBreakpointCtrl()
{
	if (ImGui::BeginTable("##bp_table", 4, table_flags))
	{
		ImGui::TableSetupColumn("##Enabled", column_flags, 10.f);
		ImGui::TableSetupColumn("Address", column_flags);
		ImGui::TableSetupColumn("Instruction", column_flags, 40.f);
		ImGui::TableSetupColumn("##action", column_flags, 20.f);
		ImGui::TableHeadersRow();

		RefreshBreakpoints();

		ImGuiListClipper clipper;
		clipper.Begin(bpData.size());
		while (clipper.Step())
		{
			for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();

				ImGui::Checkbox("##enabled", &bpData[i].enabled);
				ImGui::TableNextColumn();
				ImGui::TextUnformatted(bpData[i].address.c_str());
				ImGui::TableNextColumn();

				if (bpData[i].instruction == "add_button")
				{
					ImGui::TextUnformatted("");
					ImGui::TableNextColumn();

					if (ImGui::Button("Add"))
					{
						addBreakpointPopup = true;
					}

					if (addBreakpointPopup)
					{
						ImGui::OpenPopup("Add Breakpoint##popup");
						addBreakpointPopup = false;
					}

					if (ImGui::BeginPopup("Add Breakpoint##popup"))
					{
						char buffer[5] = "0000";
						if (ImGui::InputText("Address", buffer, sizeof(buffer),
							ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_EnterReturnsTrue))
						{
							uint16_t address = static_cast<uint16_t>(strtol(buffer, NULL, 16));
							debugger->AddBreakpoint(address);

							ImGui::CloseCurrentPopup();
							addBreakpointPopup = false;
						}

						ImGui::EndPopup();
					}
				}
				else
				{
					ImGui::TextUnformatted(bpData[i].instruction.c_str());
					ImGui::TableNextColumn();

					if (ImGui::Button("Remove"))
					{
						uint16_t address = static_cast<uint16_t>(strtol(bpData[i].address.c_str(), NULL, 16));
						debugger->RemoveBreakpoint(address);
					}
				}

				ImGui::PopID();
			}
		}

		ImGui::EndTable();
	}
}

void DebuggerWidget::ShowMemoryCtrl()
{
	static MemoryEditor memEdit;
	memEdit.ReadOnly = true;
	memEdit.Cols = 8;
	memEdit.DrawContents(map.raw.data(), map.raw.size(), 0x8000);
}

void DebuggerWidget::OnGetItemAttr(int item)
{
	for (auto i : debugger->GetBreakpoints())
	{
		uint16_t index = addressToIndex.at(i.address);
		if (item == index)
		{
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, 0xFF291EAF);
		}
	}

	if (debugger->regState.cpu.pc < 0x8000)
	{
		auto it = addressToIndex.find(debugger->regState.cpu.pc);
		uint16_t currAddressIndex = it->second;
		if (item == currAddressIndex)
		{
			ImGui::TableSetBgColor(ImGuiTableBgTarget_RowBg0, 0xFF406626);
		}
	}
}

void DebuggerWidget::OnToggleBreakpoint(int item)
{
	if (item == selected)
	{
		selected = -1;
	}

	uint16_t address = this->debugger->GetDisassembly()[item].address;
	this->debugger->ToggleBreakpoint(address);

	RefreshBreakpoints();
}
