#pragma once
#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include "mc_savekeyboard.h"
#include "mc_kmxfile.h"

int dummytest_helpers();
void  check_avaiability_of_modules_();
void  check_avaiability_of_modules_mc();
void  check_avaiability_of_modules_WC();

// My std::cout : writes pre-in  ;  1 for end of line
void MyCout(std::string in, bool end, std::string pre = "");
void MyCoutW(std::wstring in, bool end, std::wstring pre =L"");

//Just for now: wrote my oẃn DebugLog -TODO to be replaced
void DebugLog_S2(std::wstring  txt, std::wstring fileName);

#endif /* HELPERS_H*/