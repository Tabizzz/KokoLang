#include "kilasm_internal.h"
#include <stdexcept>

#define INTTOREG(x) SWITCH_TYPE(x, klint_t, klreg_t)

#define SETOPERAND(x, y) operands[x] = y(values[x])

using namespace std;

KLOpcode ProgramVisitor::getOpcode(KokoLangParser::OpcodeContext *pContext) {
	auto code = pContext->Id()->getText();
	// https://stackoverflow.com/a/16388594
	static const map<string, KLOpcode> optionStrings{
		{"noc",    KLOpcode::noc},
		{"go",     KLOpcode::go},
		{"goif",   KLOpcode::goif},
		{"goifn",  KLOpcode::goifn},
		{"push",   KLOpcode::push},
		{"pop",    KLOpcode::pop},
		{"cl",     KLOpcode::cl},
		{"cp",     KLOpcode::cp},
		{"mv",     KLOpcode::mv},
		{"lflag",  KLOpcode::lflag},
		{"set",    KLOpcode::set},
		{"get",    KLOpcode::get},
		{"starg",  KLOpcode::starg},
		{"ldarg",  KLOpcode::ldarg},
		{"and",    KLOpcode::andi},
		{"or",     KLOpcode::ori},
		{"xor",    KLOpcode::xori},
		{"oplt",   KLOpcode::oplt},
		{"ople",   KLOpcode::ople},
		{"opgt",   KLOpcode::opgt},
		{"opge",   KLOpcode::opge},
		{"opeq",   KLOpcode::opeq},
		{"opne",   KLOpcode::opne},
		{"add",    KLOpcode::add},
		{"sub",    KLOpcode::sub},
		{"mul",    KLOpcode::mul},
		{"div",    KLOpcode::div},
		{"mod",    KLOpcode::mod},
		{"tstr",   KLOpcode::tstr},
		{"tint",   KLOpcode::tint},
		{"tflt",   KLOpcode::tflt},
		{"tbit",   KLOpcode::tbit},
		{"tobj",   KLOpcode::tobj},
		{"cast",   KLOpcode::cast},
		{"ivk",    KLOpcode::ivk},
		{"call",   KLOpcode::call},
		{"argc",   KLOpcode::argc},
		{"ret",    KLOpcode::ret},
		{"aloc",   KLOpcode::aloc},
		{"free",   KLOpcode::freei},
		{"copy",   KLOpcode::copy},
		{"fill",   KLOpcode::fill},
		{"arr",    KLOpcode::arr},
		{"arl",    KLOpcode::arl},
		{"ard",    KLOpcode::ard},
		{"lde",    KLOpcode::lde},
		{"ste",    KLOpcode::ste},
		{"type",   KLOpcode::type},
		{"typeof", KLOpcode::typeofi},
		{"is",     KLOpcode::is},
		{"new",    KLOpcode::newi},
		{"sizeof", KLOpcode::sizeofi},
		{"stfld",  KLOpcode::stfld},
		{"ldfld",  KLOpcode::ldfld},
		{"ref",    KLOpcode::ref},
		{"deref",  KLOpcode::deref},
		{"ins",    KLOpcode::ins},
		{"ldfnd",  KLOpcode::ldfnd},
	};

	auto itr = optionStrings.find(code);
	if (itr != optionStrings.end()) {
		return itr->second;
	}

	throw std::invalid_argument("invalid opcode in program");
}

#define RETURN_REG auto reg = ctx->register_();    \
if(reg) {                                        \
auto regtext = reg->Number()->getText();        \
if(regtext == "-0") return nullptr;                \
auto base = KLINT(stol(regtext));                \
INTTOREG(base)                                    \
KASINT(base) += CALL_REG_COUNT;                    \
return base;                                    \
}

#define RETURN_INT auto val = ctx->Number();    \
if(val) {                                        \
return KLINT(stol(val->getText()));                \
}

#define  RETURN_ID auto id = ctx->identifier();    \
if(id) {                                        \
return KLSTR(id->getText());                    \
}

#define RETURN_ANY_CORE                        \
auto boolv = ctx->bool_();                        \
if(boolv) {                                        \
return KLBOOL(boolv->True()?1:0);                \
}                                                \
auto stringv = ctx->String();                    \
if(stringv) {                                    \
auto str = stringv->getText();                    \
auto size = str.size() - 2;                        \
auto vals = str.substr(1, size);                \
return KLSTR(vals);                                \
}                                                \
auto numberv = ctx->Number();                    \
if(numberv) {                                    \
auto numt = numberv->getText();                    \
auto isflt = numt.find('.') != string::npos;    \
if (isflt) {                                    \
return KLFLOAT(stod(numt));                        \
} else {                                        \
return KLINT(stol(numt));                        \
}}                                                \

#pragma region checks

KLObject *kliCheckInteger(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	RETURN_INT
	throw std::invalid_argument("invalid operand in program");
}

KLObject *kliCheckRegOrInt(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	RETURN_REG
	RETURN_INT
	throw std::invalid_argument("invalid operand in program");
}

KLObject *kliCheckOptionalAnyNoId(KokoLangParser::ValueContext *ctx) {
	if (ctx) {
		auto idv = ctx->identifier();
		if (idv) {
			throw invalid_argument("identifier is not a valid operand");
		}
		RETURN_ANY_CORE
		RETURN_REG
	}
	return nullptr;
}

KLObject *kliCheckReg(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	RETURN_REG
	throw std::invalid_argument("invalid operand in program");
}

KLObject *kliCheckIdentifierOrReg(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	RETURN_ID
	RETURN_REG
	throw std::invalid_argument("invalid operand in program");
}

KLObject *kliCheckIdentifier(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	RETURN_ID
	throw std::invalid_argument("invalid operand in program");
}

KLObject *kliCheckAnyNoId(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	auto idv = ctx->identifier();
	if (idv) {
		throw invalid_argument("identifier is not a valid operand");
	}
	RETURN_ANY_CORE
	RETURN_REG
	return nullptr;
}

KLObject *kliCheckAnyNoReg(KokoLangParser::ValueContext *ctx) {
	if (!ctx) throw std::invalid_argument("missing required operand");
	auto reg = ctx->register_();
	if (reg) {
		throw invalid_argument("register is not a valid operand");
	}
	RETURN_ANY_CORE
	RETURN_ID
	return nullptr;
}

#pragma endregion

void ProgramVisitor::getOperands(KLOpcode *pOpcode, KLObject **operands, const vector<KokoLangParser::ValueContext *> &values, size_t size) {
	switch (*pOpcode) {
#pragma region 1id
		case KLOpcode::go:
		case KLOpcode::goif:
		case KLOpcode::goifn:
			SETOPERAND(0, kliCheckIdentifier);
			break;
#pragma endregion
#pragma region 1any_no_reg 1reg
		case KLOpcode::push:
			if (values[0]->identifier()) *pOpcode = KLOpcode::get;
			SETOPERAND(0, kliCheckAnyNoReg);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1reg
		case KLOpcode::pop:
		case KLOpcode::argc:
		case KLOpcode::freei:
		case KLOpcode::ref:
		case KLOpcode::deref:
			SETOPERAND(0, kliCheckReg);
			break;
#pragma endregion
#pragma region 2reg
		case KLOpcode::cl:
		case KLOpcode::cp:
		case KLOpcode::mv:
		case KLOpcode::typeofi:
		case KLOpcode::arl:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1int 1reg
		case KLOpcode::lflag:
		case KLOpcode::aloc:
			SETOPERAND(0, kliCheckInteger);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1int_or_reg 1reg
		case KLOpcode::starg:
			SETOPERAND(0, kliCheckRegOrInt);
			SETOPERAND(1, kliCheckAnyNoId);
			break;
#pragma endregion
#pragma region 1int_or_reg 1reg
		case KLOpcode::ldarg:
			SETOPERAND(0, kliCheckRegOrInt);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1id 1reg
		case KLOpcode::set:
		case KLOpcode::get:
		case KLOpcode::type:
		case KLOpcode::is:
		case KLOpcode::ins:
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 2any_no_id
		case KLOpcode::andi:
		case KLOpcode::ori:
		case KLOpcode::xori:
		case KLOpcode::oplt:
		case KLOpcode::ople:
		case KLOpcode::opgt:
		case KLOpcode::opge:
		case KLOpcode::opeq:
		case KLOpcode::opne:
			SETOPERAND(0, kliCheckAnyNoId);
			SETOPERAND(1, kliCheckAnyNoId);
			break;
#pragma endregion
#pragma region 2any_no_id 1reg
		case KLOpcode::add:
		case KLOpcode::sub:
		case KLOpcode::mul:
		case KLOpcode::div:
		case KLOpcode::mod:
			SETOPERAND(0, kliCheckAnyNoId);
			SETOPERAND(1, kliCheckAnyNoId);
			SETOPERAND(2, kliCheckReg);
			break;
#pragma endregion
#pragma region 1any_no_id 1reg
		case KLOpcode::tstr:
		case KLOpcode::tint:
		case KLOpcode::tflt:
		case KLOpcode::tbit:
			SETOPERAND(0, kliCheckAnyNoId);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1id 1any_no_id 1reg
		case KLOpcode::tobj:
		case KLOpcode::cast:
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckAnyNoId);
			SETOPERAND(2, kliCheckReg);
			break;
#pragma endregion
#pragma region 1reg Uany_no_id
		case KLOpcode::ard: {
			SETOPERAND(0, kliCheckReg);
			for (int i = 1; i < size; ++i) {
				operands[i] = kliCheckAnyNoId(values[i]);
			}
			break;
		}
#pragma endregion
#pragma region 2reg Uany_no_id
		case KLOpcode::ivk: {
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			for (int i = 2; i < size; ++i) {
				operands[i] = kliCheckAnyNoId(values[i]);
			}
			break;
		}
#pragma endregion
#pragma region 1id 1reg Uany_no_id
		case KLOpcode::call:
		case KLOpcode::newi: {
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckReg);
			for (int i = 2; i < size; ++i) {
				operands[i] = kliCheckAnyNoId(values[i]);
			}
			break;
		}
#pragma endregion
#pragma region 1id 2reg
		case KLOpcode::stfld:
		case KLOpcode::ldfld:
		case KLOpcode::ldfnd:
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckReg);
			break;
#pragma endregion
#pragma region 1opany_no_id
		case KLOpcode::ret:
			if (size > 0) {
				SETOPERAND(0, kliCheckOptionalAnyNoId);
			} else {
				operands[0] = nullptr;
			}
			break;
#pragma endregion
#pragma region 2reg 1reg_or_int
		case KLOpcode::copy:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckRegOrInt);
			break;
#pragma endregion
#pragma region 1reg 2reg_or_int
		case KLOpcode::fill:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckRegOrInt);
			SETOPERAND(2, kliCheckRegOrInt);
			break;
#pragma endregion
#pragma region 1reg 1reg_or_int
		case KLOpcode::arr:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckRegOrInt);
			break;
#pragma endregion
#pragma region 2reg 1any_no_id
		case KLOpcode::lde:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckAnyNoId);
			break;
#pragma endregion
#pragma region 1reg 2any_no_id
		case KLOpcode::ste:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckAnyNoId);
			SETOPERAND(2, kliCheckAnyNoId);
			break;
#pragma endregion
#pragma region 1id_or_reg 1reg
		case KLOpcode::sizeofi:
			SETOPERAND(0, kliCheckIdentifierOrReg);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
		default:
			break;
	}
}

int ProgramVisitor::CheckOperandCount(size_t size, KLOpcode opcode, int *optionals) {
	int flag = 0;
	switch (opcode) {
#pragma region zero one
		case KLOpcode::ret:
			*optionals = 1;
			break;
#pragma endregion
#pragma region one unlimited
		case KLOpcode::ard:
			*optionals = -1;
#pragma endregion
#pragma region one
		case KLOpcode::go:
		case KLOpcode::goif:
		case KLOpcode::goifn:
		case KLOpcode::pop:
		case KLOpcode::argc:
		case KLOpcode::freei:
		case KLOpcode::ref:
		case KLOpcode::deref:
			flag = 1;
			break;
#pragma endregion
#pragma region two unlimited
		case KLOpcode::ivk:
		case KLOpcode::call:
		case KLOpcode::newi:
			*optionals = -1;
#pragma endregion
#pragma region two
		case KLOpcode::push:
		case KLOpcode::cl:
		case KLOpcode::cp:
		case KLOpcode::mv:
		case KLOpcode::lflag:
		case KLOpcode::set:
		case KLOpcode::get:
		case KLOpcode::starg:
		case KLOpcode::ldarg:
		case KLOpcode::andi:
		case KLOpcode::ori:
		case KLOpcode::xori:
		case KLOpcode::oplt:
		case KLOpcode::ople:
		case KLOpcode::opgt:
		case KLOpcode::opge:
		case KLOpcode::opeq:
		case KLOpcode::opne:
		case KLOpcode::tstr:
		case KLOpcode::tint:
		case KLOpcode::tflt:
		case KLOpcode::tbit:
		case KLOpcode::aloc:
		case KLOpcode::type:
		case KLOpcode::typeofi:
		case KLOpcode::is:
		case KLOpcode::sizeofi:
		case KLOpcode::ins:
		case KLOpcode::arl:
		case KLOpcode::arr:
			flag = 2;
			break;
#pragma endregion
#pragma region three
		case KLOpcode::add:
		case KLOpcode::sub:
		case KLOpcode::mul:
		case KLOpcode::div:
		case KLOpcode::mod:
		case KLOpcode::tobj:
		case KLOpcode::cast:
		case KLOpcode::copy:
		case KLOpcode::fill:
		case KLOpcode::stfld:
		case KLOpcode::ldfld:
		case KLOpcode::ldfnd:
		case KLOpcode::lde:
		case KLOpcode::ste:
			flag = 3;
			break;
#pragma endregion
		default:
			break;
	}

	if (size < flag) throw invalid_argument("invalid operand count in program");
	return flag;
}

#undef INTTOREG
#undef SETOPERAND