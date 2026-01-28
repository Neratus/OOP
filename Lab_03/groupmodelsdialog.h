#ifndef GROUPMODELSDIALOG_H
#define GROUPMODELSDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QObject>

class GroupModelsDialog : public QDialog {
  Q_OBJECT

public:
  explicit GroupModelsDialog(
      const QVector<std::pair<std::size_t, QString>> &models,
      QWidget *parent = nullptr);

  std::vector<std::size_t> selectedModelIds() const;

private:
  QListWidget *listWidget;
};

#endif // GROUPMODELSDIALOG_H
