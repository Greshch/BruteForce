#pragma once
#include "AlgorithmsBase.h"
class Md_5Algorithm : public AlgorithmsBase {
public:
	void PasswordToKey(std::string& password) override;
private:

};

