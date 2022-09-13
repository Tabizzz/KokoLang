
// Generated from KokoLang.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  KokoLangParser : public antlr4::Parser {
public:
  enum {
    COMMENT = 1, STACK = 2, LOCALS = 3, FUNC = 4, Null = 5, Bool = 6, Id = 7, 
    String = 8, Number = 9, Dp = 10, Bopen = 11, Bclose = 12, Sdot = 13, 
    Scomment = 14, Sminus = 15, NL = 16, WS = 17, ALL = 18
  };

  enum {
    RuleProgram = 0, RuleFunction = 1, RuleFuncblock = 2, RuleStack = 3, 
    RuleLocal = 4, RuleSentence = 5, RuleLabel = 6, RuleOpcode = 7, RuleNlr = 8, 
    RuleNlo = 9, RuleValue = 10
  };

  explicit KokoLangParser(antlr4::TokenStream *input);

  KokoLangParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~KokoLangParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class ProgramContext;
  class FunctionContext;
  class FuncblockContext;
  class StackContext;
  class LocalContext;
  class SentenceContext;
  class LabelContext;
  class OpcodeContext;
  class NlrContext;
  class NloContext;
  class ValueContext; 

  class  ProgramContext : public antlr4::ParserRuleContext {
  public:
    ProgramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<FunctionContext *> function();
    FunctionContext* function(size_t i);
    std::vector<NloContext *> nlo();
    NloContext* nlo(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgramContext* program();

  class  FunctionContext : public antlr4::ParserRuleContext {
  public:
    FunctionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Sdot();
    antlr4::tree::TerminalNode *FUNC();
    antlr4::tree::TerminalNode *Id();
    NloContext *nlo();
    FuncblockContext *funcblock();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FunctionContext* function();

  class  FuncblockContext : public antlr4::ParserRuleContext {
  public:
    FuncblockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Bopen();
    NloContext *nlo();
    LocalContext *local();
    antlr4::tree::TerminalNode *Bclose();
    StackContext *stack();
    std::vector<SentenceContext *> sentence();
    SentenceContext* sentence(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  FuncblockContext* funcblock();

  class  StackContext : public antlr4::ParserRuleContext {
  public:
    StackContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Sdot();
    antlr4::tree::TerminalNode *STACK();
    antlr4::tree::TerminalNode *Number();
    NlrContext *nlr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StackContext* stack();

  class  LocalContext : public antlr4::ParserRuleContext {
  public:
    LocalContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Sdot();
    antlr4::tree::TerminalNode *LOCALS();
    antlr4::tree::TerminalNode *Number();
    NlrContext *nlr();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocalContext* local();

  class  SentenceContext : public antlr4::ParserRuleContext {
  public:
    SentenceContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    NlrContext *nlr();
    OpcodeContext *opcode();
    LabelContext *label();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SentenceContext* sentence();

  class  LabelContext : public antlr4::ParserRuleContext {
  public:
    LabelContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Dp();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LabelContext* label();

  class  OpcodeContext : public antlr4::ParserRuleContext {
  public:
    OpcodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    ValueContext *value();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  OpcodeContext* opcode();

  class  NlrContext : public antlr4::ParserRuleContext {
  public:
    NlrContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NL();
    antlr4::tree::TerminalNode* NL(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NlrContext* nlr();

  class  NloContext : public antlr4::ParserRuleContext {
  public:
    NloContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<antlr4::tree::TerminalNode *> NL();
    antlr4::tree::TerminalNode* NL(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  NloContext* nlo();

  class  ValueContext : public antlr4::ParserRuleContext {
  public:
    ValueContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Null();
    antlr4::tree::TerminalNode *Bool();
    antlr4::tree::TerminalNode *String();
    antlr4::tree::TerminalNode *Number();
    antlr4::tree::TerminalNode *Id();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ValueContext* value();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

