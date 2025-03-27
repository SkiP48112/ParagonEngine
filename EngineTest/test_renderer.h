#pragma once

#include "test.h"


class testENGINE_TEST : public testTEST
{
public:
   bool Initialize() override;
   void Run() override;
   void Shutdown() override;
};