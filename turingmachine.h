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
#ifndef TURINGMACHINE_H
#define TURINGMACHINE_H

#include <QString>
#include <QList>

class TuringMachine
{
public:
	struct Command
	{
		enum Direction{LEFT, RIGHT};
		uint state;
		QString symbol;
		int newState;
		QString newSymbol;
		Direction move;

		Command():state(0), symbol(""), newState(0), newSymbol(""), move(LEFT){}
		Command(uint state, const QString &symbol, int newState, const QString &newSymbol, Direction move)
		{
			this->state = state;
			this->newState = newState;
			this->symbol = symbol;
			this->newSymbol = newSymbol;
			this->move = move;

		}
		Command(const Command &command)
		{
			this->state = command.state;
			this->newState = command.newState;
			this->symbol = command.symbol;
			this->newSymbol = command.newSymbol;
			this->move = command.move;
		}
		bool operator == (const Command &other)
		{
			return state == other.state && symbol == other.symbol;
		}
	};
private:
	QList<QString> tape;
	QList<Command> commands;

	int state;
	int position;
	QString currentSymbol() const;
	void replaceCurrentSymbol(const QString &symbol);
	void fixTape();
public:
	QString defaultSymbol;
	int lastCommand;

	TuringMachine():state(0), position(0), defaultSymbol("λ"), lastCommand(-1){}


	void startState(uint pos);
	void fillTape(const QList<QString> &tape);
	bool addCommand(const Command &command); // return false if there are errors
	void clearCommands();
	void removeCommand(uint num);
	QList<Command> getCommands();

	bool isFinished() const;
	bool nextIteration(); // returns true if finished

	QList<QString> getTape() const;
	uint getPosition() const;

	int nextCommand() const;

	void saveToFile(const QString &filename) const;
	void loadFromFile(const QString &filename);
};

#endif // TURINGMACHINE_H
