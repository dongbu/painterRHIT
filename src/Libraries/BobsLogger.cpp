#include "stdafx.h"

class BobsLogger : public QPlainTextEdit
{
	Q_OBJECT
public:
	void appendMessage(const QString& text);

private:
	QFile m_logFile;
};


void BobsLogger::appendMessage(const QString& text)
{
	this->appendPlainText(text); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); // Scrolls to the bottom
	m_logFile.write(text); // Logs to file
}