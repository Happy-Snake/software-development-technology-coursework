#include "sideBar.h"

#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>

#define action_height 120

SideBar::SideBar(QWidget *parent)
    : QWidget(parent), mCheckedAction(NULL), mOverAction(NULL) {
  setMouseTracking(true);
}

void SideBar::paintEvent(QPaintEvent *event) {
  QPainter p(this);
  QString newActionName;

  QFont fontText(p.font());
  fontText.setFamily("Helvetica Neue");
  p.setFont(fontText);

  int action_y = 0;
  p.fillRect(rect(), QColor(95,110,139));
  for (auto action : mActions) {

    QRect actionRect(0, action_y, event->rect().width(), action_height);

    p.setPen(QColor(255, 255, 255));
    QSize size = p.fontMetrics().size(Qt::TextSingleLine, action->text());
    QRect actionTextRect(QPoint(actionRect.width() / 2 - size.width() / 2,
                                actionRect.bottom() - size.height() - 5),
                         size);
    p.drawText(actionTextRect, Qt::AlignCenter, action->text());

    QRect actionIconRect(0, action_y + 10, actionRect.width(),
                         actionRect.height() - 2 * actionTextRect.height() - 10);

    if (action->isChecked() || action == mOverAction) {
        QIcon actionIcon("checked-" + _pathes.value(action->text()));
        actionIcon.paint(&p, actionIconRect);
        if (_actionName != _pathes.value(action->text()))
            newActionName = _pathes.value(action->text());

    } else {
        QIcon actionIcon(action->icon());
        actionIcon.paint(&p, actionIconRect);
    }

    action_y += actionRect.height();
  }

  _actionName = newActionName;
}

QSize SideBar::minimumSizeHint() const {
  return action_height * QSize(1, mActions.size());
}

QAction* SideBar::addAction(const QString &text, const QString &iconPath) {
  QAction *action = new QAction(QIcon(iconPath), text, this);
  action->setCheckable(true);
  mActions.push_back(action);
  _pathes.insert(text, iconPath);
  update();
  return action;
}

void SideBar::mousePressEvent(QMouseEvent *event) {
  QAction *tempAction = actionAt(event->pos());
  if (tempAction == NULL || tempAction->isChecked())
    return;
  if (mCheckedAction)
    mCheckedAction->setChecked(false);
  if (mOverAction == tempAction)
    mOverAction = NULL;
  mCheckedAction = tempAction;
  tempAction->setChecked(true);
  update();
  QWidget::mousePressEvent(event);
  emit this->actionChanged(_actionName);
}

void SideBar::mouseMoveEvent(QMouseEvent *event) {
  QAction *tempAction = actionAt(event->pos());
  if (tempAction == NULL) {
    mOverAction = NULL;
    update();
    return;
  }
  if (tempAction->isChecked() || mOverAction == tempAction)
    return;
  mOverAction = tempAction;
  update();
  QWidget::mouseMoveEvent(event);
}

void SideBar::leaveEvent(QEvent *event) {
  mOverAction = NULL;
  update();
  QWidget::leaveEvent(event);
}

QAction *SideBar::actionAt(const QPoint &at) {
  int action_y = 0;
  for (auto action : mActions) {
    QRect actionRect(0, action_y, rect().width(), action_height);
    if (actionRect.contains(at))
      return action;
    action_y += actionRect.height();
  }
  return NULL;
}

#undef action_height
