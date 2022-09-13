
// Generated from KokoLang.g4 by ANTLR 4.10.1

#pragma once


#include "antlr4-runtime.h"




class  KokoLangLexer : public antlr4::Lexer {
public:
  enum {
    COMMENT = 1, STACK = 2, LOCALS = 3, FUNC = 4, Null = 5, Bool = 6, Id = 7, 
    String = 8, Number = 9, Dp = 10, Bopen = 11, Bclose = 12, Sdot = 13, 
    Scomment = 14, Sminus = 15, NL = 16, WS = 17, ALL = 18
  };

  explicit KokoLangLexer(antlr4::CharStream *input);

  ~KokoLangLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

