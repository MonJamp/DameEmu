#pragma once
#include "../../Disassembler.h"
#include <wx/wx.h>
#include <vector>
#include <memory>

class RegPanel : public wxPanel
{
public:
	RegPanel(wxWindow* parent);
	~RegPanel();
	void UpdateValues(Debugger::RegState regs);

private:
	class RegLabel
	{
	public:
		RegLabel()
		{
			name_label = nullptr;
			value_label = nullptr;
		}

		~RegLabel()
		{
			delete name_label;
			delete value_label;
		}

		wxStaticText* GetNameLabel() { return name_label; }
		wxStaticText* GetValueLabel() { return value_label; }

		virtual void UpdateValues() {}

	protected:
		wxStaticText* name_label;
		wxStaticText* value_label;
	};

	class CpuRegLabel : public RegLabel
	{
	public:
		CpuRegLabel(wxWindow* parent, const wxString& name, uint16_t value = 0xFFFF)
		{
			this->value = value;
			name_label = new wxStaticText(parent, wxID_ANY, name);
			value_label = new wxStaticText(parent, wxID_ANY, intToHexString(value, 2, "0x"));
		}

		void UpdateValue(uint16_t new_value)
		{
			this->value = new_value;
			value_label->SetLabel(intToHexString(value, 2, "0x"));
		}

	private:
		uint16_t value;
	};

	class MemRegLabel : public RegLabel
	{
	public:
		MemRegLabel(wxWindow* parent, const wxString& name, uint8_t value = 0xFF)
		{
			this->value = value;
			name_label = new wxStaticText(parent, wxID_ANY, name);
			value_label = new wxStaticText(parent, wxID_ANY, intToHexString(value, 1, "0x"));
		}

		void UpdateValue(uint8_t new_value)
		{
			this->value = new_value;
			value_label->SetLabel(intToHexString(value, 1, "0x"));
		}

	private:
		uint8_t value;
	};

private:
	std::unique_ptr<CpuRegLabel> pc_label;
	std::unique_ptr<CpuRegLabel> sp_label;
	std::unique_ptr<CpuRegLabel> af_label;
	std::unique_ptr<CpuRegLabel> bc_label;
	std::unique_ptr<CpuRegLabel> de_label;
	std::unique_ptr<CpuRegLabel> hl_label;
	
	std::unique_ptr<MemRegLabel> p1_label;
	std::unique_ptr<MemRegLabel> sb_label;
	std::unique_ptr<MemRegLabel> sc_label;
	std::unique_ptr<MemRegLabel> div_label;
	std::unique_ptr<MemRegLabel> tima_label;
	std::unique_ptr<MemRegLabel> tma_label;
	std::unique_ptr<MemRegLabel> tac_label;
	std::unique_ptr<MemRegLabel> nr10_label;
	std::unique_ptr<MemRegLabel> nr11_label;
	std::unique_ptr<MemRegLabel> nr12_label;
	std::unique_ptr<MemRegLabel> nr13_label;
	std::unique_ptr<MemRegLabel> nr14_label;
	std::unique_ptr<MemRegLabel> nr21_label;
	std::unique_ptr<MemRegLabel> nr22_label;
	std::unique_ptr<MemRegLabel> nr23_label;
	std::unique_ptr<MemRegLabel> nr24_label;
	std::unique_ptr<MemRegLabel> nr30_label;
	std::unique_ptr<MemRegLabel> nr31_label;
	std::unique_ptr<MemRegLabel> nr32_label;
	std::unique_ptr<MemRegLabel> nr33_label;
	std::unique_ptr<MemRegLabel> nr34_label;
	std::unique_ptr<MemRegLabel> nr41_label;
	std::unique_ptr<MemRegLabel> nr42_label;
	std::unique_ptr<MemRegLabel> nr43_label;
	std::unique_ptr<MemRegLabel> nr44_label;
	std::unique_ptr<MemRegLabel> nr50_label;
	std::unique_ptr<MemRegLabel> nr51_label;
	std::unique_ptr<MemRegLabel> nr52_label;
	std::unique_ptr<MemRegLabel> lcdc_label;
	std::unique_ptr<MemRegLabel> stat_label;
	std::unique_ptr<MemRegLabel> scy_label;
	std::unique_ptr<MemRegLabel> scx_label;
	std::unique_ptr<MemRegLabel> ly_label;
	std::unique_ptr<MemRegLabel> lyc_label;
	std::unique_ptr<MemRegLabel> dma_label;
	std::unique_ptr<MemRegLabel> bgp_label;
	std::unique_ptr<MemRegLabel> obp0_label;
	std::unique_ptr<MemRegLabel> obp1_label;
	std::unique_ptr<MemRegLabel> wy_label;
	std::unique_ptr<MemRegLabel> wx_label;
	std::unique_ptr<MemRegLabel> int_req_label;
	std::unique_ptr<MemRegLabel> int_ie_label;
};