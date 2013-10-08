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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QScrollBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->removeCommand->setEnabled(false);
	ui->clearList->setEnabled(false);

	for (int i = 0; i < 2; i++)
	{
		addCellButton[i] = new QPushButton(ui->scrollArea->widget());
		addCellButton[i]->setIcon(QIcon::fromTheme("list-add"));
		addCellButton[i]->setGeometry(0,0, 40,40);
		addCellButton[i]->setMinimumSize(40, 40);
		addCellButton[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	}

	ui->gridLayout_2->addWidget(addCellButton[0],0,0);
	ui->gridLayout_2->addWidget(addCellButton[1],0,2);

	connect(addCellButton[0], SIGNAL(clicked()), SLOT(addCellLeft()));
	connect(addCellButton[1], SIGNAL(clicked()), SLOT(addCellRight()));

	for (int i = 0; i < 5; i++)
		addCellRight();

	startingCell = 2;
	tape[startingCell]->setStyleSheet("background-color: green; color: white;");
	ui->gridLayout_2->addWidget(ui->currentCellMoving, 1 , startingCell + 1);


	ui->newCommandToState->addItem("q*", -1);
	for (int i = 0; i < 5; i++)
	{
		ui->newCommandFromState->addItem(QString("q%1").arg(i), i);
		ui->newCommandToState->addItem(QString("q%1").arg(i), i);
	}

	debugStarted = false;
	ui->tbNext->setEnabled(false);
	ui->tbStop->setEnabled(false);
    ui->loadTape->setEnabled(false);

	programTimer = -1;
}

MainWindow::~MainWindow()
{
	delete addCellButton[0];
	delete addCellButton[1];
	for (auto i = tape.begin(); i != tape.end(); i++)
		delete *i;
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	TuringMachine::Command c;
	c.state = ui->newCommandFromState->itemData(ui->newCommandFromState->currentIndex()).toUInt();
	c.symbol = ui->newCommandFromSymbol->text();
	c.newState = ui->newCommandToState->itemData(ui->newCommandToState->currentIndex()).toInt();
	c.newSymbol = ui->newCommandSetSymbol->text();
	QChar move;
	if (ui->newCommandGoLeft->isChecked())
	{
		c.move = TuringMachine::Command::LEFT;
		move = 'L';
	}
	else
	{
		c.move = TuringMachine::Command::RIGHT;
		move = 'R';
	}

	if (!machine.addCommand(c))
	{
		QMessageBox::warning(this, "Error", "Dublicate commands.", QMessageBox::Ok);
		return;
	}

	if (c.newState >= 0)
		ui->commandsList->addItem(QString("q%1:%2 → q%3:%4 %5").arg(c.state).arg(c.symbol)
									.arg(c.newState).arg(c.newSymbol).arg(move));
	else
		ui->commandsList->addItem(QString("q%1:%2 → q*:%3").arg(c.state).arg(c.symbol)
									.arg(c.newSymbol));

	ui->clearList->setEnabled(true);
	if ((int)c.state + 1 == ui->newCommandFromState->count() || c.newState + 2 == ui->newCommandToState->count())
	{
		int state = c.state + 1;
		if (c.newState + 1 > state)
			state = c.newState + 1;
		ui->newCommandFromState->addItem(QString("q%1").arg(state), state);
		ui->newCommandToState->addItem(QString("q%1").arg(state), state);
	}

	ui->emulator->setEnabled(true);
}

void MainWindow::on_commandsList_currentRowChanged(int row)
{
	if (row >= 0 && !debugStarted)
		ui->removeCommand->setEnabled(true);
	else
		ui->removeCommand->setEnabled(false);
}

void MainWindow::on_clearList_clicked()
{
	machine.clearCommands();
	ui->commandsList->clear();
	ui->clearList->setEnabled(false);
	ui->emulator->setEnabled(false);
}

void MainWindow::on_removeCommand_clicked()
{
	int index = ui->commandsList->currentIndex().row();
	delete ui->commandsList->item(index);
	machine.removeCommand(index);
	if (ui->commandsList->count() == 0)
	{
		ui->emulator->setEnabled(false);
		ui->clearList->setEnabled(false);
	}
}


void MainWindow::addCellLeft()
{
	QLineEdit *cell = new QLineEdit(ui->scrollArea->widget());
	cell->setText("λ");
	cell->setAlignment(Qt::AlignHCenter);
	cell->setGeometry(0,0, 40,40);
	cell->setMinimumSize(40, 40);
	cell->setMaximumSize(40, 40);
	cell->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	setTabOrder(addCellButton[0], cell);

	tape.push_front(cell);

	for (int i = 0; i < tape.size(); i++)
		ui->gridLayout_2->addWidget(tape[i], 0, i + 1);

	ui->gridLayout_2->addWidget(addCellButton[1], 0 , ui->gridLayout_2->columnCount());

	startingCell++;

	ui->currentCellMoveLeft->setEnabled(true);
	ui->gridLayout_2->addWidget(ui->currentCellMoving, 1 , startingCell + 1);

}

void MainWindow::addCellRight()
{
	QLineEdit *cell = new QLineEdit(ui->scrollArea->widget());
	cell->setText("λ");
	cell->setAlignment(Qt::AlignHCenter);
	cell->setGeometry(0,0, 40,40);
	cell->setMinimumSize(40, 40);
	cell->setMaximumSize(40, 40);
	cell->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	if (tape.size() > 0)
		setTabOrder(tape.back(), cell);
	else
		setTabOrder(cell, addCellButton[1]);

	tape.push_back(cell);

	int col = tape.size();
	ui->gridLayout_2->addWidget(cell, 0, col);
	ui->gridLayout_2->addWidget(addCellButton[1], 0 , col+1);

	ui->currentCellMoveRight->setEnabled(true);
}

void MainWindow::on_tbStart_clicked()
{

	QList<QString> param;
	for (auto i = tape.begin(); i != tape.end(); i++)
	{
		(*i)->setReadOnly(true);
		param.push_back((*i)->text());
	}

	machine.fillTape(param);
	machine.startState(startingCell);

	int next = machine.nextCommand();
	if (next  >= 0)
	{
		auto item = ui->commandsList->item(next);
		item->setBackgroundColor(QColor(0, 0, 255));
		item->setTextColor(QColor(255,255,255));
	}

	debugStarted = true;
	ui->tbNext->setEnabled(true);
	ui->tbStop->setEnabled(true);
	ui->tbStart->setEnabled(false);
	ui->tbExecute->setEnabled(false);
	ui->addCommands->setEnabled(false);
	ui->clearList->setEnabled(false);
	ui->currentCellMoving->setVisible(false);

	for (int i = 0; i < 2; i++)
		addCellButton[i]->setVisible(false);

	ui->commandsList->setCurrentRow(-1);
}

void MainWindow::on_tbNext_clicked()
{
	tape[machine.getPosition()]->setStyleSheet("background-color: white; color: black;");

	machine.nextIteration();

	if (machine.lastCommand >= 0)
	{
		auto item = ui->commandsList->item(machine.lastCommand);
		item->setBackgroundColor(QColor(255, 255, 255));
		item->setTextColor(QColor(0,0,0));
	}

	int next = machine.nextCommand();
	if (next >= 0)
	{
		auto item = ui->commandsList->item(next);
		item->setBackgroundColor(QColor(0, 0, 255));
		item->setTextColor(QColor(255,255,255));
	}

	auto t = machine.getTape();
	for (int i = tape.size(); i < t.size(); i++)
	{
		addCellRight();
		tape[tape.size() - 1]->setReadOnly(true);
	}

	for (int i = 0; i < t.size(); i++)
		tape[i]->setText(t[i]);

	tape[machine.getPosition()]->setStyleSheet("background-color: green; color: white;");

	if (machine.isFinished())
		ui->tbStop->click();
}

void MainWindow::on_tbStop_clicked()
{
	debugStarted = false;
	if (programTimer != -1)
	{
		killTimer(programTimer);
		programTimer = -1;
	}

    for (int i = 0; i < ui->commandsList->count(); i++)
	{
        auto item = ui->commandsList->item(i);
		item->setBackgroundColor(QColor(255,255, 255));
		item->setTextColor(QColor(0,0,0));
	}

	tape[machine.getPosition()]->setStyleSheet("background-color: green; color: white;");

	ui->tbNext->setEnabled(false);
	ui->tbNext->setVisible(true);
	ui->tbStop->setEnabled(false);
	ui->tbStart->setEnabled(true);
	ui->tbExecute->setEnabled(true);
	ui->addCommands->setEnabled(true);
	ui->clearList->setEnabled(true);
	ui->currentCellMoving->setVisible(true);

	for (int i = 0; i < 2; i++)
		addCellButton[i]->setVisible(true);

	ui->gridLayout_2->addWidget(addCellButton[0], 0 , 0);

	for (int i = 0; i < tape.size(); i++)
	{
		tape[i]->setReadOnly(false);
		ui->gridLayout_2->addWidget(tape[i], 0, i + 1);
	}

	ui->gridLayout_2->addWidget(addCellButton[1], 0 , tape.size() + 1);

	startingCell = machine.getPosition();

   currentCellUpdate();
}

void MainWindow::on_currentCellMoveLeft_clicked()
{
	if (startingCell == 0) return;

	tape[startingCell--]->setStyleSheet("background-color: white; color: black;");
	tape[startingCell]->setStyleSheet("background-color: green; color: white;");

	if (startingCell == 0)
		ui->currentCellMoveLeft->setEnabled(false);

	ui->currentCellMoveRight->setEnabled(true);
	ui->gridLayout_2->addWidget(ui->currentCellMoving, 1 , startingCell + 1);
}

void MainWindow::on_currentCellMoveRight_clicked()
{
	if (startingCell >= tape.size() - 1) return;

	tape[startingCell++]->setStyleSheet("background-color: white; color: black;");
	tape[startingCell]->setStyleSheet("background-color: green; color: white;");

	if (startingCell >= tape.size() - 1)
		ui->currentCellMoveRight->setEnabled(false);

	ui->currentCellMoveLeft->setEnabled(true);
	ui->gridLayout_2->removeWidget(ui->currentCellMoving);
	ui->gridLayout_2->addWidget(ui->currentCellMoving, 1 , startingCell + 1);
}

void MainWindow::on_actionSave_commands_triggered()
{
	QString file = QFileDialog::getSaveFileName(this, "Save as");
	if (!file.isEmpty())
		machine.saveToFile(file);
}

void MainWindow::on_actionExit_triggered()
{
	close();
}

void MainWindow::on_actionLoad_triggered()
{
	QString file = QFileDialog::getOpenFileName(this, "Select file with saved commands");
	if (!file.isEmpty())
	{
		machine.loadFromFile(file);
		ui->commandsList->clear();
		auto t = machine.getCommands();

		for (auto i = t.begin(); i != t.end(); i++)
		{
			QChar move;
			if (i->move == TuringMachine::Command::LEFT)
				move = 'L';
			else
				move = 'R';
			if (i->newState >= 0)
				ui->commandsList->addItem(QString("q%1:%2 → q%3:%4 %5").arg(i->state).arg(i->symbol)
											.arg(i->newState).arg(i->newSymbol).arg(move));
			else
				ui->commandsList->addItem(QString("q%1:%2 → q*:%3").arg(i->state).arg(i->symbol)
											.arg(i->newSymbol));
											ui->clearList->setEnabled(true);
			while ((int)i->state > ui->newCommandFromState->count() ||
						i->newState + 2 > ui->newCommandToState->count())
			{
				int state = i->state + 1;
				if (i->newState + 1 > state)
					state = i->newState + 1;
				ui->newCommandFromState->addItem(QString("q%1").arg(state), state);
				ui->newCommandToState->addItem(QString("q%1").arg(state), state);
			}
			ui->emulator->setEnabled(true);
		}
	}
}

void MainWindow::on_tbExecute_clicked()
{
	ui->tbStart->click();
	ui->tbNext->setVisible(false);
	programTimer = startTimer(1);

}


void MainWindow::timerEvent(QTimerEvent *e)
{
	if (e->timerId() == programTimer)
	{
		ui->tbNext->click();
	}
}

void MainWindow::on_loadTape_clicked()
{
    while (tape.size() < tape_backup.size())
        addCellRight();
    for (int i = 0; i < tape_backup.size(); i++)
        tape[i]->setText(tape_backup[i]);
    while (tape.size() > tape_backup.size())
    {
        delete tape.back();
        tape.removeLast();
    }
    startingCell = startingCell_backup;
    currentCellUpdate();
}

void MainWindow::on_saveTape_clicked()
{
    tape_backup.clear();
    for (auto i = tape.begin(); i != tape.end(); i++)
        tape_backup.append((*i)->text());
    startingCell_backup = startingCell;
    ui->loadTape->setEnabled(true);
}

void MainWindow::currentCellUpdate()
{
    for (auto i = tape.begin(); i != tape.end(); i++)
        (*i)->setStyleSheet("background-color: white; color: black;");
    tape[startingCell]->setStyleSheet("background-color: green; color: white;");

    if (startingCell >= tape.size() - 1)
        ui->currentCellMoveRight->setEnabled(false);
    else
        ui->currentCellMoveRight->setEnabled(true);

    if (startingCell == 0)
        ui->currentCellMoveLeft->setEnabled(false);
    else
        ui->currentCellMoveLeft->setEnabled(true);

    ui->gridLayout_2->removeWidget(ui->currentCellMoving);
    ui->gridLayout_2->addWidget(ui->currentCellMoving, 1 , startingCell + 1);
}
