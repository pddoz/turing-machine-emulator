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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QLineEdit>
#include <QPushButton>

#include "turingmachine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	
private slots:
	void on_pushButton_clicked();

	void on_commandsList_currentRowChanged(int currentRow);

	void on_clearList_clicked();

	void on_removeCommand_clicked();

	void addCellLeft();

	void addCellRight();
	void on_tbStart_clicked();

	void on_tbNext_clicked();

	void on_tbStop_clicked();

	void on_currentCellMoveLeft_clicked();

	void on_currentCellMoveRight_clicked();

	void on_actionSave_commands_triggered();

	void on_actionExit_triggered();

	void on_actionLoad_triggered();

	void on_tbExecute_clicked();

    void on_loadTape_clicked();

    void on_saveTape_clicked();

private:
	Ui::MainWindow *ui;
	TuringMachine machine;
	QList<QLineEdit*> tape;
    QList<QString> tape_backup;
	QPushButton *addCellButton[20];
	int startingCell;
    int startingCell_backup;
	bool debugStarted;
	int programTimer;

	void timerEvent(QTimerEvent *);
    void currentCellUpdate();
};

#endif // MAINWINDOW_H
