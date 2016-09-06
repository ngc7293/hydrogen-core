#include "test.h"

#include <iostream>

Test::Test()
	: Object(TEST)
{
}

Test::~Test()
{
}

void Test::update()
{
}

void Test::render()
{
}

void Test::talktome()
{
	std::cout << "IT FUCKING WORKED WE DID IT REDDIT\n";
}