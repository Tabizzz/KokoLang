#include "KokoLangLib.h"
#include "ProgramVisitor.h"

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
	KlObject* foperand = nullptr;
	KlObject * soperand = nullptr;
	auto values = opcodectx->value();
	KokoLangParser::ValueContext* fval = nullptr;
	KokoLangParser::ValueContext* sval = nullptr;
	auto size = values.size();
	if(size>0)
		fval = values[0];
	if(size>1)
		sval = values[1];

	getOperands(&opcode, &foperand, &soperand, fval, sval);

	instruction->opcode = opcode;
	instruction->foperand = foperand;
	instruction->soperand = soperand;
	return instruction;
}

OpCodes ProgramVisitor::getOpcode(KokoLangParser::OpcodeContext *pContext) {
	auto code = pContext->Id()->getText();
	// https://stackoverflow.com/a/16388594
	static const map<string, OpCodes> optionStrings {
			{ "noc",		OpCodes::noc	},
			{ "go",			OpCodes::go		},
			{ "goif",		OpCodes::goif	},
			{ "goifn",		OpCodes::goifn	},
			{ "push",		OpCodes::push	},
			{ "pop",		OpCodes::pop	},
			{ "dup",		OpCodes::dup	},
			{ "clear",		OpCodes::clear	},
			{ "stackl",		OpCodes::stackl	},
			{ "stvar",		OpCodes::stvar	},
			{ "ldvar",		OpCodes::ldvar	},
			{ "set",		OpCodes::set	},
			{ "get",		OpCodes::get	},
			{ "starg",		OpCodes::starg	},
			{ "ldarg",		OpCodes::ldarg	},
			{ "and",		OpCodes::andi	},
			{ "or",			OpCodes::ori	},
			{ "xor",		OpCodes::xori	},
			{ "oplt",		OpCodes::oplt	},
			{ "ople",		OpCodes::ople	},
			{ "opgt",		OpCodes::opgt	},
			{ "opge",		OpCodes::opge	},
			{ "opeq",		OpCodes::opeq	},
			{ "opne",		OpCodes::opne	},
			{ "add",		OpCodes::add	},
			{ "sub",		OpCodes::sub	},
			{ "mul",		OpCodes::mul	},
			{ "div",		OpCodes::divi	},
			{ "mod",		OpCodes::mod	},
			{ "pot",		OpCodes::pot	},
			{ "root",		OpCodes::root	},
			{ "ln",			OpCodes::ln		},
			{ "tstr",		OpCodes::tstr	},
			{ "tint",		OpCodes::tint	},
			{ "tflt",		OpCodes::tflt	},
			{ "tbit",		OpCodes::tbit	},
			{ "tobj",		OpCodes::tobj	},
			{ "cast",		OpCodes::cast	},
			{ "jump",		OpCodes::jump	},
			{ "ret",		OpCodes::ret	},
			{ "call",		OpCodes::call	},
			{ "argc",		OpCodes::argc	},
			{ "aloc",		OpCodes::aloc	},
			{ "free",		OpCodes::freei	},
			{ "copy",		OpCodes::copy	},
			{ "fill",		OpCodes::fill	},
			{ "arr",		OpCodes::arr	},
			{ "arl",		OpCodes::arl	},
			{ "ard",		OpCodes::ard	},
			{ "lde",		OpCodes::lde	},
			{ "ste",		OpCodes::ste	},
			{ "type",		OpCodes::type	},
			{ "typeof",		OpCodes::typeofi},
			{ "is",			OpCodes::is		},
			{ "new",		OpCodes::newi	},
			{ "size",		OpCodes::size	},
			{ "stfld",		OpCodes::stfld	},
			{ "ldfld",		OpCodes::ldfld	},
			{ "ref",		OpCodes::ref	},
			{ "deref",		OpCodes::deref	},
			{ "ins",		OpCodes::ins	},
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

KlObject* kliCheckOptionalInteger(KokoLangParser::ValueContext* ctx, int value)
{
	if(ctx)
	{
		auto val = ctx->Number();
		if (val) {
			auto number = stoi(val->getText());
			return KLINT(number);
		}
	}
	return KLINT(value);
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

KlObject* kliCheckOptionalIdentifier(KokoLangParser::ValueContext *ctx)
{
	if(ctx) {
		auto id = ctx->Id();
		if (id) {
			return KLSTR(id->getText());
		}
	}
	return nullptr;
}

KlObject* kliCheckPush(OpCodes *pCodes, KokoLangParser::ValueContext *ctx)
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
	auto idv = ctx->Id();
	if(idv)
	{
		*pCodes = OpCodes::get;
		return KLSTR(idv->getText());
	}
	return nullptr;
}
#pragma endregion

void ProgramVisitor::getOperands(OpCodes *pCodes, KlObject **fOperand, KlObject **sOperand,
								 KokoLangParser::ValueContext *fContext, KokoLangParser::ValueContext *sContext) {
	switch (*pCodes) {
#pragma region no operands
		case noc:
		case pop:
		case dup:
		case clear:
		case stackl:
		case andi:
		case ori:
		case xori:
		case oplt:
		case ople:
		case opgt:
		case opge:
		case opeq:
		case opne:
		case add:
		case sub:
		case mul:
		case divi:
		case mod:
		case pot:
		case root:
		case ln:
		case tstr:
		case tint:
		case tflt:
		case tbit:
		case ret:
		case argc:
		case freei:
		case OpCodes::copy:
		case OpCodes::fill:
		case ard:
		case typeofi:
		case OpCodes::ref:
		case deref:
		case reserved_ext:
			break;
#pragma endregion
#pragma region one identifier
		case go:
		case goif:
		case goifn:
		case OpCodes::set:
		case OpCodes::get:
		case tobj:
		case cast:
		case type:
		case is:
		case stfld:
		case ldfld:
		case ins:
			*fOperand = kliCheckIdentifier(fContext);
			break;
#pragma endregion
#pragma region push
		case push:
			*fOperand = kliCheckPush(pCodes, fContext);
			break;
#pragma endregion
#pragma region one integer
		case stvar:
		case ldvar:
		case starg:
		case ldarg:
		case aloc:
			*fOperand = kliCheckInteger(fContext);
			break;
#pragma endregion
#pragma region one identifier and one optional integer
		case jump:
		case call:
		case newi:
			*fOperand = kliCheckIdentifier(fContext);
			*sOperand = kliCheckOptionalInteger(sContext, -1);
			break;
#pragma endregion
#pragma region one optional integer
		case arr:
		case arl:
		case lde:
		case ste:
			*fOperand = kliCheckOptionalInteger(fContext, 0);
			break;
#pragma endregion
#pragma region one optional identifier
		case OpCodes::size:
			*fOperand = kliCheckOptionalIdentifier(fContext);
			break;
#pragma endregion
	}
}
