
// Generated from KokoLang.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"
#include "KokoLangVisitor.h"


/**
 * This class provides an empty implementation of KokoLangVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  KokoLangBaseVisitor : public KokoLangVisitor {
public:

  virtual std::any visitProgram(KokoLangParser::ProgramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFunction(KokoLangParser::FunctionContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitFuncblock(KokoLangParser::FuncblockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStack(KokoLangParser::StackContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLocal(KokoLangParser::LocalContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitSentence(KokoLangParser::SentenceContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLabel(KokoLangParser::LabelContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitOpcode(KokoLangParser::OpcodeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNlr(KokoLangParser::NlrContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitNlo(KokoLangParser::NloContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitValue(KokoLangParser::ValueContext *ctx) override {
    return visitChildren(ctx);
  }


};

