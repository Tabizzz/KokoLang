
// Generated from KokoLang.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "KokoLangParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by KokoLangParser.
 */
class  KokoLangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by KokoLangParser.
   */
    virtual std::any visitProgram(KokoLangParser::ProgramContext *context) = 0;

    virtual std::any visitFunction(KokoLangParser::FunctionContext *context) = 0;

    virtual std::any visitFuncblock(KokoLangParser::FuncblockContext *context) = 0;

    virtual std::any visitStack(KokoLangParser::StackContext *context) = 0;

    virtual std::any visitLocal(KokoLangParser::LocalContext *context) = 0;

    virtual std::any visitSentence(KokoLangParser::SentenceContext *context) = 0;

    virtual std::any visitLabel(KokoLangParser::LabelContext *context) = 0;

    virtual std::any visitOpcode(KokoLangParser::OpcodeContext *context) = 0;

    virtual std::any visitNlr(KokoLangParser::NlrContext *context) = 0;

    virtual std::any visitNlo(KokoLangParser::NloContext *context) = 0;

    virtual std::any visitValue(KokoLangParser::ValueContext *context) = 0;


};

