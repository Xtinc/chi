#pragma once
#include <QList>

namespace qdiffview
{
struct Diff;
}

class QDiffView
{
public:
    void setSource(const QString &oldString, const QString &newString);
    ~QDiffView();
private:
    QList<qdiffview::Diff *> _diffs;
    void _update();
};
