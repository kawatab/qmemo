// qMemo/fileinfomodel.cpp - file list class
// qMemo is a note taking application
//
//  Copyright (C) 2019  Yasuhiro Yamakawa <kawatab@outlook.com>
//
//  This program is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or any
//  later version.
//
//  This program is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
//  License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include "fileinfomodel.hpp"


FileInfoModel::FileInfoModel(QObject *parent)
  : QAbstractListModel(parent),
    mList()
{
}

void FileInfoModel::appendItem(const QUrl& fileURL, const QString& modified, const QString& preview)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  mList.append(PreviewItem { fileURL.toString(), modified, preview } );
  endInsertRows();
}

void FileInfoModel::prependItem(const QUrl& fileURL, const QString& modified, const QString& preview)
{
  beginInsertRows(QModelIndex(), 0, 0);
  mList.prepend(PreviewItem { fileURL.toString(), modified, preview } );
  endInsertRows();
}

void FileInfoModel::modifyItem(const QUrl& fileURL, const QString& modified, const QString& preview)
{
  for (int i { 0 }; i < rowCount(); ++i) {
    PreviewItem& item { mList[i] };

    if (item.fileURL == fileURL) {
      item.modified = modified;
      item.preview = preview;
      emit dataChanged(index(i), index(i));
      break;
    }
  }
}

bool FileInfoModel::dynamicRoles() const
{
  return false;
}

QHash<int, QByteArray> FileInfoModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[FileURLRole] = "fileURL";
  roles[ModifiedRole] = "modified";
  roles[PreviewRole] = "preview";

  return roles;
}

int FileInfoModel::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);

  return mList.count();
}

QVariant FileInfoModel::data(const QModelIndex &index, int role) const
{
  int dataIndex { index.row() };

  if (dataIndex < 0 || dataIndex >= mList.count()) {
    return QVariant();
  }

  return
    role == FileURLRole ? QVariant(mList.at(dataIndex).fileURL) :
    role == ModifiedRole ? QVariant(mList.at(dataIndex).modified) :
    role == PreviewRole ? QVariant(mList.at(dataIndex).preview) :
    role == Qt::EditRole ? QVariant(16) :
    QVariant();
}

QModelIndex FileInfoModel::removeItem(const QUrl& path)
{
  QString pathString { path.toString() };

  for (int i { 0 }; i < mList.count(); ++i) {
    if (mList[i].fileURL == pathString) {
      auto index { this->index(i) };
      beginRemoveRows(QModelIndex(), i, i);
      mList.removeAt(i);
      endRemoveRows();
      return index;
    }
  }

  return QModelIndex();
}

QVariant FileInfoModel::get(const QModelIndex& index, const QString& role) const
{
  int dataIndex { index.row() };

  return
    (dataIndex < 0 || dataIndex >= mList.count()) ? QVariant() :
    role == "fileURL" ? QVariant(mList.at(dataIndex).fileURL) :
    role == "modified" ? QVariant(mList.at(dataIndex).modified) :
    role == "preview" ? QVariant(mList.at(dataIndex).preview) :
    QVariant();
}

Qt::ItemFlags FileInfoModel::flags(const QModelIndex &index) const
{
  // return Qt::ItemIsEnabled;
  return QAbstractListModel::flags(index);
}
