#include "ScarlettCore.hpp"
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	Scarlett::ScarlettCore sc(argc,argv);
	sc.Run();
	return 0;
}