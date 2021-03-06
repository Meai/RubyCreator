#include "RubyIndenter.h"
#include "RubyScanner.h"

#include <texteditor/tabsettings.h>
#include <QRegExp>
#include <QSet>
#include <QDebug>

namespace Ruby {

static bool didBlockStart(const QTextBlock &block)
{
    QString text = block.text();
    Scanner scanner(&text);
    scanner.readLine();
    return scanner.indentVariation() > 0 || scanner.didBlockInterrupt();
}

void Indenter::indentBlock(QTextDocument*, const QTextBlock &block, const QChar &, const TextEditor::TabSettings &settings)
{
    bool isNewBlock = false;
    int indent = block.userState() >> 20;

    if (indent < 0) {
        QTextBlock previous = block.previous();
        while (indent == -1 && previous.isValid()) {
            indent = previous.userState() >> 20;
            previous = block.previous();
        }
        isNewBlock = true;
    }

    if (didBlockStart(block) && indent > 0)
        indent--;

    if (isNewBlock || !block.text().isEmpty())
        settings.indentLine(block, indent  *settings.m_indentSize);
}

}
