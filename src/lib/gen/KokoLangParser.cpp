
// Generated from KokoLang.g4 by ANTLR 4.10.1


#include "KokoLangVisitor.h"

#include "KokoLangParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct KokoLangParserStaticData final {
  KokoLangParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  KokoLangParserStaticData(const KokoLangParserStaticData&) = delete;
  KokoLangParserStaticData(KokoLangParserStaticData&&) = delete;
  KokoLangParserStaticData& operator=(const KokoLangParserStaticData&) = delete;
  KokoLangParserStaticData& operator=(KokoLangParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag kokolangParserOnceFlag;
KokoLangParserStaticData *kokolangParserStaticData = nullptr;

void kokolangParserInitialize() {
  assert(kokolangParserStaticData == nullptr);
  auto staticData = std::make_unique<KokoLangParserStaticData>(
    std::vector<std::string>{
      "program", "function", "funcblock", "stack", "local", "sentence", 
      "label", "opcode", "nlr", "nlo", "value"
    },
    std::vector<std::string>{
      "", "", "'stack'", "'locals'", "'func'", "'null'", "", "", "", "", 
      "':'", "'{'", "'}'", "'.'", "'//'", "'-'", "'\\n'"
    },
    std::vector<std::string>{
      "", "COMMENT", "STACK", "LOCALS", "FUNC", "Null", "Bool", "Id", "String", 
      "Number", "Dp", "Bopen", "Bclose", "Sdot", "Scomment", "Sminus", "NL", 
      "WS", "ALL"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,18,85,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,2,8,7,8,2,9,7,9,2,10,7,10,1,0,1,0,1,0,4,0,26,8,0,11,0,12,0,27,1,1,
  	1,1,1,1,1,1,1,1,1,1,1,2,1,2,1,2,1,2,3,2,40,8,2,1,2,4,2,43,8,2,11,2,12,
  	2,44,1,2,1,2,1,3,1,3,1,3,1,3,1,3,1,4,1,4,1,4,1,4,1,4,1,5,1,5,3,5,61,8,
  	5,1,5,1,5,1,6,1,6,1,6,1,7,1,7,3,7,70,8,7,1,8,4,8,73,8,8,11,8,12,8,74,
  	1,9,5,9,78,8,9,10,9,12,9,81,9,9,1,10,1,10,1,10,0,0,11,0,2,4,6,8,10,12,
  	14,16,18,20,0,1,1,0,5,9,80,0,25,1,0,0,0,2,29,1,0,0,0,4,35,1,0,0,0,6,48,
  	1,0,0,0,8,53,1,0,0,0,10,60,1,0,0,0,12,64,1,0,0,0,14,67,1,0,0,0,16,72,
  	1,0,0,0,18,79,1,0,0,0,20,82,1,0,0,0,22,23,3,2,1,0,23,24,3,18,9,0,24,26,
  	1,0,0,0,25,22,1,0,0,0,26,27,1,0,0,0,27,25,1,0,0,0,27,28,1,0,0,0,28,1,
  	1,0,0,0,29,30,5,13,0,0,30,31,5,4,0,0,31,32,5,7,0,0,32,33,3,18,9,0,33,
  	34,3,4,2,0,34,3,1,0,0,0,35,36,5,11,0,0,36,37,3,18,9,0,37,39,3,8,4,0,38,
  	40,3,6,3,0,39,38,1,0,0,0,39,40,1,0,0,0,40,42,1,0,0,0,41,43,3,10,5,0,42,
  	41,1,0,0,0,43,44,1,0,0,0,44,42,1,0,0,0,44,45,1,0,0,0,45,46,1,0,0,0,46,
  	47,5,12,0,0,47,5,1,0,0,0,48,49,5,13,0,0,49,50,5,2,0,0,50,51,5,9,0,0,51,
  	52,3,16,8,0,52,7,1,0,0,0,53,54,5,13,0,0,54,55,5,3,0,0,55,56,5,9,0,0,56,
  	57,3,16,8,0,57,9,1,0,0,0,58,61,3,14,7,0,59,61,3,12,6,0,60,58,1,0,0,0,
  	60,59,1,0,0,0,61,62,1,0,0,0,62,63,3,16,8,0,63,11,1,0,0,0,64,65,5,7,0,
  	0,65,66,5,10,0,0,66,13,1,0,0,0,67,69,5,7,0,0,68,70,3,20,10,0,69,68,1,
  	0,0,0,69,70,1,0,0,0,70,15,1,0,0,0,71,73,5,16,0,0,72,71,1,0,0,0,73,74,
  	1,0,0,0,74,72,1,0,0,0,74,75,1,0,0,0,75,17,1,0,0,0,76,78,5,16,0,0,77,76,
  	1,0,0,0,78,81,1,0,0,0,79,77,1,0,0,0,79,80,1,0,0,0,80,19,1,0,0,0,81,79,
  	1,0,0,0,82,83,7,0,0,0,83,21,1,0,0,0,7,27,39,44,60,69,74,79
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  kokolangParserStaticData = staticData.release();
}

}

KokoLangParser::KokoLangParser(TokenStream *input) : KokoLangParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

KokoLangParser::KokoLangParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  KokoLangParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *kokolangParserStaticData->atn, kokolangParserStaticData->decisionToDFA, kokolangParserStaticData->sharedContextCache, options);
}

KokoLangParser::~KokoLangParser() {
  delete _interpreter;
}

const atn::ATN& KokoLangParser::getATN() const {
  return *kokolangParserStaticData->atn;
}

std::string KokoLangParser::getGrammarFileName() const {
  return "KokoLang.g4";
}

const std::vector<std::string>& KokoLangParser::getRuleNames() const {
  return kokolangParserStaticData->ruleNames;
}

const dfa::Vocabulary& KokoLangParser::getVocabulary() const {
  return kokolangParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView KokoLangParser::getSerializedATN() const {
  return kokolangParserStaticData->serializedATN;
}


//----------------- ProgramContext ------------------------------------------------------------------

KokoLangParser::ProgramContext::ProgramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<KokoLangParser::FunctionContext *> KokoLangParser::ProgramContext::function() {
  return getRuleContexts<KokoLangParser::FunctionContext>();
}

KokoLangParser::FunctionContext* KokoLangParser::ProgramContext::function(size_t i) {
  return getRuleContext<KokoLangParser::FunctionContext>(i);
}

std::vector<KokoLangParser::NloContext *> KokoLangParser::ProgramContext::nlo() {
  return getRuleContexts<KokoLangParser::NloContext>();
}

KokoLangParser::NloContext* KokoLangParser::ProgramContext::nlo(size_t i) {
  return getRuleContext<KokoLangParser::NloContext>(i);
}


size_t KokoLangParser::ProgramContext::getRuleIndex() const {
  return KokoLangParser::RuleProgram;
}


std::any KokoLangParser::ProgramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitProgram(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::ProgramContext* KokoLangParser::program() {
  ProgramContext *_localctx = _tracker.createInstance<ProgramContext>(_ctx, getState());
  enterRule(_localctx, 0, KokoLangParser::RuleProgram);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(25); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(22);
      function();
      setState(23);
      nlo();
      setState(27); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == KokoLangParser::Sdot);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FunctionContext ------------------------------------------------------------------

KokoLangParser::FunctionContext::FunctionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::FunctionContext::Sdot() {
  return getToken(KokoLangParser::Sdot, 0);
}

tree::TerminalNode* KokoLangParser::FunctionContext::FUNC() {
  return getToken(KokoLangParser::FUNC, 0);
}

tree::TerminalNode* KokoLangParser::FunctionContext::Id() {
  return getToken(KokoLangParser::Id, 0);
}

KokoLangParser::NloContext* KokoLangParser::FunctionContext::nlo() {
  return getRuleContext<KokoLangParser::NloContext>(0);
}

KokoLangParser::FuncblockContext* KokoLangParser::FunctionContext::funcblock() {
  return getRuleContext<KokoLangParser::FuncblockContext>(0);
}


size_t KokoLangParser::FunctionContext::getRuleIndex() const {
  return KokoLangParser::RuleFunction;
}


std::any KokoLangParser::FunctionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitFunction(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::FunctionContext* KokoLangParser::function() {
  FunctionContext *_localctx = _tracker.createInstance<FunctionContext>(_ctx, getState());
  enterRule(_localctx, 2, KokoLangParser::RuleFunction);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(29);
    match(KokoLangParser::Sdot);
    setState(30);
    match(KokoLangParser::FUNC);
    setState(31);
    match(KokoLangParser::Id);
    setState(32);
    nlo();
    setState(33);
    funcblock();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FuncblockContext ------------------------------------------------------------------

KokoLangParser::FuncblockContext::FuncblockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::FuncblockContext::Bopen() {
  return getToken(KokoLangParser::Bopen, 0);
}

KokoLangParser::NloContext* KokoLangParser::FuncblockContext::nlo() {
  return getRuleContext<KokoLangParser::NloContext>(0);
}

KokoLangParser::LocalContext* KokoLangParser::FuncblockContext::local() {
  return getRuleContext<KokoLangParser::LocalContext>(0);
}

tree::TerminalNode* KokoLangParser::FuncblockContext::Bclose() {
  return getToken(KokoLangParser::Bclose, 0);
}

KokoLangParser::StackContext* KokoLangParser::FuncblockContext::stack() {
  return getRuleContext<KokoLangParser::StackContext>(0);
}

std::vector<KokoLangParser::SentenceContext *> KokoLangParser::FuncblockContext::sentence() {
  return getRuleContexts<KokoLangParser::SentenceContext>();
}

KokoLangParser::SentenceContext* KokoLangParser::FuncblockContext::sentence(size_t i) {
  return getRuleContext<KokoLangParser::SentenceContext>(i);
}


size_t KokoLangParser::FuncblockContext::getRuleIndex() const {
  return KokoLangParser::RuleFuncblock;
}


std::any KokoLangParser::FuncblockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitFuncblock(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::FuncblockContext* KokoLangParser::funcblock() {
  FuncblockContext *_localctx = _tracker.createInstance<FuncblockContext>(_ctx, getState());
  enterRule(_localctx, 4, KokoLangParser::RuleFuncblock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(35);
    match(KokoLangParser::Bopen);
    setState(36);
    nlo();
    setState(37);
    local();
    setState(39);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == KokoLangParser::Sdot) {
      setState(38);
      stack();
    }
    setState(42); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(41);
      sentence();
      setState(44); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == KokoLangParser::Id);
    setState(46);
    match(KokoLangParser::Bclose);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StackContext ------------------------------------------------------------------

KokoLangParser::StackContext::StackContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::StackContext::Sdot() {
  return getToken(KokoLangParser::Sdot, 0);
}

tree::TerminalNode* KokoLangParser::StackContext::STACK() {
  return getToken(KokoLangParser::STACK, 0);
}

tree::TerminalNode* KokoLangParser::StackContext::Number() {
  return getToken(KokoLangParser::Number, 0);
}

KokoLangParser::NlrContext* KokoLangParser::StackContext::nlr() {
  return getRuleContext<KokoLangParser::NlrContext>(0);
}


size_t KokoLangParser::StackContext::getRuleIndex() const {
  return KokoLangParser::RuleStack;
}


std::any KokoLangParser::StackContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitStack(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::StackContext* KokoLangParser::stack() {
  StackContext *_localctx = _tracker.createInstance<StackContext>(_ctx, getState());
  enterRule(_localctx, 6, KokoLangParser::RuleStack);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(48);
    match(KokoLangParser::Sdot);
    setState(49);
    match(KokoLangParser::STACK);
    setState(50);
    match(KokoLangParser::Number);
    setState(51);
    nlr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LocalContext ------------------------------------------------------------------

KokoLangParser::LocalContext::LocalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::LocalContext::Sdot() {
  return getToken(KokoLangParser::Sdot, 0);
}

tree::TerminalNode* KokoLangParser::LocalContext::LOCALS() {
  return getToken(KokoLangParser::LOCALS, 0);
}

tree::TerminalNode* KokoLangParser::LocalContext::Number() {
  return getToken(KokoLangParser::Number, 0);
}

KokoLangParser::NlrContext* KokoLangParser::LocalContext::nlr() {
  return getRuleContext<KokoLangParser::NlrContext>(0);
}


size_t KokoLangParser::LocalContext::getRuleIndex() const {
  return KokoLangParser::RuleLocal;
}


std::any KokoLangParser::LocalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitLocal(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::LocalContext* KokoLangParser::local() {
  LocalContext *_localctx = _tracker.createInstance<LocalContext>(_ctx, getState());
  enterRule(_localctx, 8, KokoLangParser::RuleLocal);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(53);
    match(KokoLangParser::Sdot);
    setState(54);
    match(KokoLangParser::LOCALS);
    setState(55);
    match(KokoLangParser::Number);
    setState(56);
    nlr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SentenceContext ------------------------------------------------------------------

KokoLangParser::SentenceContext::SentenceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

KokoLangParser::NlrContext* KokoLangParser::SentenceContext::nlr() {
  return getRuleContext<KokoLangParser::NlrContext>(0);
}

KokoLangParser::OpcodeContext* KokoLangParser::SentenceContext::opcode() {
  return getRuleContext<KokoLangParser::OpcodeContext>(0);
}

KokoLangParser::LabelContext* KokoLangParser::SentenceContext::label() {
  return getRuleContext<KokoLangParser::LabelContext>(0);
}


size_t KokoLangParser::SentenceContext::getRuleIndex() const {
  return KokoLangParser::RuleSentence;
}


std::any KokoLangParser::SentenceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitSentence(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::SentenceContext* KokoLangParser::sentence() {
  SentenceContext *_localctx = _tracker.createInstance<SentenceContext>(_ctx, getState());
  enterRule(_localctx, 10, KokoLangParser::RuleSentence);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx)) {
    case 1: {
      setState(58);
      opcode();
      break;
    }

    case 2: {
      setState(59);
      label();
      break;
    }

    default:
      break;
    }
    setState(62);
    nlr();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LabelContext ------------------------------------------------------------------

KokoLangParser::LabelContext::LabelContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::LabelContext::Id() {
  return getToken(KokoLangParser::Id, 0);
}

tree::TerminalNode* KokoLangParser::LabelContext::Dp() {
  return getToken(KokoLangParser::Dp, 0);
}


size_t KokoLangParser::LabelContext::getRuleIndex() const {
  return KokoLangParser::RuleLabel;
}


std::any KokoLangParser::LabelContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitLabel(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::LabelContext* KokoLangParser::label() {
  LabelContext *_localctx = _tracker.createInstance<LabelContext>(_ctx, getState());
  enterRule(_localctx, 12, KokoLangParser::RuleLabel);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    match(KokoLangParser::Id);
    setState(65);
    match(KokoLangParser::Dp);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OpcodeContext ------------------------------------------------------------------

KokoLangParser::OpcodeContext::OpcodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::OpcodeContext::Id() {
  return getToken(KokoLangParser::Id, 0);
}

KokoLangParser::ValueContext* KokoLangParser::OpcodeContext::value() {
  return getRuleContext<KokoLangParser::ValueContext>(0);
}


size_t KokoLangParser::OpcodeContext::getRuleIndex() const {
  return KokoLangParser::RuleOpcode;
}


std::any KokoLangParser::OpcodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitOpcode(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::OpcodeContext* KokoLangParser::opcode() {
  OpcodeContext *_localctx = _tracker.createInstance<OpcodeContext>(_ctx, getState());
  enterRule(_localctx, 14, KokoLangParser::RuleOpcode);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(67);
    match(KokoLangParser::Id);
    setState(69);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << KokoLangParser::Null)
      | (1ULL << KokoLangParser::Bool)
      | (1ULL << KokoLangParser::Id)
      | (1ULL << KokoLangParser::String)
      | (1ULL << KokoLangParser::Number))) != 0)) {
      setState(68);
      value();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NlrContext ------------------------------------------------------------------

KokoLangParser::NlrContext::NlrContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> KokoLangParser::NlrContext::NL() {
  return getTokens(KokoLangParser::NL);
}

tree::TerminalNode* KokoLangParser::NlrContext::NL(size_t i) {
  return getToken(KokoLangParser::NL, i);
}


size_t KokoLangParser::NlrContext::getRuleIndex() const {
  return KokoLangParser::RuleNlr;
}


std::any KokoLangParser::NlrContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitNlr(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::NlrContext* KokoLangParser::nlr() {
  NlrContext *_localctx = _tracker.createInstance<NlrContext>(_ctx, getState());
  enterRule(_localctx, 16, KokoLangParser::RuleNlr);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(71);
      match(KokoLangParser::NL);
      setState(74); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == KokoLangParser::NL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NloContext ------------------------------------------------------------------

KokoLangParser::NloContext::NloContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> KokoLangParser::NloContext::NL() {
  return getTokens(KokoLangParser::NL);
}

tree::TerminalNode* KokoLangParser::NloContext::NL(size_t i) {
  return getToken(KokoLangParser::NL, i);
}


size_t KokoLangParser::NloContext::getRuleIndex() const {
  return KokoLangParser::RuleNlo;
}


std::any KokoLangParser::NloContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitNlo(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::NloContext* KokoLangParser::nlo() {
  NloContext *_localctx = _tracker.createInstance<NloContext>(_ctx, getState());
  enterRule(_localctx, 18, KokoLangParser::RuleNlo);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(79);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == KokoLangParser::NL) {
      setState(76);
      match(KokoLangParser::NL);
      setState(81);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

KokoLangParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* KokoLangParser::ValueContext::Null() {
  return getToken(KokoLangParser::Null, 0);
}

tree::TerminalNode* KokoLangParser::ValueContext::Bool() {
  return getToken(KokoLangParser::Bool, 0);
}

tree::TerminalNode* KokoLangParser::ValueContext::String() {
  return getToken(KokoLangParser::String, 0);
}

tree::TerminalNode* KokoLangParser::ValueContext::Number() {
  return getToken(KokoLangParser::Number, 0);
}

tree::TerminalNode* KokoLangParser::ValueContext::Id() {
  return getToken(KokoLangParser::Id, 0);
}


size_t KokoLangParser::ValueContext::getRuleIndex() const {
  return KokoLangParser::RuleValue;
}


std::any KokoLangParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<KokoLangVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}

KokoLangParser::ValueContext* KokoLangParser::value() {
  ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, getState());
  enterRule(_localctx, 20, KokoLangParser::RuleValue);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(82);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << KokoLangParser::Null)
      | (1ULL << KokoLangParser::Bool)
      | (1ULL << KokoLangParser::Id)
      | (1ULL << KokoLangParser::String)
      | (1ULL << KokoLangParser::Number))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

void KokoLangParser::initialize() {
  std::call_once(kokolangParserOnceFlag, kokolangParserInitialize);
}
