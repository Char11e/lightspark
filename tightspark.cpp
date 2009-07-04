/**************************************************************************
    Lightspark, a free flash player implementation

    Copyright (C) 2009  Alessandro Pignotti (a.pignotti@sssup.it)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
**************************************************************************/

#include "abc.h"

#include <fstream>

using namespace std;

__thread SystemState* sys=NULL;

int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		cout << "Usage: " << argv[0] << " <file.abc>" << endl;
		exit(-1);
	}
	Log::initLogging(CALLS);
	sys=new SystemState;
	sys->performance_profiling=false;

	ifstream f(argv[1]);
	ABCVm vm(sys,f);
	vm.Run(&vm);
}
