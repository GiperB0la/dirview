#include "dirview.h"


Dirview::Dirview(QWidget* parent) : QMainWindow(parent)
{
    model = new CustomFileSystemModel(this);
    model->setRootPath(QDir::rootPath());
    model->setReadOnly(false);
    model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);

    QTreeView* treeView = new QTreeView(this);
    treeView->setModel(model);
    treeView->setRootIndex(model->index(QDir::homePath()));

    treeView->setSortingEnabled(true);
    treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    treeView->header()->setSortIndicatorShown(true);

    AlignDelegate* alignDelegate = new AlignDelegate(this);
    treeView->setItemDelegate(alignDelegate);
    treeView->setColumnWidth(0, 300);

    filterLineEdit = new QLineEdit(this);
    filterLineEdit->setPlaceholderText("Filter files and folders");

    updateButton = new QPushButton("Update Folder Sizes", this);

    connect(filterLineEdit, &QLineEdit::textChanged, this, &Dirview::filterFiles);
    connect(updateButton, &QPushButton::clicked, this, &Dirview::updateFolderSizes);

    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->addWidget(updateButton);
    hLayout->addWidget(filterLineEdit);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addLayout(hLayout);
    layout->addWidget(treeView);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    setCentralWidget(centralWidget);
    setWindowTitle("Dirview");
    resize(1050, 580);
}

void AlignDelegate::initStyleOption(QStyleOptionViewItem* option, const QModelIndex& index) const
{
    QStyledItemDelegate::initStyleOption(option, index);
    option->displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
}

void Dirview::filterFiles(const QString& text)
{
    model->setNameFilters(QStringList() << "*" + text + "*");
    model->setNameFilterDisables(false);
}

QVariant CustomFileSystemModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole) {
        if (index.column() == 1 && isDir(index)) {
            QString dirPath = filePath(index);
            return calculateFolderSize(dirPath);
        }
    }
    return QFileSystemModel::data(index, role);
}

qint64 CustomFileSystemModel::calculateFolderSize(const QString& path) const
{
    if (folderSizeCache.contains(path)) {
        return folderSizeCache[path];
    }

    qint64 totalSize = 0;
    QDirIterator it(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFileInfo fileInfo(it.next());

        if (fileInfo.isFile() || fileInfo.isSymLink()) {
            totalSize += fileInfo.size();
        }
    }

    folderSizeCache.insert(path, totalSize);
    return totalSize;
}

void CustomFileSystemModel::resetFolderSizeCache()
{
    folderSizeCache.clear();
    emit dataChanged(index(0, 2), index(rowCount() - 1, 2));
}
