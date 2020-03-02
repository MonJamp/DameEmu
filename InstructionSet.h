#pragma once
#include <cstdint>
#include <string>
#include <vector>


#define _o_t(x)		Operand::Type::x
#define _o_or(o)	Operand::OffsetReg::o
#define _o(x)		Operand(_o_t(x))
#define _oa(x)		Operand(_o_t(x), Operand::Mode::Address)
#define _oii(x)		Operand(_o_t(x), Operand::Mode::AutoInc)
#define _oid(x)		Operand(_o_t(x), Operand::Mode::AutoDec)
#define _oo(x, o)	Operand(_o_t(x), o, Operand::Mode::Offset)
#define _oio(x, o)	Operand(_o_t(x), _o_or(o), Operand::Mode::IndexOffset)


struct Operand
{
public:
	// TODO: Investigate type s8
	enum class Type
	{
		NONE,
		A, B, C, D, E, H, L,
		AF, BC, DE, HL, SP, PC,
		s8, u8, u16
	};

	enum class OffsetReg
	{
		NONE,
		SP, PC
	};

	enum class Mode
	{
		Immediate,	// Operand data is immediate register or const
		Address,	// Operand is a pointer
		AutoInc,	// Operand is a pointer which gets incremented after
		AutoDec,	// Operand is a pointer which gets decremented after
		Offset,		// Operand is an offset to an address
		IndexOffset	// Operand is an offset to an address stored in a register
	};

	static std::string typeToString(Type t)
	{
		switch (t)
		{
		case Type::A:	return "a";
		case Type::B:	return "b";
		case Type::C:	return "c";
		case Type::D:	return "d";
		case Type::E:	return "e";
		case Type::H:	return "h";
		case Type::L:	return "l";
		case Type::AF:	return "af";
		case Type::BC:	return "bc";
		case Type::DE:	return "de";
		case Type::HL:	return "hl";
		case Type::SP:	return "sp";
		case Type::s8:	return "s8";
		case Type::u8:	return "u8";
		case Type::u16:	return "u16";
		default:		return "n/a";
		}
	}

	// Return how many bytes the type of operand is
	static uint8_t bytesOfType(Type t)
	{
		switch (t)
		{
		case Type::A:	return 1;
		case Type::B:	return 1;
		case Type::C:	return 1;
		case Type::D:	return 1;
		case Type::E:	return 1;
		case Type::H:	return 1;
		case Type::L:	return 1;
		case Type::AF:	return 2;
		case Type::BC:	return 2;
		case Type::DE:	return 2;
		case Type::HL:	return 2;
		case Type::SP:	return 2;
		case Type::s8:	return 1;
		case Type::u8:	return 1;
		case Type::u16:	return 2;
		default:		return 0;
		}
	}

	static std::string typeToString(OffsetReg o)
	{
		switch (o) {
		case OffsetReg::PC: return "pc";
		case OffsetReg::SP: return "sp";
		default:			return "n/a";
		}
	}

public:
	Operand(Type t = Type::NONE, Mode m = Mode::Immediate)
		: type(t), mode(m) { }

	Operand(Type t, uint16_t off, Mode m = Mode::Offset)
		: offset(off), type(t), mode(m) { }

	Operand(Type t, OffsetReg off, Mode m = Mode::IndexOffset)
		: offset_reg(off), type(t), mode(m) { }

	bool isTypeRegister()
	{
		switch (type) {
		case Type::NONE:
		case Type::s8:
		case Type::u8:
		case Type::u16:
			return false;
		default:
			return true;
		}
	}

	// Get size of operand in bytes
	uint8_t size()
	{
		return bytesOfType(type);
	}

public:
	Type type;
	Mode mode;
	int32_t value = 0;
	uint16_t offset = 0x0000;
	OffsetReg offset_reg = OffsetReg::NONE;
};

struct Instruction {
public:
	Instruction()
	{
		this->mnemonic = "N/A; this instruction obj was created using default cstor";
	}

	Instruction(std::string mnemonic, Operand o1 = Operand(), Operand o2 = Operand())
	{
		this->mnemonic = mnemonic;

		if (o1.type != Operand::Type::NONE)
		{
			operands.push_back(o1);
		}
		
		if (o2.type != Operand::Type::NONE)
		{
			operands.push_back(o2);
		}
	}

	// For CB instructions
	Instruction(std::string mnemonic, bool is_prefix)
	{
		this->mnemonic = mnemonic;
		this->is_prefix = is_prefix;
	}

public:
	std::string getMnemonic()
	{
		return mnemonic;
	}

	uint8_t getNumOfOperands()
	{
		return static_cast<uint8_t>(operands.size());
	}

	uint8_t getLength()
	{
		uint8_t length = 1;

		for (auto i : operands)
		{
			if (!i.isTypeRegister())
			{
				length += i.size();
			}
		}

		prefixed ? length++ : length;
		return  length;
	}

public:
	std::string mnemonic;
	std::vector<Operand> operands;
	// Is this an instruction or a prefix
	bool is_prefix = false;
	// Is instruction prefixed
	bool prefixed = false;
};

struct CB_Instruction : public Instruction {
	CB_Instruction(std::string mnemonic, Operand operand) : Instruction{ mnemonic, operand } {};
	bool prefixed = true;
};

static Instruction insTable[256] = {
	{"nop"},									//00
	{"ld", _o(BC), _o(u16)},					//01
	{"ld", _oa(BC), _o(A)},						//02
	{"inc", _o(BC)},							//03
	{"inc", _o(B)},								//04
	{"dec", _o(B)},								//05
	{"ld", _o(B), _o(u8)},						//06
	{"rlca"},									//07		
	{"ld", _oa(u16), _o(SP)},					//08
	{"add", _o(HL), _o(BC)},					//09
	{"ld", _o(A), _oa(BC)},						//0A
	{"dec", _o(BC)},							//0B
	{"inc", _o(C)},								//0C
	{"dec", _o(C)},								//0D
	{"ld", _o(C), _o(u8)},						//0E
	{"rrca"},									//0F
	{"stop"},									//10
	{"ld", _o(DE), _o(u16)},					//11
	{"ld", _oa(DE), _o(A)},						//12
	{"inc", _o(DE)},							//13
	{"inc", _o(D)},								//14
	{"dec", _o(D)},								//15
	{"ld", _o(D), _o(u8)},						//16
	{"rla"},									//17
	{"jr", _oio(s8, PC)},						//18
	{"add", _o(HL), _o(DE)},					//19
	{"ld", _o(A), _oa(DE)},						//1A
	{"dec", _o(DE)},							//1B
	{"inc", _o(E)},								//1C
	{"dec", _o(E)},								//1D
	{"ld", _o(E), _o(u8)},						//1E
	{"rra"},									//1F
	{"jr nz", _oio(s8, PC)},					//20
	{"ld", _o(HL), _o(u16)},					//21
	{"ldi", _oii(HL), _o(A)},					//22
	{"inc", _o(HL)},							//23
	{"inc", _o(H)},								//24
	{"dec", _o(H)},								//25
	{"ld", _o(H), _o(u8)},						//26
	{"daa"},									//27
	{"jr z", _oio(s8, PC)},						//28
	{"add", _o(HL), _o(HL)},					//29
	{"ldi", _o(A), _oii(HL)},					//2A
	{"dec", _o(HL)},							//2B
	{"inc", _o(L)},								//2C
	{"dec", _o(L)},								//2D
	{"ld", _o(L), _o(u8)},						//2E
	{"cpl"},									//2F
	{"jr nc", _oio(s8, PC)},					//30
	{"ld", _o(SP), _o(u16)},					//31
	{"ldd", _oid(HL), _o(A)},					//32
	{"inc", _o(SP)},							//33
	{"inc", _oa(HL)},							//34
	{"dec", _oa(HL)},							//35
	{"ld", _oa(HL), _o(u8)},					//36
	{"scf"},									//37
	{"jr c", _oio(s8, PC)},						//38
	{"add", _o(HL), _o(SP)},					//39
	{"ldd", _o(A), _oid(HL)},					//3A
	{"dec", _o(SP)},							//3B
	{"inc", _o(A)},								//3C
	{"dec", _o(A)},								//3D
	{"ld", _o(A), _o(u8)},						//3E
	{"ccf"},									//3F
	{"ld", _o(B), _o(B)},						//40
	{"ld", _o(B), _o(C)},						//41
	{"ld", _o(B), _o(D)},						//42
	{"ld", _o(B), _o(E)},						//43
	{"ld", _o(B), _o(H)},						//44
	{"ld", _o(B), _o(L)},						//45
	{"ld", _o(B), _oa(HL)},						//46
	{"ld", _o(B), _o(A)},						//47
	{"ld", _o(C), _o(B)},						//48
	{"ld", _o(C), _o(C)},						//49
	{"ld", _o(C), _o(D)},						//4A
	{"ld", _o(C), _o(E)},						//4B
	{"ld", _o(C), _o(H)},						//4C
	{"ld", _o(C), _o(L)},						//4D
	{"ld", _o(C), _oa(HL)},						//4E
	{"ld", _o(C), _o(A)},						//4F
	{"ld", _o(D), _o(B)},						//50
	{"ld", _o(D), _o(C)},						//51
	{"ld", _o(D), _o(D)},						//52
	{"ld", _o(D), _o(E)},						//53
	{"ld", _o(D), _o(H)},						//54
	{"ld", _o(D), _o(L)},						//55
	{"ld", _o(D), _oa(HL)},						//56
	{"ld", _o(D), _o(A)},						//57
	{"ld", _o(E), _o(B)},						//58
	{"ld", _o(E), _o(C)},						//59
	{"ld", _o(E), _o(D)},						//5A
	{"ld", _o(E), _o(E)},						//5B
	{"ld", _o(E), _o(H)},						//5C
	{"ld", _o(E), _o(L)},						//5D
	{"ld", _o(E), _oa(HL)},						//5E
	{"ld", _o(E), _o(A)},						//5F
	{"ld", _o(H), _o(B)},						//60
	{"ld", _o(H), _o(C)},						//61
	{"ld", _o(H), _o(D)},						//62
	{"ld", _o(H), _o(E)},						//63
	{"ld", _o(H), _o(H)},						//64
	{"ld", _o(H), _o(L)},						//65
	{"ld", _o(H), _oa(HL)},						//66
	{"ld", _o(H), _o(A)},						//67
	{"ld", _o(L), _o(B)},						//68
	{"ld", _o(L), _o(C)},						//69
	{"ld", _o(L), _o(D)},						//6A
	{"ld", _o(L), _o(E)},						//6B
	{"ld", _o(L), _o(H)},						//6C
	{"ld", _o(L), _o(L)},						//6D
	{"ld", _o(L), _oa(HL)},						//6E
	{"ld", _o(L), _o(A)},						//6F
	{"ld", _oa(HL), _o(B)},						//70
	{"ld", _oa(HL), _o(C)},						//71
	{"ld", _oa(HL), _o(D)},						//72
	{"ld", _oa(HL), _o(E)},						//73
	{"ld", _oa(HL), _o(H)},						//74
	{"ld", _oa(HL), _o(L)},						//75
	{"halt"},									//76
	{"ld", _oa(HL), _o(A)},						//77
	{"ld", _o(A), _o(B)},						//78
	{"ld", _o(A), _o(C)},						//79
	{"ld", _o(A), _o(D)},						//7A
	{"ld", _o(A), _o(E)},						//7B
	{"ld", _o(A), _o(H)},						//7C
	{"ld", _o(A), _o(L)},						//7D
	{"ld", _o(A), _oa(HL)},						//7E
	{"ld", _o(A), _o(A)},						//7F
	{"add", _o(B)},								//80
	{"add", _o(C)},								//81
	{"add", _o(D)},								//82
	{"add", _o(E)},								//83
	{"add", _o(H)},								//84
	{"add", _o(L)},								//85
	{"add", _oa(HL)},							//86
	{"add", _o(A)},								//87
	{"adc", _o(B)},								//88
	{"adc", _o(C)},								//89
	{"adc", _o(D)},								//8A
	{"adc", _o(E)},								//8B
	{"adc", _o(H)},								//8C
	{"adc", _o(L)},								//8D
	{"adc", _oa(HL)},							//8E
	{"adc", _o(A)},								//8F
	{"sub", _o(B)},								//90
	{"sub", _o(C)},								//91
	{"sub", _o(D)},								//92
	{"sub", _o(E)},								//93
	{"sub", _o(H)},								//94
	{"sub", _o(L)},								//95
	{"sub", _oa(HL)},							//96
	{"sub", _o(A)},								//97
	{"sbc", _o(B)},								//98
	{"sbc", _o(C)},								//99
	{"sbc", _o(D)},								//9A
	{"sbc", _o(E)},								//9B
	{"sbc", _o(H)},								//9C
	{"sbc", _o(L)},								//9D
	{"sbc", _oa(HL)},							//9E
	{"sbc", _o(A)},								//9F
	{"and", _o(B)},								//A0
	{"and", _o(C)},								//A1
	{"and", _o(D)},								//A2
	{"and", _o(E)},								//A3
	{"and", _o(H)},								//A4
	{"and", _o(L)},								//A5
	{"and", _oa(HL)},							//A6
	{"and", _o(A)},								//A7
	{"xor", _o(B)},								//A8
	{"xor", _o(C)},								//A9
	{"xor", _o(D)},								//AA
	{"xor", _o(E)},								//AB
	{"xor", _o(H)},								//AC
	{"xor", _o(L)},								//AD
	{"xor", _oa(HL)},							//AE
	{"xor", _o(A)},								//AF
	{"or", _o(B)},								//B0
	{"or", _o(C)},								//B1
	{"or", _o(D)},								//B2
	{"or", _o(E)},								//B3
	{"or", _o(H)},								//B4
	{"or", _o(L)},								//B5
	{"or", _oa(HL)},							//B6
	{"or", _o(A)},								//B7
	{"cp", _o(B)},								//B8
	{"cp", _o(C)},								//B9
	{"cp", _o(D)},								//BA
	{"cp", _o(E)},								//BB
	{"cp", _o(H)},								//BC
	{"cp", _o(L)},								//BD
	{"cp", _oa(HL)},							//BE
	{"cp", _o(A)},								//BF
	{"ret nz"},									//C0
	{"pop", _o(BC)},							//C1
	{"jp nz", _o(u16)},							//C2
	{"jp", _o(u16)},							//C3
	{"call nz", _o(u16)},						//C4
	{"push", _o(BC)},							//C5
	{"add", _o(u8)},							//C6
	{"rst 0x00"},								//C7
	{"ret z"},									//C8
	{"ret"},									//C9
	{"jp z", _o(u16)},							//CA
	{"cb", true},								//CB
	{"call z", _o(u16)},						//CC
	{"call", _o(u16)},							//CD
	{"adc", _o(u8)},							//CE
	{"rst 0x08"},								//CF
	{"ret nc"},									//D0
	{"pop", _o(DE)},							//D1
	{"jp nc", _o(u16)},							//D2
	{"Undefined OP"},							//D3
	{"call nc", _o(u16)},						//D4
	{"push", _o(DE)},							//D5
	{"sub", _o(u8)},							//D6
	{"rst 0x10"},								//D7
	{"ret c"},									//D8
	{"reti"},									//D9
	{"jp c", _o(u16)},							//DA
	{"Undefined OP"},							//DB
	{"call c", _o(u16)},						//DC
	{"Undefined OP"},							//DD
	{"sbc", _o(u8)},							//DE
	{"rst 0x18"},								//DF
	{"ldh", _oo(u8, 0xFF00), _o(A)},			//E0
	{"pop", _o(HL)},							//E1
	{"ldh", _oo(C, 0xFF00), _o(A)},				//E2
	{"Undefined OP"},							//E3
	{"Undefined OP"},							//E4
	{"push", _o(HL)},							//E5
	{"and", _o(u8)},							//E6
	{"rst 0x20"},								//E7
	{"add", _o(SP), _o(s8)},					//E8
	{"jp", _o(HL)},								//E9
	{"ld", _oa(u16), _o(A)},					//EA
	{"Undefined OP"},							//EB
	{"Undefined OP"},							//EC
	{"Undefined OP"},							//ED
	{"xor", _o(u8)},							//EE
	{"rst 0x28"},								//EF
	{"ldh", _o(A), _oo(u8, 0xFF00)},			//F0
	{"pop", _o(AF)},							//F1
	{"ldh", _o(A), _oo(C, 0xFF00)},				//F2
	{"di"},										//F3
	{"Undefined OP"},							//F4
	{"push", _o(AF)},							//F5
	{"or", _o(u8)},								//F6
	{"rst 0x30"},								//F7
	{"ldhl", _o(HL), _oio(s8, SP)},				//F8
	{"ld", _o(SP), _o(HL)},						//F9
	{"ld", _o(A), _oa(u16)},					//FA
	{"ei"},										//FB
	{"Undefined OP"},							//FC
	{"Undefined OP"},							//FD
	{"cp", _o(u8)},								//FE
	{"rst 0x38"},								//FF
};

static CB_Instruction cb_insTable[256] = {
	{"rlc", _o(B)},			//00
	{"rlc", _o(C)},			//01
	{"rlc", _o(D)},			//02
	{"rlc", _o(E)},			//03
	{"rlc", _o(H)},			//04
	{"rlc", _o(L)},			//05
	{"rlc", _oa(HL)},		//06
	{"rlc", _o(A)},		//07
	{"rrc", _o(B)},			//08
	{"rrc", _o(C)},			//09
	{"rrc", _o(D)},			//0A
	{"rrc", _o(E)},			//0B
	{"rrc", _o(H)},			//0C
	{"rrc", _o(L)},			//0D
	{"rrc", _oa(HL)},		//0E
	{"rrc", _o(A)},		//0F
	{"rl", _o(B)},			//10
	{"rl", _o(C)},			//11
	{"rl", _o(D)},			//12
	{"rl", _o(E)},			//13
	{"rl", _o(H)},			//14
	{"rl", _o(L)},			//15
	{"rl", _oa(HL)},		//16
	{"rl", _o(A)},			//17
	{"rr", _o(B)},			//18
	{"rr", _o(C)},			//19
	{"rr", _o(D)},			//1A
	{"rr", _o(E)},			//1B
	{"rr", _o(H)},			//1C
	{"rr", _o(L)},			//1D
	{"rr", _oa(HL)},		//1E
	{"rr", _o(A)},			//1F
	{"sla", _o(B)},			//20
	{"sla", _o(C)},			//21
	{"sla", _o(D)},			//22
	{"sla", _o(E)},			//23
	{"sla", _o(H)},			//24
	{"sla", _o(L)},			//25
	{"sla", _oa(HL)},		//26
	{"sla", _o(A)},		//27
	{"sra", _o(B)},			//28
	{"sra", _o(C)},			//29
	{"sra", _o(D)},			//2A
	{"sra", _o(E)},			//2B
	{"sra", _o(H)},			//2C
	{"sra", _o(L)},			//2D
	{"sra", _oa(HL)},		//2E
	{"sra", _o(A)},		//2F
	{"swap", _o(B)},		//30
	{"swap", _o(C)},		//31
	{"swap", _o(D)},		//32
	{"swap", _o(E)},		//33
	{"swap", _o(H)},		//34
	{"swap", _o(L)},		//35
	{"swap", _oa(HL)},		//36
	{"swap", _o(A)},		//37
	{"srl", _o(B)},			//38
	{"srl", _o(C)},			//39
	{"srl", _o(D)},			//3A
	{"srl", _o(E)},			//3B
	{"srl", _o(H)},			//3C
	{"srl", _o(L)},			//3D
	{"srl", _oa(HL)},		//3E
	{"srl", _o(A)},		//3F
	{"bit 0", _o(B)},		//40
	{"bit 0", _o(C)},		//41
	{"bit 0", _o(D)},		//42
	{"bit 0", _o(E)},		//43
	{"bit 0", _o(H)},		//44
	{"bit 0", _o(L)},		//45
	{"bit 0", _oa(HL)},		//46
	{"bit 0", _o(A)},		//47
	{"bit 1", _o(B)},		//48
	{"bit 1", _o(C)},		//49
	{"bit 1", _o(D)},		//4A
	{"bit 1", _o(E)},		//4B
	{"bit 1", _o(H)},		//4C
	{"bit 1", _o(L)},		//4D
	{"bit 1", _oa(HL)},		//4E
	{"bit 1", _o(A)},		//4F
	{"bit 2", _o(B)},		//50
	{"bit 2", _o(C)},		//51
	{"bit 2", _o(D)},		//52
	{"bit 2", _o(E)},		//53
	{"bit 2", _o(H)},		//54
	{"bit 2", _o(L)},		//55
	{"bit 2", _oa(HL)},		//56
	{"bit 2", _o(A)},		//57
	{"bit 3", _o(B)},		//58
	{"bit 3", _o(C)},		//59
	{"bit 3", _o(D)},		//5A
	{"bit 3", _o(E)},		//5B
	{"bit 3", _o(H)},		//5C
	{"bit 3", _o(L)},		//5D
	{"bit 3", _oa(HL)},		//5E
	{"bit 3", _o(A)},		//5F
	{"bit 4", _o(B)},		//60
	{"bit 4", _o(C)},		//61
	{"bit 4", _o(D)},		//62
	{"bit 4", _o(E)},		//63
	{"bit 4", _o(H)},		//64
	{"bit 4", _o(L)},		//65
	{"bit 4", _oa(HL)},		//66
	{"bit 4", _o(A)},		//67
	{"bit 5", _o(B)},		//68
	{"bit 5", _o(C)},		//69
	{"bit 5", _o(D)},		//6A
	{"bit 5", _o(E)},		//6B
	{"bit 5", _o(H)},		//6C
	{"bit 5", _o(L)},		//6D
	{"bit 5", _oa(HL)},		//6E
	{"bit 5", _o(A)},		//6F
	{"bit 6", _o(B)},		//70
	{"bit 6", _o(C)},		//71
	{"bit 6", _o(D)},		//72
	{"bit 6", _o(E)},		//73
	{"bit 6", _o(H)},		//74
	{"bit 6", _o(L)},		//75
	{"bit 6", _oa(HL)},		//76
	{"bit 6", _o(A)},		//77
	{"bit 7", _o(B)},		//78
	{"bit 7", _o(C)},		//79
	{"bit 7", _o(D)},		//7A
	{"bit 7", _o(E)},		//7B
	{"bit 7", _o(H)},		//7C
	{"bit 7", _o(L)},		//7D
	{"bit 7", _oa(HL)},		//7E
	{"bit 7", _o(A)},		//7F
	{"res 0", _o(B)},		//80
	{"res 0", _o(C)},		//81
	{"res 0", _o(D)},		//82
	{"res 0", _o(E)},		//83
	{"res 0", _o(H)},		//84
	{"res 0", _o(L)},		//85
	{"res 0", _oa(HL)},	//86
	{"res 0", _o(A)},		//87
	{"res 1", _o(B)},		//88
	{"res 1", _o(C)},		//89
	{"res 1", _o(D)},		//8A
	{"res 1", _o(E)},		//8B
	{"res 1", _o(H)},		//8C
	{"res 1", _o(L)},		//8D
	{"res 1", _oa(HL)},	//8E
	{"res 1", _o(A)},		//8F
	{"res 2", _o(B)},		//90
	{"res 2", _o(C)},		//91
	{"res 2", _o(D)},		//92
	{"res 2", _o(E)},		//93
	{"res 2", _o(H)},		//94
	{"res 2", _o(L)},		//95
	{"res 2", _oa(HL)},	//96
	{"res 2", _o(A)},		//97
	{"res 3", _o(B)},		//98
	{"res 3", _o(C)},		//99
	{"res 3", _o(D)},		//9A
	{"res 3", _o(E)},		//9B
	{"res 3", _o(H)},		//9C
	{"res 3", _o(L)},		//9D
	{"res 3", _oa(HL)},	//9E
	{"res 3", _o(A)},		//9F
	{"res 4", _o(B)},		//A0
	{"res 4", _o(C)},		//A1
	{"res 4", _o(D)},		//A2
	{"res 4", _o(E)},		//A3
	{"res 4", _o(H)},		//A4
	{"res 4", _o(L)},		//A5
	{"res 4", _oa(HL)},	//A6
	{"res 4", _o(A)},		//A7
	{"res 5", _o(B)},		//A8
	{"res 5", _o(C)},		//A9
	{"res 5", _o(D)},		//AA
	{"res 5", _o(E)},		//AB
	{"res 5", _o(H)},		//AC
	{"res 5", _o(L)},		//AD
	{"res 5", _oa(HL)},	//AE
	{"res 5", _o(A)},		//AF
	{"res 6", _o(B)},		//B0
	{"res 6", _o(C)},		//B1
	{"res 6", _o(D)},		//B2
	{"res 6", _o(E)},		//B3
	{"res 6", _o(H)},		//B4
	{"res 6", _o(L)},		//B5
	{"res 6", _oa(HL)},	//B6
	{"res 6", _o(A)},		//B7
	{"res 7", _o(B)},		//B8
	{"res 7", _o(C)},		//B9
	{"res 7", _o(D)},		//BA
	{"res 7", _o(E)},		//BB
	{"res 7", _o(H)},		//BC
	{"res 7", _o(L)},		//BD
	{"res 7", _oa(HL)},	//BE
	{"res 7", _o(A)},		//BF
	{"set 0", _o(B)},		//C0
	{"set 0", _o(C)},		//C1
	{"set 0", _o(D)},		//C2
	{"set 0", _o(E)},		//C3
	{"set 0", _o(H)},		//C4
	{"set 0", _o(L)},		//C5
	{"set 0", _oa(HL)},	//C6
	{"set 0", _o(A)},		//C7
	{"set 1", _o(B)},		//C8
	{"set 1", _o(C)},		//C9
	{"set 1", _o(D)},		//CA
	{"set 1", _o(E)},		//CB
	{"set 1", _o(H)},		//CC
	{"set 1", _o(L)},		//CD
	{"set 1", _oa(HL)},	//CE
	{"set 1", _o(A)},		//CF
	{"set 2", _o(B)},		//D0
	{"set 2", _o(C)},		//D1
	{"set 2", _o(D)},		//D2
	{"set 2", _o(E)},		//D3
	{"set 2", _o(H)},		//D4
	{"set 2", _o(L)},		//D5
	{"set 2", _oa(HL)},	//D6
	{"set 2", _o(A)},		//D7
	{"set 3", _o(B)},		//D8
	{"set 3", _o(C)},		//D9
	{"set 3", _o(D)},		//DA
	{"set 3", _o(E)},		//DB
	{"set 3", _o(H)},		//DC
	{"set 3", _o(L)},		//DD
	{"set 3", _oa(HL)},	//DE
	{"set 3", _o(A)},		//DF
	{"set 4", _o(B)},		//E0
	{"set 4", _o(C)},		//E1
	{"set 4", _o(D)},		//E2
	{"set 4", _o(E)},		//E3
	{"set 4", _o(H)},		//E4
	{"set 4", _o(L)},		//E5
	{"set 4", _oa(HL)},	//E6
	{"set 4", _o(A)},		//E7
	{"set 5", _o(B)},		//E8
	{"set 5", _o(C)},		//E9
	{"set 5", _o(D)},		//EA
	{"set 5", _o(E)},		//EB
	{"set 5", _o(H)},		//EC
	{"set 5", _o(L)},		//ED
	{"set 5", _oa(HL)},	//EE
	{"set 5", _o(A)},		//EF
	{"set 6", _o(B)},		//F0
	{"set 6", _o(C)},		//F1
	{"set 6", _o(D)},		//F2
	{"set 6", _o(E)},		//F3
	{"set 6", _o(H)},		//F4
	{"set 6", _o(L)},		//F5
	{"set 6", _oa(HL)},	//F6
	{"set 6", _o(A)},		//F7
	{"set 7", _o(B)},		//F8
	{"set 7", _o(C)},		//F9
	{"set 7", _o(D)},		//FA
	{"set 7", _o(E)},		//FB
	{"set 7", _o(H)},		//FC
	{"set 7", _o(L)},		//FD
	{"set 7", _oa(HL)},	//FE
	{"set 7", _o(A)},		//FF
};


#undef _o_t
#undef _o_or
#undef _o
#undef _oa
#undef _oii
#undef _oid
#undef _oo
#undef _oio
