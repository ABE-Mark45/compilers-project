#pragma once

#include <optional>
#include <string>

struct AcceptValue {
  int priority;
  std::string value;

  AcceptValue(int priority, const std::string& value)
      : priority(priority), value(value) {}
  AcceptValue(const AcceptValue& other)
      : priority(other.priority), value(other.value) {}

  AcceptValue() : priority(INT32_MAX) {}

  bool operator<(const AcceptValue& other) const {
    return priority < other.priority;
  };

  void reduceMin(std::optional<AcceptValue> other) {
    if (other && other->priority < priority) {
      priority = other->priority;
      value = other->value;
    }
  }
};