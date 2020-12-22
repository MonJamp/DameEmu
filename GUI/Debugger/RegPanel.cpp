#include "RegPanel.h"


RegPanel::RegPanel(wxWindow* parent) : wxPanel(parent)
{
	pc_label.reset(new CpuRegLabel(this, "PC"));
	sp_label.reset(new CpuRegLabel(this, "SP"));
	af_label.reset(new CpuRegLabel(this, "AF"));
	bc_label.reset(new CpuRegLabel(this, "BC"));
	de_label.reset(new CpuRegLabel(this, "DE"));
	hl_label.reset(new CpuRegLabel(this, "HL"));

	
	wxGridSizer* cpu_grid = new wxGridSizer(6, 2, 0, 0);
	cpu_grid->Add(pc_label->GetNameLabel()); cpu_grid->Add(pc_label->GetValueLabel());
	cpu_grid->Add(sp_label->GetNameLabel()); cpu_grid->Add(sp_label->GetValueLabel());
	cpu_grid->Add(af_label->GetNameLabel()); cpu_grid->Add(af_label->GetValueLabel());
	cpu_grid->Add(bc_label->GetNameLabel()); cpu_grid->Add(bc_label->GetValueLabel());
	cpu_grid->Add(de_label->GetNameLabel()); cpu_grid->Add(de_label->GetValueLabel());
	cpu_grid->Add(hl_label->GetNameLabel()); cpu_grid->Add(hl_label->GetValueLabel());

	wxStaticBoxSizer* cpu_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "CPU");
	cpu_box->Add(cpu_grid, wxSizerFlags(1).Border(wxALL, 5));
	


	p1_label.reset(new MemRegLabel(this, "P1"));
	sb_label.reset(new MemRegLabel(this, "SB"));
	sc_label.reset(new MemRegLabel(this, "SC"));
	div_label.reset(new MemRegLabel(this, "DIV"));
	tima_label.reset(new MemRegLabel(this, "TIMA"));
	tma_label.reset(new MemRegLabel(this, "TMA"));
	tac_label.reset(new MemRegLabel(this, "TAC"));
	nr10_label.reset(new MemRegLabel(this, "NR10"));
	nr11_label.reset(new MemRegLabel(this, "NR11"));
	nr12_label.reset(new MemRegLabel(this, "NR12"));
	nr13_label.reset(new MemRegLabel(this, "NR13"));
	nr14_label.reset(new MemRegLabel(this, "NR14"));
	nr21_label.reset(new MemRegLabel(this, "NR21"));
	nr22_label.reset(new MemRegLabel(this, "NR22"));
	nr23_label.reset(new MemRegLabel(this, "NR23"));
	nr24_label.reset(new MemRegLabel(this, "NR24"));
	nr30_label.reset(new MemRegLabel(this, "NR30"));
	nr31_label.reset(new MemRegLabel(this, "NR31"));
	nr32_label.reset(new MemRegLabel(this, "NR32"));
	nr33_label.reset(new MemRegLabel(this, "NR33"));
	nr34_label.reset(new MemRegLabel(this, "NR34"));
	nr41_label.reset(new MemRegLabel(this, "NR41"));
	nr42_label.reset(new MemRegLabel(this, "NR42"));
	nr43_label.reset(new MemRegLabel(this, "NR43"));
	nr44_label.reset(new MemRegLabel(this, "NR44"));
	nr50_label.reset(new MemRegLabel(this, "NR50"));
	nr51_label.reset(new MemRegLabel(this, "NR51"));
	nr52_label.reset(new MemRegLabel(this, "NR52"));
	lcdc_label.reset(new MemRegLabel(this, "LCDC"));
	stat_label.reset(new MemRegLabel(this, "STAT"));
	scy_label.reset(new MemRegLabel(this, "SCY"));
	scx_label.reset(new MemRegLabel(this, "SCX"));
	ly_label.reset(new MemRegLabel(this, "LY"));
	lyc_label.reset(new MemRegLabel(this, "LYC"));
	dma_label.reset(new MemRegLabel(this, "DMA"));
	bgp_label.reset(new MemRegLabel(this, "BGP"));
	obp0_label.reset(new MemRegLabel(this, "OBP0"));
	obp1_label.reset(new MemRegLabel(this, "OBP1"));
	wy_label.reset(new MemRegLabel(this, "WY"));
	wx_label.reset(new MemRegLabel(this, "WX"));
	int_req_label.reset(new MemRegLabel(this, "IF"));
	int_ie_label.reset(new MemRegLabel(this, "IE"));


	wxGridSizer* mem_grid1 = new wxGridSizer(2, 5, 10);
	mem_grid1->Add(p1_label->GetNameLabel());		mem_grid1->Add(p1_label->GetValueLabel());
	mem_grid1->Add(sb_label->GetNameLabel());		mem_grid1->Add(sb_label->GetValueLabel());
	mem_grid1->Add(sc_label->GetNameLabel());		mem_grid1->Add(sc_label->GetValueLabel());
	mem_grid1->Add(div_label->GetNameLabel());		mem_grid1->Add(div_label->GetValueLabel());
	mem_grid1->Add(tima_label->GetNameLabel());		mem_grid1->Add(tima_label->GetValueLabel());
	mem_grid1->Add(tma_label->GetNameLabel());		mem_grid1->Add(tma_label->GetValueLabel());
	mem_grid1->Add(tac_label->GetNameLabel());		mem_grid1->Add(tac_label->GetValueLabel());
	mem_grid1->Add(nr10_label->GetNameLabel());		mem_grid1->Add(nr10_label->GetValueLabel());
	mem_grid1->Add(nr11_label->GetNameLabel());		mem_grid1->Add(nr11_label->GetValueLabel());
	mem_grid1->Add(nr12_label->GetNameLabel());		mem_grid1->Add(nr12_label->GetValueLabel());
	mem_grid1->Add(nr13_label->GetNameLabel());		mem_grid1->Add(nr13_label->GetValueLabel());
	mem_grid1->Add(nr14_label->GetNameLabel());		mem_grid1->Add(nr14_label->GetValueLabel());
	mem_grid1->Add(nr21_label->GetNameLabel());		mem_grid1->Add(nr21_label->GetValueLabel());
	mem_grid1->Add(nr22_label->GetNameLabel());		mem_grid1->Add(nr22_label->GetValueLabel());
	mem_grid1->Add(nr23_label->GetNameLabel());		mem_grid1->Add(nr23_label->GetValueLabel());
	mem_grid1->Add(nr24_label->GetNameLabel());		mem_grid1->Add(nr24_label->GetValueLabel());
	mem_grid1->Add(nr30_label->GetNameLabel());		mem_grid1->Add(nr30_label->GetValueLabel());
	mem_grid1->Add(nr31_label->GetNameLabel());		mem_grid1->Add(nr31_label->GetValueLabel());
	mem_grid1->Add(nr32_label->GetNameLabel());		mem_grid1->Add(nr32_label->GetValueLabel());
	mem_grid1->Add(nr33_label->GetNameLabel());		mem_grid1->Add(nr33_label->GetValueLabel());
	mem_grid1->Add(nr34_label->GetNameLabel());		mem_grid1->Add(nr34_label->GetValueLabel());

	wxGridSizer* mem_grid2 = new wxGridSizer(2, 5, 10);
	mem_grid2->Add(nr41_label->GetNameLabel());		mem_grid2->Add(nr41_label->GetValueLabel());
	mem_grid2->Add(nr42_label->GetNameLabel());		mem_grid2->Add(nr42_label->GetValueLabel());
	mem_grid2->Add(nr43_label->GetNameLabel());		mem_grid2->Add(nr43_label->GetValueLabel());
	mem_grid2->Add(nr44_label->GetNameLabel());		mem_grid2->Add(nr44_label->GetValueLabel());
	mem_grid2->Add(nr50_label->GetNameLabel());		mem_grid2->Add(nr50_label->GetValueLabel());
	mem_grid2->Add(nr51_label->GetNameLabel());		mem_grid2->Add(nr51_label->GetValueLabel());
	mem_grid2->Add(nr52_label->GetNameLabel());		mem_grid2->Add(nr52_label->GetValueLabel());
	mem_grid2->Add(lcdc_label->GetNameLabel());		mem_grid2->Add(lcdc_label->GetValueLabel());
	mem_grid2->Add(stat_label->GetNameLabel());		mem_grid2->Add(stat_label->GetValueLabel());
	mem_grid2->Add(scy_label->GetNameLabel());		mem_grid2->Add(scy_label->GetValueLabel());
	mem_grid2->Add(scx_label->GetNameLabel());		mem_grid2->Add(scx_label->GetValueLabel());
	mem_grid2->Add(ly_label->GetNameLabel());		mem_grid2->Add(ly_label->GetValueLabel());
	mem_grid2->Add(lyc_label->GetNameLabel());		mem_grid2->Add(lyc_label->GetValueLabel());
	mem_grid2->Add(dma_label->GetNameLabel());		mem_grid2->Add(dma_label->GetValueLabel());
	mem_grid2->Add(bgp_label->GetNameLabel());		mem_grid2->Add(bgp_label->GetValueLabel());
	mem_grid2->Add(obp0_label->GetNameLabel());		mem_grid2->Add(obp0_label->GetValueLabel());
	mem_grid2->Add(obp1_label->GetNameLabel());		mem_grid2->Add(obp1_label->GetValueLabel());
	mem_grid2->Add(wy_label->GetNameLabel());		mem_grid2->Add(wy_label->GetValueLabel());
	mem_grid2->Add(wx_label->GetNameLabel());		mem_grid2->Add(wx_label->GetValueLabel());
	mem_grid2->Add(int_req_label->GetNameLabel());	mem_grid2->Add(int_req_label->GetValueLabel());
	mem_grid2->Add(int_ie_label->GetNameLabel());	mem_grid2->Add(int_ie_label->GetValueLabel());

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
	hbox->Add(mem_grid1);

	wxStaticLine* vline = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	vline->SetBackgroundColour(*wxBLACK);

	hbox->Add(vline, wxSizerFlags().Expand().Border(wxALL, 5));
	hbox->Add(mem_grid2);

	wxStaticBoxSizer* mem_box = new wxStaticBoxSizer(wxHORIZONTAL, this, "Memory");
	mem_box->Add(hbox, wxSizerFlags(1).Border(wxALL, 5));

	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	vbox->Add(cpu_box);
	vbox->Add(mem_box);
	SetSizer(vbox);
}

RegPanel::~RegPanel()
{

}

void RegPanel::UpdateValues(Debugger::RegState regs)
{
	pc_label->UpdateValue(regs.cpu.pc);
	sp_label->UpdateValue(regs.cpu.sp);
	af_label->UpdateValue(regs.cpu.af);
	bc_label->UpdateValue(regs.cpu.bc);
	de_label->UpdateValue(regs.cpu.de);
	hl_label->UpdateValue(regs.cpu.hl);

	p1_label->UpdateValue(regs.mem.p1);
	sb_label->UpdateValue(regs.mem.sb);
	sc_label->UpdateValue(regs.mem.sc);
	div_label->UpdateValue(regs.mem.div);
	tima_label->UpdateValue(regs.mem.tima);
	tma_label->UpdateValue(regs.mem.tma);
	tac_label->UpdateValue(regs.mem.tac);
	nr10_label->UpdateValue(regs.mem.nr10);
	nr11_label->UpdateValue(regs.mem.nr11);
	nr12_label->UpdateValue(regs.mem.nr12);
	nr13_label->UpdateValue(regs.mem.nr13);
	nr14_label->UpdateValue(regs.mem.nr14);
	nr21_label->UpdateValue(regs.mem.nr21);
	nr22_label->UpdateValue(regs.mem.nr22);
	nr23_label->UpdateValue(regs.mem.nr23);
	nr24_label->UpdateValue(regs.mem.nr24);
	nr30_label->UpdateValue(regs.mem.nr30);
	nr31_label->UpdateValue(regs.mem.nr31);
	nr32_label->UpdateValue(regs.mem.nr32);
	nr33_label->UpdateValue(regs.mem.nr33);
	nr34_label->UpdateValue(regs.mem.nr34);
	nr41_label->UpdateValue(regs.mem.nr41);
	nr42_label->UpdateValue(regs.mem.nr42);
	nr43_label->UpdateValue(regs.mem.nr43);
	nr44_label->UpdateValue(regs.mem.nr44);
	nr50_label->UpdateValue(regs.mem.nr50);
	nr51_label->UpdateValue(regs.mem.nr51);
	nr52_label->UpdateValue(regs.mem.nr52);
	lcdc_label->UpdateValue(regs.mem.lcdc);
	stat_label->UpdateValue(regs.mem.stat);
	scy_label->UpdateValue(regs.mem.scy);
	scx_label->UpdateValue(regs.mem.scx);
	ly_label->UpdateValue(regs.mem.ly);
	lyc_label->UpdateValue(regs.mem.lyc);
	dma_label->UpdateValue(regs.mem.dma);
	bgp_label->UpdateValue(regs.mem.bgp);
	obp0_label->UpdateValue(regs.mem.obp0);
	obp1_label->UpdateValue(regs.mem.obp1);
	wy_label->UpdateValue(regs.mem.wy);
	wx_label->UpdateValue(regs.mem.wx);
	int_req_label->UpdateValue(regs.mem.int_req);
	int_ie_label->UpdateValue(regs.mem.int_ie);
}