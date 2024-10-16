#ifndef DIRVIEW_H
#define DIRVIEW_H

#include <QMainWindow>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QSplitter>
#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QHash>
#include <QPushButton>


class CustomFileSystemModel : public QFileSystemModel
{
    Q_OBJECT

public:
    explicit CustomFileSystemModel(QObject* parent = nullptr) : QFileSystemModel(parent) {}
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    void resetFolderSizeCache();

private:
    mutable QHash<QString, qint64> folderSizeCache;
    qint64 calculateFolderSize(const QString& path) const;
};

class Dirview : public QMainWindow {
    Q_OBJECT

public:
    Dirview(QWidget* parent = nullptr);

private slots:
    void filterFiles(const QString& text);
    void updateFolderSizes() { model->resetFolderSizeCache(); }

private:
    CustomFileSystemModel* model;
    QLineEdit* filterLineEdit;
    QPushButton* updateButton;
    QHash<QString, qint64> folderSizeCache;
};


class AlignDelegate : public QStyledItemDelegate
{
public:
    explicit AlignDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

public:
    void initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const override;
};

#endif // DIRVIEW_H
