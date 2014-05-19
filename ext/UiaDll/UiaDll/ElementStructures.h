#pragma once
#include "ArrayHelper.h"
#include "StringHelper.h"

typedef struct _ElementInformation {
  int nativeWindowHandle;
  int* runtimeId;
  int runtimeIdLength;
  char* name;
  char* helpText;
  char* className;
  int controlTypeId;
  int* patterns;
  int patternsLength;
  char* id;

  bool isEnabled;
  bool isVisible;
  bool hasKeyboardFocus;
  long boundingRectangle[4];

  _ElementInformation() : name(NULL), nativeWindowHandle(0), runtimeId(NULL), patterns(NULL), id(NULL), className(NULL), helpText(NULL) {}

  _ElementInformation(Element^ element) : name(NULL), nativeWindowHandle(0), runtimeId(NULL), patterns(NULL), id(NULL), className(NULL), helpText(NULL) {
    Refresh(element);
  }

  static _ElementInformation* From(Element^ element) {
    return nullptr != element ? new _ElementInformation(element) : NULL;
  }

  static _ElementInformation* From(...array<Element^>^ elements) {
    if( nullptr == elements || elements->Length == 0 ) return NULL;

    auto elementInformation = new _ElementInformation[elements->Length];
    auto index = 0;

    try {
      for each(auto element in elements) {
        elementInformation[index++].Refresh(element);
      }
    } catch(Exception^) {
      delete[] elementInformation;
      throw;
    }

    return elementInformation;
  }

  void Refresh(Element^ element) {
    Reset();

    try {
      id = StringHelper::ToUnmanaged(element->Id);
      name = StringHelper::ToUnmanaged(element->Name);
      className = StringHelper::ToUnmanaged(element->ClassName);
      nativeWindowHandle = element->NativeWindowHandle;
      runtimeId = ArrayHelper::FromArray(element->RuntimeId);
      runtimeIdLength = element->RuntimeId->Length;
      controlTypeId = element->ControlTypeId;
      patterns = ArrayHelper::FromArray(element->SupportedPatternIds);
      patternsLength = element->SupportedPatternIds->Length;

      isEnabled = element->IsEnabled;
      isVisible = element->IsVisible;
      hasKeyboardFocus = element->HasKeyboardFocus;

      helpText = StringHelper::ToUnmanaged(element->HelpText);

      auto r = element->BoundingRectangle;
      for(auto coord = 0; coord < 4; coord++) {
        boundingRectangle[coord] = r[coord];
      }
    } catch(Exception^) {
      Reset();
      throw;
    }
  }

  ~_ElementInformation() {
    Reset();
  }

private:
  void Reset() {
    delete[] name; delete[] runtimeId; delete[] patterns; delete[] id; delete[] className; delete[] helpText;
    name = NULL; runtimeId = NULL; patterns = NULL; id = NULL; className = NULL; helpText = NULL;
  }

} ElementInformation, *ElementInformationPtr;
