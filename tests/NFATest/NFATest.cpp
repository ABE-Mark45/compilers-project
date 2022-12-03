#include <gtest/gtest.h>
#include <iostream>
#include "LanguageTokens/Token.h"
#include "NFA/NFA.h"

TEST(NFATest, constructCharacterGroupNFA) {
  Token token{TokenType::CHAR_GROUP, std::make_pair('a', 'z')};
  auto nfa = NFA::constructCharacterGroupNFA(token);

  auto startState = nfa->getStartState();
  auto endState = nfa->getEndState();

  for (char c = 'a'; c <= 'z'; c++) {
    const auto& [closure, acceptValue] = startState->moveThrough(c);

    ASSERT_EQ(closure.size(), 1);
    ASSERT_FALSE(acceptValue.has_value());

    const auto state = *closure.begin();

    ASSERT_EQ(state, endState);
  }

  for (char c = '0'; c <= '9'; c++) {
    const auto& [closure, acceptValue] = startState->moveThrough(c);

    ASSERT_TRUE(closure.empty());
    ASSERT_FALSE(acceptValue.has_value());
  }
}

TEST(NFATest, constructCharacterNFA) {
  Token token{TokenType::CHAR, 'x'};
  auto nfa = NFA::constructCharacterNFA(token);

  auto startState = nfa->getStartState();
  auto endState = nfa->getEndState();

  const auto& [closure, acceptValue] = startState->moveThrough('x');
  ASSERT_EQ(closure.size(), 1);
  ASSERT_FALSE(acceptValue.has_value());

  const auto state = *closure.begin();
  ASSERT_EQ(state, endState);

  const auto& [emptyClosure, emptyAcceptValue] = startState->moveThrough('y');
  ASSERT_TRUE(emptyClosure.empty());
  ASSERT_FALSE(emptyAcceptValue.has_value());
}

TEST(NFATest, constructPunctuationCharacterNFA) {
  auto nfa = NFA::constructPunctuationCharacterNFA(';', 1);

  auto startState = nfa->getStartState();
  auto endState = nfa->getEndState();

  const auto& [closure, acceptValue] = startState->moveThrough(';');
  ASSERT_EQ(closure.size(), 1);
  ASSERT_TRUE(acceptValue.has_value());
  ASSERT_EQ(acceptValue.value().priority, 1);
  ASSERT_EQ(acceptValue.value().value, ";");

  const auto state = *closure.begin();
  ASSERT_EQ(state, endState);

  const auto& [emptyClosure, emptyAcceptValue] = startState->moveThrough('y');
  ASSERT_TRUE(emptyClosure.empty());
  ASSERT_FALSE(emptyAcceptValue.has_value());
}

TEST(NFATest, constructKeywordNFA) {
  const std::string testString = "hello";
  const int priority = 2;
  auto nfa = NFA::constructKeywordNFA(testString, priority);

  auto startState = nfa->getStartState();
  auto endState = nfa->getEndState();
  auto currentState = startState;
  std::optional<State::AcceptValue> finalAcceptValue;

  for (char c : testString) {
    const auto& [closure, acceptValue] = currentState->moveThrough(c);
    finalAcceptValue = acceptValue;
    ASSERT_EQ(closure.size(), 1);
    currentState = *closure.begin();
  }

  ASSERT_EQ(currentState, endState);
  ASSERT_TRUE(finalAcceptValue.has_value());
  ASSERT_EQ(finalAcceptValue.value().priority, priority);
  ASSERT_EQ(finalAcceptValue.value().value, testString);
}

TEST(NFATest, concatenate) {
  const std::string word1 = "hello";
  const std::string word2 = "world";
  auto nfa1 = NFA::constructKeywordNFA(word1, 1);

  auto nfa2 = NFA::constructKeywordNFA(word2, 2);
  auto nfa2EndState = nfa2->getEndState();

  nfa1->concatenate(std::move(nfa2));
  auto endStateAfterConcat = nfa1->getEndState();
  auto acceptValueAfterConcat = endStateAfterConcat->getAcceptValue();
  ASSERT_EQ(endStateAfterConcat, nfa2EndState);
  ASSERT_TRUE(acceptValueAfterConcat.has_value());
  ASSERT_EQ(acceptValueAfterConcat.value().value, word2);
  ASSERT_EQ(acceptValueAfterConcat.value().priority, 2);
}