#include "groupmodelsdialog.h"
#include <QVBoxLayout>

GroupModelsDialog::GroupModelsDialog(
    const QVector<std::pair<std::size_t, QString>> &models, QWidget *parent)
    : QDialog(parent) {
  setWindowTitle("Выбор моделей для группировки");
  resize(300, 400);

  auto *layout = new QVBoxLayout(this);

  listWidget = new QListWidget(this);
  listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

  for (const auto &[id, name] : models) {
    QListWidgetItem *item = new QListWidgetItem(name, listWidget);
    item->setData(Qt::UserRole, static_cast<qulonglong>(id));
  }

  layout->addWidget(listWidget);

  auto *buttons = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  layout->addWidget(buttons);

  connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

std::vector<std::size_t> GroupModelsDialog::selectedModelIds() const {
  std::vector<std::size_t> result;
  for (QListWidgetItem *item : listWidget->selectedItems()) {
    result.push_back(item->data(Qt::UserRole).toULongLong());
  }
  return result;
}
