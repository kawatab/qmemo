// qMemo/fileinfoproxy.cpp - proxy class for file info list
// qMemo is a note taking application
//
//  Copyright (C) 2019  Yasuhiro Yamakawa <kawatab@yahoo.co.jp>
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


#include "fileinfoproxy.hpp"

#include <QtWidgets>
#include "fileinfomodel.hpp"


FileInfoProxy::FileInfoProxy(QObject* parent)
  : QSortFilterProxyModel(parent)
{
}

void FileInfoProxy::setFileInfoModel(FileInfoModel* model)
{
  QSortFilterProxyModel::setSourceModel(model);
}

FileInfoModel* FileInfoProxy::fileInfoModel() const
{
  return static_cast<FileInfoModel*>(QSortFilterProxyModel::sourceModel());
}

bool FileInfoProxy::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
  Q_UNUSED(sourceRow);
  Q_UNUSED(sourceParent);
  return true;
}

bool FileInfoProxy::lessThan(const QModelIndex& left, const QModelIndex& right) const
{
  QVariant leftData { sourceModel()->data(left, FileInfoModel::ModifiedRole) };
  QVariant rightData { sourceModel()->data(right, FileInfoModel::ModifiedRole) };

  QString leftString { leftData.toString() };
  QString rightString { rightData.toString() };

  return QString::localeAwareCompare(leftString, rightString) < 0;
}
