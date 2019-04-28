// qMemo/fileinfomodel.hpp - file list class
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


#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QStringList>
#include <QUrl>


struct PreviewItem
{
  QUrl fileURL;
  QString modified;
  QString preview;
};


class FileInfoModel : public QAbstractListModel {
  Q_OBJECT
  Q_ENUMS(PreviewRoles)
  Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
  Q_PROPERTY(bool dynamicRoles READ dynamicRoles)

public:
  enum PreviewRoles {
    FileURLRole = Qt::UserRole + 1,
    ModifiedRole,
    PreviewRole,
  };

  explicit FileInfoModel(QObject *parent = 0);

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;


  void appendItem(const QUrl& fileURL, const QString& modified, const QString& preview);
  bool dynamicRoles() const;
  QVariant get(int index, const QString& role) const;
  void modifyItem(const QUrl& fileURL, const QString& modified, const QString& preivew);
  void prependItem(const QUrl& fileURL, const QString& modified, const QString& preview);
  int removeItem(const QUrl& path);
  void sort();

signals:
  void countChanged();

protected:
  QHash<int, QByteArray> roleNames() const override;

private:
  QList<PreviewItem> mList;
};
