
// Generated from KokoLang.g4 by ANTLR 4.10.1


#include "KokoLangLexer.h"


using namespace antlr4;



using namespace antlr4;

namespace {

struct KokoLangLexerStaticData final {
  KokoLangLexerStaticData(std::vector<std::string> ruleNames,
                          std::vector<std::string> channelNames,
                          std::vector<std::string> modeNames,
                          std::vector<std::string> literalNames,
                          std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), channelNames(std::move(channelNames)),
        modeNames(std::move(modeNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  KokoLangLexerStaticData(const KokoLangLexerStaticData&) = delete;
  KokoLangLexerStaticData(KokoLangLexerStaticData&&) = delete;
  KokoLangLexerStaticData& operator=(const KokoLangLexerStaticData&) = delete;
  KokoLangLexerStaticData& operator=(KokoLangLexerStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> channelNames;
  const std::vector<std::string> modeNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

std::once_flag kokolanglexerLexerOnceFlag;
KokoLangLexerStaticData *kokolanglexerLexerStaticData = nullptr;

void kokolanglexerLexerInitialize() {
  assert(kokolanglexerLexerStaticData == nullptr);
  auto staticData = std::make_unique<KokoLangLexerStaticData>(
    std::vector<std::string>{
      "COMMENT", "LOWER", "UPPER", "LETTER", "DIGIT", "ANY", "STACK", "LOCALS", 
      "FUNC", "Null", "Bool", "Id", "String", "Number", "Dp", "Bopen", "Bclose", 
      "Sdot", "Scomment", "Sminus", "NL", "WS", "ALL"
    },
    std::vector<std::string>{
      "DEFAULT_TOKEN_CHANNEL", "HIDDEN"
    },
    std::vector<std::string>{
      "DEFAULT_MODE"
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
  	4,0,18,156,6,-1,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,
  	6,2,7,7,7,2,8,7,8,2,9,7,9,2,10,7,10,2,11,7,11,2,12,7,12,2,13,7,13,2,14,
  	7,14,2,15,7,15,2,16,7,16,2,17,7,17,2,18,7,18,2,19,7,19,2,20,7,20,2,21,
  	7,21,2,22,7,22,1,0,1,0,5,0,50,8,0,10,0,12,0,53,9,0,1,0,1,0,1,1,1,1,1,
  	2,1,2,1,3,1,3,3,3,63,8,3,1,4,1,4,1,5,1,5,1,6,1,6,1,6,1,6,1,6,1,6,1,7,
  	1,7,1,7,1,7,1,7,1,7,1,7,1,8,1,8,1,8,1,8,1,8,1,9,1,9,1,9,1,9,1,9,1,10,
  	1,10,1,10,1,10,1,10,1,10,1,10,1,10,1,10,3,10,101,8,10,1,11,4,11,104,8,
  	11,11,11,12,11,105,1,12,1,12,5,12,110,8,12,10,12,12,12,113,9,12,1,12,
  	1,12,1,13,3,13,118,8,13,1,13,4,13,121,8,13,11,13,12,13,122,1,13,1,13,
  	4,13,127,8,13,11,13,12,13,128,3,13,131,8,13,1,14,1,14,1,15,1,15,1,16,
  	1,16,1,17,1,17,1,18,1,18,1,18,1,19,1,19,1,20,1,20,1,21,4,21,149,8,21,
  	11,21,12,21,150,1,21,1,21,1,22,1,22,1,111,0,23,1,1,3,0,5,0,7,0,9,0,11,
  	0,13,2,15,3,17,4,19,5,21,6,23,7,25,8,27,9,29,10,31,11,33,12,35,13,37,
  	14,39,15,41,16,43,17,45,18,1,0,5,1,0,97,122,1,0,65,90,1,0,48,57,2,0,10,
  	10,13,13,3,0,9,9,13,13,32,32,160,0,1,1,0,0,0,0,13,1,0,0,0,0,15,1,0,0,
  	0,0,17,1,0,0,0,0,19,1,0,0,0,0,21,1,0,0,0,0,23,1,0,0,0,0,25,1,0,0,0,0,
  	27,1,0,0,0,0,29,1,0,0,0,0,31,1,0,0,0,0,33,1,0,0,0,0,35,1,0,0,0,0,37,1,
  	0,0,0,0,39,1,0,0,0,0,41,1,0,0,0,0,43,1,0,0,0,0,45,1,0,0,0,1,47,1,0,0,
  	0,3,56,1,0,0,0,5,58,1,0,0,0,7,62,1,0,0,0,9,64,1,0,0,0,11,66,1,0,0,0,13,
  	68,1,0,0,0,15,74,1,0,0,0,17,81,1,0,0,0,19,86,1,0,0,0,21,100,1,0,0,0,23,
  	103,1,0,0,0,25,107,1,0,0,0,27,117,1,0,0,0,29,132,1,0,0,0,31,134,1,0,0,
  	0,33,136,1,0,0,0,35,138,1,0,0,0,37,140,1,0,0,0,39,143,1,0,0,0,41,145,
  	1,0,0,0,43,148,1,0,0,0,45,154,1,0,0,0,47,51,3,37,18,0,48,50,3,11,5,0,
  	49,48,1,0,0,0,50,53,1,0,0,0,51,49,1,0,0,0,51,52,1,0,0,0,52,54,1,0,0,0,
  	53,51,1,0,0,0,54,55,6,0,0,0,55,2,1,0,0,0,56,57,7,0,0,0,57,4,1,0,0,0,58,
  	59,7,1,0,0,59,6,1,0,0,0,60,63,3,3,1,0,61,63,3,5,2,0,62,60,1,0,0,0,62,
  	61,1,0,0,0,63,8,1,0,0,0,64,65,7,2,0,0,65,10,1,0,0,0,66,67,8,3,0,0,67,
  	12,1,0,0,0,68,69,5,115,0,0,69,70,5,116,0,0,70,71,5,97,0,0,71,72,5,99,
  	0,0,72,73,5,107,0,0,73,14,1,0,0,0,74,75,5,108,0,0,75,76,5,111,0,0,76,
  	77,5,99,0,0,77,78,5,97,0,0,78,79,5,108,0,0,79,80,5,115,0,0,80,16,1,0,
  	0,0,81,82,5,102,0,0,82,83,5,117,0,0,83,84,5,110,0,0,84,85,5,99,0,0,85,
  	18,1,0,0,0,86,87,5,110,0,0,87,88,5,117,0,0,88,89,5,108,0,0,89,90,5,108,
  	0,0,90,20,1,0,0,0,91,92,5,116,0,0,92,93,5,114,0,0,93,94,5,117,0,0,94,
  	101,5,101,0,0,95,96,5,102,0,0,96,97,5,97,0,0,97,98,5,108,0,0,98,99,5,
  	115,0,0,99,101,5,101,0,0,100,91,1,0,0,0,100,95,1,0,0,0,101,22,1,0,0,0,
  	102,104,3,7,3,0,103,102,1,0,0,0,104,105,1,0,0,0,105,103,1,0,0,0,105,106,
  	1,0,0,0,106,24,1,0,0,0,107,111,5,34,0,0,108,110,3,11,5,0,109,108,1,0,
  	0,0,110,113,1,0,0,0,111,112,1,0,0,0,111,109,1,0,0,0,112,114,1,0,0,0,113,
  	111,1,0,0,0,114,115,5,34,0,0,115,26,1,0,0,0,116,118,5,45,0,0,117,116,
  	1,0,0,0,117,118,1,0,0,0,118,120,1,0,0,0,119,121,3,9,4,0,120,119,1,0,0,
  	0,121,122,1,0,0,0,122,120,1,0,0,0,122,123,1,0,0,0,123,130,1,0,0,0,124,
  	126,5,46,0,0,125,127,3,9,4,0,126,125,1,0,0,0,127,128,1,0,0,0,128,126,
  	1,0,0,0,128,129,1,0,0,0,129,131,1,0,0,0,130,124,1,0,0,0,130,131,1,0,0,
  	0,131,28,1,0,0,0,132,133,5,58,0,0,133,30,1,0,0,0,134,135,5,123,0,0,135,
  	32,1,0,0,0,136,137,5,125,0,0,137,34,1,0,0,0,138,139,5,46,0,0,139,36,1,
  	0,0,0,140,141,5,47,0,0,141,142,5,47,0,0,142,38,1,0,0,0,143,144,5,45,0,
  	0,144,40,1,0,0,0,145,146,5,10,0,0,146,42,1,0,0,0,147,149,7,4,0,0,148,
  	147,1,0,0,0,149,150,1,0,0,0,150,148,1,0,0,0,150,151,1,0,0,0,151,152,1,
  	0,0,0,152,153,6,21,0,0,153,44,1,0,0,0,154,155,9,0,0,0,155,46,1,0,0,0,
  	11,0,51,62,100,105,111,117,122,128,130,150,1,0,1,0
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  kokolanglexerLexerStaticData = staticData.release();
}

}

KokoLangLexer::KokoLangLexer(CharStream *input) : Lexer(input) {
  KokoLangLexer::initialize();
  _interpreter = new atn::LexerATNSimulator(this, *kokolanglexerLexerStaticData->atn, kokolanglexerLexerStaticData->decisionToDFA, kokolanglexerLexerStaticData->sharedContextCache);
}

KokoLangLexer::~KokoLangLexer() {
  delete _interpreter;
}

std::string KokoLangLexer::getGrammarFileName() const {
  return "KokoLang.g4";
}

const std::vector<std::string>& KokoLangLexer::getRuleNames() const {
  return kokolanglexerLexerStaticData->ruleNames;
}

const std::vector<std::string>& KokoLangLexer::getChannelNames() const {
  return kokolanglexerLexerStaticData->channelNames;
}

const std::vector<std::string>& KokoLangLexer::getModeNames() const {
  return kokolanglexerLexerStaticData->modeNames;
}

const dfa::Vocabulary& KokoLangLexer::getVocabulary() const {
  return kokolanglexerLexerStaticData->vocabulary;
}

antlr4::atn::SerializedATNView KokoLangLexer::getSerializedATN() const {
  return kokolanglexerLexerStaticData->serializedATN;
}

const atn::ATN& KokoLangLexer::getATN() const {
  return *kokolanglexerLexerStaticData->atn;
}




void KokoLangLexer::initialize() {
  std::call_once(kokolanglexerLexerOnceFlag, kokolanglexerLexerInitialize);
}
