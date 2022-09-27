#include "KokoLangLib.h"
#include "ProgramVisitor.h"

#define INTTOREG(x) SWITCH_TYPE(x, klBType_Int, klBType_Reg)

#define SETOPERAND(x,y) operands[x] = y(values[x])

using namespace std;

any ProgramVisitor::visitProgram(KokoLangParser::ProgramContext* ctx)
{
	auto program = klCreatePackage();
	auto functionContexts = ctx->function();
	auto funcionCount = functionContexts.size();

	for (size_t i = 0; i < funcionCount; i++)
	{
		auto func = any_cast<KLFunction*>(visitFunction(functionContexts[i]));
		string name = KLCAST(kl_string, func->name)->value;

		auto find = program->functions->find(name);
		if(find == program->functions->end()) {
			program->functions->insert(pair<string, KlObject *>(name, KLWRAP(func)));
		} else {
			cout << "ignoring repeat function definition: " << name << endl;
		}
	}

	return program;
}

#define FUNCATTR(x) auto x##c = attr->x(); \
					if(x##c) *x = stoi(x##c->Number()->getText());

void parseFunctionAttributes(KokoLangParser::FuncattrsContext *pContext, unsigned char* local, unsigned char *stack, char *args,
							 unsigned char *margs)
{
	auto vect = pContext->funcattr();

	for (auto attr : vect) {
		FUNCATTR(local)
		FUNCATTR(stack)
		FUNCATTR(args)
		FUNCATTR(margs)
	}
}

any ProgramVisitor::visitFunction(KokoLangParser::FunctionContext* ctx)
{
	auto name = ctx->Id()->getText();
	auto body = ctx->funcblock();
	auto function = KLCAST(KLFunction, klIns(&klBType_Func));
	function->name = KLSTR(name);
	function->body = new vector<KLInstruction*>();

	parseFunctionAttributes(ctx->funcblock()->funcattrs(), &function->locals, &function->stack, &function->args, &function->margs);
	auto sentences = body->sentence();
	auto sentencecount = sentences.size();
	for (int i = 0; i < sentencecount; ++i) {
		function->body->push_back(any_cast<KLInstruction*>(visitSentence(sentences[i])));
	}
	function->size = function->body->size();
	return function;
}

any ProgramVisitor::visitSentence(KokoLangParser::SentenceContext *ctx) {
	auto opcodectx = ctx->opcode();
	string name;
	auto instruction = KLCAST(KLInstruction, klIns(&klBType_Instruction));

	if(!opcodectx)
	{
		name = ctx->label()->Id()->getText();
		instruction->label = KLSTR(name);
		return instruction;
	}
	auto opcode = getOpcode(opcodectx);
	auto values = opcodectx->value();
	int size = values.size(); // NOLINT(cppcoreguidelines-narrowing-conversions)
	int optionalargs = 0;
	auto requiredoperands = CheckOperandCount(size, opcode, &optionalargs);

	auto numargs = requiredoperands;
	if(optionalargs != 0)
	{
		numargs = optionalargs == -1 ? size : requiredoperands + optionalargs;
	}

	auto operandv = new KlObject*[numargs]{};

	if(requiredoperands > 0 || optionalargs != 0) getOperands(&opcode, operandv, values, size);

	instruction->opcode = opcode;
	instruction->operandc = numargs;
	instruction->operands = operandv;

	return instruction;
}

KOpcode ProgramVisitor::getOpcode(KokoLangParser::OpcodeContext *pContext) {
	auto code = pContext->Id()->getText();
	// https://stackoverflow.com/a/16388594
	static const map<string, KOpcode> optionStrings {
			{"noc",		KOpcode::noc	},
			{"go",		KOpcode::go		},
			{"goif",	KOpcode::goif	},
			{"goifn",	KOpcode::goifn	},
			{"push",	KOpcode::push	},
			{"pop",		KOpcode::pop	},
			{"dup",		KOpcode::dup	},
			{"cp",		KOpcode::cp		},
			{"mv",		KOpcode::mv		},
			{"lflag",	KOpcode::lflag	},
			{"set",		KOpcode::set	},
			{"get",		KOpcode::get	},
			{"starg",	KOpcode::starg	},
			{"ldarg",	KOpcode::ldarg	},
			{"and",		KOpcode::andi	},
			{"or",		KOpcode::ori	},
			{"xor",		KOpcode::xori	},
			{"oplt",	KOpcode::oplt	},
			{"ople",	KOpcode::ople	},
			{"opgt",	KOpcode::opgt	},
			{"opge",	KOpcode::opge	},
			{"opeq",	KOpcode::opeq	},
			{"opne",	KOpcode::opne	},
			{"add",		KOpcode::add	},
			{"sub",		KOpcode::sub	},
			{"mul",		KOpcode::mul	},
			{"div",		KOpcode::divi	},
			{"mod",		KOpcode::mod	},
			{"tstr",	KOpcode::tstr	},
			{"tint",	KOpcode::tint	},
			{"tflt",	KOpcode::tflt	},
			{"tbit",	KOpcode::tbit	},
			{"tobj",	KOpcode::tobj	},
			{"cast",	KOpcode::cast	},
			{"jump",	KOpcode::jump	},
			{"jumpa",	KOpcode::jumpa	},
			{"call",	KOpcode::call	},
			{"calla",	KOpcode::calla	},
			{"argc",	KOpcode::argc	},
			{"ret",		KOpcode::ret	},
			{"aloc",	KOpcode::aloc	},
			{"free",	KOpcode::freei	},
			{"copy",	KOpcode::copy	},
			{"fill",	KOpcode::fill	},
			{"arr",		KOpcode::arr	},
			{"arl",		KOpcode::arl	},
			{"ard",		KOpcode::ard	},
			{"lde",		KOpcode::lde	},
			{"ste",		KOpcode::ste	},
			{"type",	KOpcode::type	},
			{"typeof",	KOpcode::typeofi},
			{"is",		KOpcode::is		},
			{"new",		KOpcode::newi	},
			{"newa",	KOpcode::newa	},
			{"sizeof",	KOpcode::sizeofi},
			{"stfld",	KOpcode::stfld	},
			{"ldfld",	KOpcode::ldfld	},
			{"ref",		KOpcode::ref	},
			{"deref",	KOpcode::deref	},
			{"ins",		KOpcode::ins	},
	};

	auto itr = optionStrings.find(code);
	if( itr != optionStrings.end() ) {
		return itr->second;
	}

	throw std::invalid_argument( "invalid opcode in program" );
}

#pragma region checks
KlObject* kliCheckInteger(KokoLangParser::ValueContext* ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto val = ctx->Number();
	if(val)
	{
		auto number = stoi(val->getText());
		return KLINT(number);
	}
	throw std::invalid_argument( "invalid operand in program" );
}

KlObject * kliCheckRegOrInt(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto id = ctx->register_();
	if(id) {
		auto base = KLINT(stoi(id->Number()->getText()));
		INTTOREG(base)
		KASINT(base) += CALL_REG_COUNT;
		return base;
	}
	auto val = ctx->Number();
	if(val)
	{
		auto number = stoi(val->getText());
		return KLINT(number);
	}
	throw std::invalid_argument( "invalid operand in program" );
}

KlObject* kliCheckOptionalReg(KokoLangParser::ValueContext *ctx)
{
	if(ctx) {
		auto reg = ctx->register_();
		if (reg) {
			auto base = KLINT(stoi(reg->Number()->getText()));
			INTTOREG(base)
			KASINT(base) += CALL_REG_COUNT;
			return base;
		}
	}
	return nullptr;
}

KlObject* kliCheckReg(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto reg = ctx->register_();
	if(reg) {
		auto text = reg->Number()->getText();
		if(text == "-0") return nullptr;
		auto base = KLINT(stoi(text));
		INTTOREG(base)
		KASINT(base) += CALL_REG_COUNT;
		return base;
	}
	throw std::invalid_argument( "invalid operand in program" );
}

KlObject* kliCheckIdentifierOrReg(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto id = ctx->Id();
	if(id) {
		return KLSTR(id->getText());
	}
	auto reg = ctx->register_();
	if(reg) {
		auto base = KLINT(stoi(reg->Number()->getText()));
		INTTOREG(base)
		KASINT(base) += CALL_REG_COUNT;
		return base;
	}
	throw std::invalid_argument( "invalid operand in program" );
}
KlObject* kliCheckIdentifier(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto id = ctx->Id();
	if(id) {
		return KLSTR(id->getText());
	}
	throw std::invalid_argument( "invalid operand in program" );
}

KlObject * kliCheckAnyNoId(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto idv = ctx->Id();
	if(idv) {
		throw invalid_argument("register is not a valid operand");
	}
	auto boolv = ctx->bool_();
	if(boolv) {
		return KLBOOL(boolv->True());
	}
	auto stringv = ctx->String();
	if(stringv) {
		auto str = stringv->getText();
		auto size = str.size() - 2;
		auto vals = str.substr(1, size);
		return KLSTR(vals);
	}
	auto numberv = ctx->Number();
	if(numberv) {
		auto numt = numberv->getText();
		auto isflt = numt.find('.') != string::npos;

		if (isflt) {
			return KLFLOAT(stod(numt));
		} else {
			return KLINT(stoi(numt));
		}
	}
	auto reg = ctx->register_();
	if(reg) {
		auto base = KLINT(stoi(reg->Number()->getText()));
		INTTOREG(base)
		KASINT(base) += CALL_REG_COUNT;
		return base;
	}
	return nullptr;
}

KlObject* kliCheckAny(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto boolv = ctx->bool_();
	if(boolv) {
		return KLBOOL(boolv->True());
	}
	auto stringv = ctx->String();
	if(stringv) {
		auto str = stringv->getText();
		auto size = str.size() - 2;
		auto vals = str.substr(1, size);
		return KLSTR(vals);
	}
	auto numberv = ctx->Number();
	if(numberv) {
		auto numt = numberv->getText();
		auto isflt = numt.find('.') != string::npos;

		if (isflt) {
			return KLFLOAT(stod(numt));
		} else {
			return KLINT(stoi(numt));
		}
	}
	auto reg = ctx->register_();
	if(reg) {
		auto base = KLINT(stoi(reg->Number()->getText()));
		INTTOREG(base)
		KASINT(base) += CALL_REG_COUNT;
		return base;
	}
	auto idv = ctx->Id();
	if(idv) {
		return KLSTR(idv->getText());
	}
	return nullptr;
}

KlObject* kliCheckAnyNoReg(KokoLangParser::ValueContext *ctx)
{
	if(!ctx) throw std::invalid_argument("missing required operand");
	auto reg = ctx->register_();
	if(reg) {
		throw invalid_argument("register is not a valid operand");
	}

	auto boolv = ctx->bool_();
	if(boolv) {
		return KLBOOL(boolv->True());
	}
	auto stringv = ctx->String();
	if(stringv) {
		auto str = stringv->getText();
		auto size = str.size() - 2;
		auto vals = str.substr(1, size);
		return KLSTR(vals);
	}
	auto numberv = ctx->Number();
	if(numberv) {
		auto numt = numberv->getText();
		auto isflt = numt.find('.') != string::npos;

		if (isflt) {
			return KLFLOAT(stod(numt));
		} else {
			return KLINT(stoi(numt));
		}
	}
	auto idv = ctx->Id();
	if(idv) {
		return KLSTR(idv->getText());
	}
	return nullptr;
}

#pragma endregion

void ProgramVisitor::getOperands(KOpcode *pOpcode, KlObject **operands, const vector<KokoLangParser::ValueContext *>& values, size_t size) {
	switch (*pOpcode) {
#pragma region 1id
		case go:
		case goif:
		case goifn:
			SETOPERAND(0, kliCheckIdentifier);
			break;
#pragma endregion
#pragma region 1any_no_reg 1reg
		case push:
			if(values[0]->Id()) *pOpcode = KOpcode::get;
			SETOPERAND(0, kliCheckAnyNoReg);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1reg
		case pop:
		case argc:
		case freei:
		case ard:
		case KOpcode::ref:
		case deref:
			SETOPERAND(0, kliCheckReg);
			break;
#pragma endregion
#pragma region 2reg
		case dup:
		case cp:
		case mv:
		case typeofi:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1int 1reg
		case lflag:
		case starg:
		case ldarg:
		case aloc:
			SETOPERAND(0, kliCheckInteger);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 1id 1reg
		case KOpcode::set:
		case KOpcode::get:
		case type:
		case is:
		case ins:
			break;
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 2any_no_id
		case andi:
		case ori:
		case xori:
		case oplt:
		case ople:
		case opgt:
		case opge:
		case opeq:
		case opne:
			SETOPERAND(0, kliCheckAnyNoId);
			SETOPERAND(1, kliCheckAnyNoId);
			break;
#pragma endregion
#pragma region 2any_no_id 1reg
		case add:
		case sub:
		case mul:
		case divi:
		case mod:
			SETOPERAND(0, kliCheckAnyNoId);
			SETOPERAND(1, kliCheckAnyNoId);
			SETOPERAND(2, kliCheckReg);
			break;
#pragma endregion
#pragma region 1any 1reg
		case tstr:
		case tint:
		case tflt:
		case tbit:
			SETOPERAND(0, kliCheckAny);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
#pragma region 3reg
		case tobj:
		case cast:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckReg);
			break;
#pragma endregion
#pragma region 1id 1reg Uany_no_id
		case jump:
		case call:
		case newi:
		{
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckReg);
			for (int i = 2; i < size; ++i) {
				operands[i] = kliCheckAnyNoId(values[i]);
			}
			break;
		}
#pragma endregion
#pragma region 1id 2reg
		case jumpa:
		case calla:
		case newa:
		case stfld:
		case ldfld:
			SETOPERAND(0, kliCheckIdentifier);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckReg);
			break;
#pragma endregion
#pragma region 1opreg
		case ret:
			if(size > 0) {
				SETOPERAND(0, kliCheckOptionalReg);
			} else {
				operands[0] = nullptr;
			}
			break;
#pragma endregion
#pragma region 2reg 1reg_or_int
		case KOpcode::copy:
		case arl:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckRegOrInt);
			break;
#pragma endregion
#pragma region 1reg 2reg_or_int
		case KOpcode::fill:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckRegOrInt);
			SETOPERAND(2, kliCheckRegOrInt);
			break;
#pragma endregion
#pragma region 1reg 1reg_or_int Ureg_or_int
		case arr:
		{
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckRegOrInt);
			for (int i = 2; i < size; ++i) {
				operands[i] = kliCheckRegOrInt(values[i]);
			}
			break;
		}
#pragma endregion
#pragma region 2reg 1reg_or_int Ureg_or_int
		case lde:
		case ste:
			SETOPERAND(0, kliCheckReg);
			SETOPERAND(1, kliCheckReg);
			SETOPERAND(2, kliCheckRegOrInt);
			for (int i = 3; i < size; ++i) {
				operands[i] = kliCheckRegOrInt(values[i]);
			}
			break;
#pragma endregion
#pragma region 1id_or_reg 1reg
		case sizeofi:
			SETOPERAND(0, kliCheckIdentifierOrReg);
			SETOPERAND(1, kliCheckReg);
			break;
#pragma endregion
		default:
			break;
	}
}

int inline ProgramVisitor::CheckOperandCount(size_t size, KOpcode opcode, int* optionals) {
	int flag = 0;
	switch (opcode) {
#pragma region zero one
		case ret:
			*optionals = 1;
			break;
#pragma endregion
#pragma region one
		case go:
		case goif:
		case goifn:
		case pop:
		case argc:
		case freei:
		case ard:
		case KOpcode::ref:
		case deref:
			flag = 1;
			break;
#pragma endregion
#pragma region two unlimited
		case jump:
		case call:
		case newi:
		case arr:
			*optionals = -1;
#pragma endregion
#pragma region two
		case push:
		case dup:
		case cp:
		case mv:
		case lflag:
		case KOpcode::set:
		case KOpcode::get:
		case starg:
		case ldarg:
		case andi:
		case ori:
		case xori:
		case oplt:
		case ople:
		case opgt:
		case opge:
		case opeq:
		case opne:
		case tstr:
		case tint:
		case tflt:
		case tbit:
		case aloc:
		case type:
		case typeofi:
		case is:
		case sizeofi:
		case ins:
			flag = 2;
			break;
#pragma endregion
#pragma region three unlimited
		case lde:
		case ste:
			*optionals = -1;
#pragma endregion
#pragma region three
		case add:
		case sub:
		case mul:
		case divi:
		case mod:
		case tobj:
		case cast:
		case jumpa:
		case calla:
		case KOpcode::copy:
		case KOpcode::fill:
		case arl:
		case newa:
		case stfld:
		case ldfld:
			flag = 3;
			break;
#pragma endregion
		default:
			break;
	}

	if( size < flag) throw invalid_argument("invalid operand count in program");
	return flag;
}

#undef INTTOREG
#undef SETOPERAND