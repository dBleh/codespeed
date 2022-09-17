#include "mainwindow.h"
#include "qlabel.h"
#include "styledtextzone.h"
#include <qstring.h>
#include "typingzone.h"
#include <QVBoxLayout>
#include <QElapsedTimer>

StyledTextZone::StyledTextZone(QWidget* parent, TypingZone& tZone) : QLabel(parent), tZone_(&tZone)
{
};

QString StyledTextZone::getText() const{
	return masterText_;
}

void StyledTextZone::setNewText(QString& text) {
	masterText_ = text;
	QLabel::setText(text);
	
	QString reversedText = text;
	int i = 1;
	std::reverse(reversedText.begin(), reversedText.end());
	for (const QChar& c: reversedText) {
		SmartChar sChar;
		sChar.char_ = c;
		sChar.pos_ = text.size() - i;
		textToType_.push(sChar);
		i++;
	}

	emit onNewText();
}

void StyledTextZone::onUserTyped() {
	QString uText = tZone_->toPlainText();
	if (uText.isEmpty()) return;

	int uTextLen = uText.length();
	if (!textToType_.isEmpty()) {
		if ((uText.at(uTextLen-1) != textToType_.top().char_) || uTextLen - 1 != textToType_.top().pos_) {
			badChars_.push(uText.at(uTextLen-1));
			emit badCharsChanged(!badChars_.isEmpty());
			updateStyle();
		}
		else {
			textToType_.pop();
			updateStyle();
			if (textToType_.isEmpty()) {
				emit finished();
			}
		}
	}
}

void StyledTextZone::removeBadChar() {
	if (badChars_.isEmpty()) return;
	badChars_.pop();

	updateStyle();

	emit badCharsChanged(!badChars_.isEmpty());
}

void StyledTextZone::updateStyle() {

    setStyleSheet("QLabel b { background-color : green; }");

	const int numCorrectChars = masterText_.length() - textToType_.size();
	const int numBadChars = badChars_.size();
	const int numChars = masterText_.length();

	QString correctText = masterText_.first(numCorrectChars);
	QString badText = (masterText_.first(numBadChars + numCorrectChars)).last(numBadChars);
	QString normalText = masterText_.last(numChars - (numCorrectChars + numBadChars));

	// This white - space: pre styling is magic and makes sure everything looks right
	QString coloredText = "<span style=\"white-space: pre;\"><span style=\"background-color: rgba(76,175,80,0.5);\" line-height=\"300\">" + correctText + "</span>" + "<span style=\"background-color:rgba(255,82,82, 0.5);\">" + badText + "</span>" + normalText + "</span>";

	setText(coloredText);
}


