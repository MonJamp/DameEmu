#pragma once
#include "../Disassembler.h"
#include <memory>
#include <string>
#include <vector>


class DebuggerWidget
{
public:
	struct InsData
	{
		std::string address;
		std::string opcode;
		std::string mnemonic;
		std::string operand;
		std::string comment;
	};

	struct BreakpointData
	{
		bool enabled;
		std::string address;
		std::string instruction;
	};

public:
	DebuggerWidget(std::shared_ptr<Debugger> d, bool& running);
	~DebuggerWidget();

	bool BreakExecution();
	void RefreshDisassembly();
	void RefreshBreakpoints();
	void ShowCurrentAddress();
	void ShowAddress(uint16_t a);
	void Show();

private:
	void OnGetItemAttr(int item);
	void OnToggleBreakpoint(int item);

private:
	std::shared_ptr<Debugger> debugger;
	std::vector<InsData> disasmData;
	std::vector<BreakpointData> bpData;
	AddressTable addressToIndex; // Address is key, index is value

	bool& running;

	int window_flags;
	int table_flags;
	int column_flags;

	bool addBreakpointPopup;

	uint16_t address_index;
	bool focus_address;

	int selected;

private:
	enum class ColumnID
	{
		Empty,
		Address,
		Opcode,
		Mnemonic,
		Operand,
		Comment
	};
};