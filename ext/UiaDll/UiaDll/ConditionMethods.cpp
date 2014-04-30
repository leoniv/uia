#include "Stdafx.h"

using namespace std;

extern "C" {
  __declspec(dllexport) void Condition_Release(SearchConditionPtr searchCondition) {
    delete searchCondition;
  }

  _declspec(dllexport) SearchConditionPtr Condition_Id(const char* id) {
    return new SearchCondition(AutomationElement::AutomationIdProperty->Id, id);
  }

  _declspec(dllexport) SearchConditionPtr Condition_Name(const char* name) {
    return new SearchCondition(AutomationElement::NameProperty->Id, name);
  }

  __declspec(dllexport) SearchConditionPtr Condition_Pattern(const char* pattern, char* errorInfo, const int errorInfoLength) {
    try {
      auto patternPropertyId = dynamic_cast<AutomationProperty^>(AutomationElement::typeid->GetField(gcnew String(pattern))->GetValue(nullptr))->Id;
      return new SearchCondition(patternPropertyId, true);
    } catch(Exception^ e) {
      StringHelper::CopyToUnmanagedString(String::Format("{0} is an invalid AutomationProperty", gcnew String(pattern)), errorInfo, errorInfoLength);
      return NULL;
    }
  }

  __declspec(dllexport) SearchConditionPtr Condition_ControlType(const int n, const int arg0, ...) {
    va_list arguments;
    va_start(arguments, arg0);

    list<const int> controlTypes;
    controlTypes.push_back(arg0);
    for(auto index = 1; index < n; index++) {
      auto value = va_arg(arguments, int);
      controlTypes.push_back(value);
    }

    return SearchCondition::FromControlTypes(controlTypes);
  }
}