/*
*   Copyright (c) 2013 Pawel Zimnicki(zimnicky@gmail.com)
*
*   This file is part of the Turing machine emulator.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*   THE SOFTWARE.
*/
#include "turingmachine.h"
#include <QFile>
#include <QTextStream>


void TuringMachine::fillTape(const QList<QString> &tape)
{
	this->tape.clear();
	this->tape = tape;
	fixTape();
}

bool TuringMachine::addCommand(const Command &command)
{
	if (commands.contains(command))
		return false;
	commands.push_back(command);
	return true;
}


void TuringMachine::removeCommand(uint num)
{
	if ((int)num < commands.size())
		commands.removeAt(num);
}

void TuringMachine::startState(uint pos)
{
	state = 0;
	position = pos;
	fixTape();
}

void TuringMachine::clearCommands()
{
	commands.clear();
	state = 0;
	position = 0;
}


bool TuringMachine::isFinished() const
{
	return state < 0;
}


QString TuringMachine::currentSymbol() const
{
	if (position >= tape.size() || position < 0)
		return defaultSymbol;
	return tape[position];
}


void TuringMachine::replaceCurrentSymbol(const QString &symbol)
{
	tape[position] = symbol;
}

void TuringMachine::fixTape()
{
	while (position < 0)
	{
		position++;
		tape.push_front(defaultSymbol);
	}

	while (position >= tape.size())
		tape.push_back(defaultSymbol);
}

bool TuringMachine::nextIteration()
{
	if (isFinished())
		return true;

	lastCommand = nextCommand();
	if (lastCommand < 0) return false; // undefined state

	Command c = commands[lastCommand];

	state = c.newState;
	replaceCurrentSymbol(c.newSymbol);

	if (!isFinished())
	{
		if (c.move == Command::LEFT)
			position--;
		else
			position++;

		fixTape();
		return false;
	}

	return true;
}


QList<QString> TuringMachine::getTape() const
{
	return tape;
}


uint TuringMachine::getPosition() const
{
	return position;
}


int TuringMachine::nextCommand() const
{
	int res = -1;
	for (int i = 0; i < commands.size() && res == -1; i++)
		if ((int)commands[i].state == state && commands[i].symbol == currentSymbol())
		{
			res = i;
		}
	return res;
}


void TuringMachine::saveToFile(const QString &filename) const
{
	QFile f(filename);
	f.open(QFile::WriteOnly);
	QTextStream out(&f);
	out << commands.size() << endl;
	for (auto i = commands.begin(); i != commands.end(); i++)
	{
		out << i->state << " " << i->newState << " " << i->move << endl;
		out << i->symbol << endl;
		out << i->newSymbol << endl;
	}
	f.close();
}

void TuringMachine::loadFromFile(const QString &filename)
{
	QFile f(filename);
	f.open(QFile::ReadOnly);
	QTextStream in(&f);
	int size, t;
	in >> size;
	for (int i = 0; i < size; i++)
	{
		Command c;
		in >> c.state >> c.newState >> t;
		c.move = (Command::Direction)t;
		in >> c.symbol;
		in >> c.newSymbol;
		addCommand(c);
	}
	f.close();
}


QList<TuringMachine::Command> TuringMachine::getCommands()
{
	return commands;
}
