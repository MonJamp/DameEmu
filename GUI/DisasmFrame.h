#pragma once
#include "../Disassembler.h"
#include "DisasmListCtrl.h"
#include <wx/wx.h>

#ifdef __GNUG__
#include <memory>
#endif


class RegPanel : public wxPanel
{
public:
	RegPanel(wxWindow* parent) : wxPanel(parent)
	{
		wxStaticBoxSizer* sbox = new wxStaticBoxSizer(wxHORIZONTAL, this);
		wxGridSizer* grid = new wxGridSizer(6, 2, 0, 0);

		pcText = new wxStaticText(this, wxID_ANY, "PC: ");
		spText = new wxStaticText(this, wxID_ANY, "SP: ");
		afText = new wxStaticText(this, wxID_ANY, "AF: ");
		bcText = new wxStaticText(this, wxID_ANY, "BC: ");
		deText = new wxStaticText(this, wxID_ANY, "DE: ");
		hlText = new wxStaticText(this, wxID_ANY, "HL: ");

		pc_value = new wxStaticText(this, wxID_ANY, "0xFFFF");
		sp_value = new wxStaticText(this, wxID_ANY, "0xFFFF");
		af_value = new wxStaticText(this, wxID_ANY, "0xFFFF");
		bc_value = new wxStaticText(this, wxID_ANY, "0xFFFF");
		de_value = new wxStaticText(this, wxID_ANY, "0xFFFF");
		hl_value = new wxStaticText(this, wxID_ANY, "0xFFFF");


		grid->Add(pcText); grid->Add(pc_value);
		grid->Add(spText); grid->Add(sp_value);
		grid->Add(afText); grid->Add(af_value);
		grid->Add(bcText); grid->Add(bc_value);
		grid->Add(deText); grid->Add(de_value);
		grid->Add(hlText); grid->Add(hl_value);

		sbox->Add(grid, wxSizerFlags(1).Border(wxALL, 10));
		SetSizer(sbox);
	}

	void UpdateValues(Debugger::CpuState cpuState)
	{
		pc_value->SetLabel(intToHexString(cpuState.pc, 2, "0x"));
		sp_value->SetLabel(intToHexString(cpuState.sp, 2, "0x"));
		af_value->SetLabel(intToHexString(cpuState.af, 2, "0x"));
		bc_value->SetLabel(intToHexString(cpuState.bc, 2, "0x"));
		de_value->SetLabel(intToHexString(cpuState.de, 2, "0x"));
		hl_value->SetLabel(intToHexString(cpuState.hl, 2, "0x"));
	}

private:
	wxStaticText* pcText;
	wxStaticText* spText;
	wxStaticText* afText;
	wxStaticText* bcText;
	wxStaticText* deText;
	wxStaticText* hlText;

	wxStaticText* pc_value;
	wxStaticText* sp_value;
	wxStaticText* af_value;
	wxStaticText* bc_value;
	wxStaticText* de_value;
	wxStaticText* hl_value;
};

enum ButtonID
{
	Step
};

class ButtonPanel : public wxPanel
{
public:
	ButtonPanel(wxWindow* parent) : wxPanel(parent)
	{
		wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

		// TODO Add buttons for continue, pause, step in, step out, step over
		wxButton* stepBtn = new wxButton(this, ButtonID::Step, "Step");
		hbox->Add(stepBtn, wxSizerFlags(0).Align(wxLEFT));

		SetSizer(hbox);
	}

private:
};

class DisasmFrame : public wxFrame
{
public:
	DisasmFrame(std::shared_ptr<Debugger>& d, wxWindow* parent);
	~DisasmFrame();

private:
	void InitMenuBar();
	void InitWidgets();

	void OnStep(wxCommandEvent& evt);
	void OnExit(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();

	DisasmListCtrl* disasmList;
	RegPanel* regPanel;
	ButtonPanel* btnPanel;

	std::shared_ptr<Debugger> debugger;
};

