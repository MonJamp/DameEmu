#include "DebuggerWidget.h"
#include <imgui.h>


DebuggerWidget::DebuggerWidget(std::shared_ptr<Debugger> d, bool& r)
	: debugger(d), running(r)
{
	window_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
	table_flags = ImGuiTableFlags_ScrollY;
	column_flags = ImGuiTableColumnFlags_WidthFixed;

	addBreakpointPopup = false;
	address_index = 0;
	focus_address = false;
	selected = -1;

	RefreshDisassembly();
	RefreshBreakpoints();
	ShowCurrentAddress();
}

DebuggerWidget::~DebuggerWidget()
{
}

bool DebuggerWidget::BreakExecution()
{
	if (debugger->HitBreakpoint())
	{
		debugger->Break();
		ShowCurrentAddress();
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

void DebuggerWidget::ShowCurrentAddress()
{
	ShowAddress(debugger->regState.cpu.pc);
}

void DebuggerWidget::ShowAddress(uint16_t a)
{
	address_index = addressToIndex[a];
	focus_address = true;
}

void DebuggerWidget::Show()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.f, 2.f));
	ImGui::SetNextWindowPos(ImVec2(160.f * 3.f, ImGui::GetFrameHeight()), ImGuiCond_Once);
	ImGui::SetNextWindowSize(ImVec2(320.f, 144.f * 3.f + ImGui::GetTextLineHeightWithSpacing()), ImGuiCond_Once);
	ImGui::Begin("Debugger", NULL, window_flags);

	if (ImGui::Button("Step"))
	{
		debugger->Step();
		ShowCurrentAddress();
	}

	ImGui::SameLine();

	if (running)
	{
		if (ImGui::Button("Break"))
		{
			debugger->Break();
			ShowCurrentAddress();
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

	if (ImGui::BeginTable("##dis_table", 5, table_flags, ImVec2(-1.f, 250)))
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

	if (ImGui::CollapsingHeader("Breakpoints", ImGuiTreeNodeFlags_DefaultOpen))
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

	ImGui::End();
	ImGui::PopStyleVar();
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
