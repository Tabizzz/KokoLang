#include "KokoLangLib.h"
#include "ProgramVisitor.h"

using namespace std;

any ProgramVisitor::visitProgram(KokoLangParser::ProgramContext* ctx)
{
	auto program = new KLProgram();
	auto functionContexts = ctx->function();
	auto funcionCount = functionContexts.size();

	program->functions.reserve(funcionCount);
	for (size_t i = 0; i < funcionCount; i++)
	{
		program->functions.push_back(any_cast<KLFunction*>(visitFunction(functionContexts[i])));
	}

	return program;
}

any ProgramVisitor::visitFunction(KokoLangParser::FunctionContext* ctx)
{
	auto name = ctx->Id()->getText();
	auto body = ctx->funcblock();
	auto localstext = body->local()->Number()->getText();
	auto locals = stoi(localstext);
	if (locals < 0) locals = 0;
	auto stackctx = body->stack();
	int stack = 10;
	if (stackctx)
	{
		stack = stoi(stackctx->Number()->getText());
		if (stack < 0) stack = 0;
	}
	auto function = new KLFunction(name, locals, stack);
	auto sentences = body->sentence();
	auto sentencecount = sentences.size();
	function->body.reserve(sentencecount);
	for (int i = 0; i < sentencecount; ++i) {
		function->body.push_back(any_cast<KLInstruction*>(visitSentence(sentences[i])));
	}
	cout << "Find function with name: " << name << " with " << locals << " locals" << endl;
	return function;
}

any ProgramVisitor::visitSentence(KokoLangParser::SentenceContext *ctx) {
	KLInstruction* instruction;
	auto opcodectx = ctx->opcode();
	string name;
	if(!opcodectx)
	{
		name = ctx->label()->Id()->getText();
		return new KLInstruction(name);
	}
	auto opcode = getOpcode(opcodectx);
	auto operand = getOperand(opcodectx, opcode);
	instruction = new KLInstruction(opcode, operand);
	return instruction;
}

OpCodes ProgramVisitor::getOpcode(KokoLangParser::OpcodeContext *pContext) {
	auto code = pContext->Id()->getText();
	// https://stackoverflow.com/a/16388594
	static const map<string, OpCodes> optionStrings {
			{ "noc",		OpCodes::noc		},
			{ "go",		OpCodes::go		},
			{ "goif",	OpCodes::goif	},
			{ "goifn",	OpCodes::goifn	},
			{ "push",	OpCodes::push	},
			{ "pop",		OpCodes::pop		},
			{ "dup",		OpCodes::dup		},
			{ "clear",	OpCodes::clear	},
			{ "stackl",	OpCodes::stackl	},
			{ "stvar",	OpCodes::stvar	},
			{ "ldvar",	OpCodes::ldvar	},
			{ "set",		OpCodes::set		},
			{ "get",		OpCodes::get		},
			{ "starg",	OpCodes::starg	},
			{ "ldarg",	OpCodes::ldarg	},
			{ "and",		OpCodes::andi	},
			{ "or",		OpCodes:: ori	},
			{ "xor",		OpCodes::xori	},
			{ "oplt",	OpCodes::oplt	},
			{ "ople",	OpCodes::ople	},
			{ "opgt",	OpCodes::opgt	},
			{ "opge",	OpCodes::opge	},
			{ "opeq",	OpCodes::opeq	},
			{ "opne",	OpCodes::opne	},
			{ "add",		OpCodes::add		},
			{ "sub",		OpCodes::sub		},
			{ "mul",		OpCodes::mul		},
			{ "div",		OpCodes::divi	},
			{ "mod",		OpCodes::mod		},
			{ "pot",		OpCodes::pot		},
			{ "root",	OpCodes::root	},
			{ "log",		OpCodes::ln		},
			{ "tstr",	OpCodes::tstr	},
			{ "hash",	OpCodes::hash	},
			{ "fash",	OpCodes::fash	},
			{ "tbit",	OpCodes::tbit	},
			{ "jump",	OpCodes::jump	},
			{ "ret",		OpCodes::ret		},
			{ "call",	OpCodes::call	},
			{ "aloc",	OpCodes::aloc	},
			{ "free",	OpCodes::freei	},
			{ "copy",	OpCodes::copy	},
			{ "fill",	OpCodes::fill	},
			{ "arr",		OpCodes::arr		},
			{ "arl",		OpCodes::arl		},
			{ "lde",		OpCodes::lde		},
			{ "ste",		OpCodes::ste		},
			{ "has",		OpCodes::has		},
			{ "is",		OpCodes::is		},
			{ "as",		OpCodes::as		},
			{ "new", 	OpCodes::newi	},
			{ "sizeof",	OpCodes::size	},
			{ "ins",		OpCodes::ins		},
			{ "ref",		OpCodes::ref		},
			{ "deref",	OpCodes::deref	},
			{ "build",	OpCodes::build	},
	};

	auto itr = optionStrings.find(code);
	if( itr != optionStrings.end() ) {
		return itr->second;
	}

	throw std::invalid_argument( "invalid opcode in program" );
}

KlObject* kliCheckInteger(KokoLangParser::ValueContext* ctx)
{
	if(ctx->Number())
	{
		int value = stoi(ctx->Number()->getText());
		return KLINT(value);
	}
	throw std::invalid_argument( "invalid operand in program" );
}

KlObject* ProgramVisitor::getOperand(KokoLangParser::OpcodeContext *pContext, OpCodes codes) {
	switch (codes) {
		case noc:
		case add:
		case ret:
			break;
		case stvar:
		case ldvar:
			return kliCheckInteger(pContext->value());
		case oplt:
			break;
		case go:
			break;
		case push:
			break;
		case goif:
			break;
		case call:
			break;
	}
	return nullptr;
}
