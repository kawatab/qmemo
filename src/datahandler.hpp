// qMemo/datahandler.hpp - Main application class
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


#pragma once


#include <QDir>
#include <QObject>
#include <QUrl>
#include "fileinfomodel.hpp"

class QByteArray;
class QFile;
class QStringList;


class DataHandler : public QObject {
  Q_OBJECT
  Q_PROPERTY(QAbstractListModel* currentFileList READ currentFileList NOTIFY fileListSwitched)
  Q_PROPERTY(bool isEditable READ isEditable WRITE setIsEditable NOTIFY isEditableChanged)
  Q_PROPERTY(bool isActiveMode READ isActiveMode WRITE setActiveMode)

public:
  DataHandler();
  ~DataHandler() = default;
  DataHandler(const DataHandler& other) = delete;
  DataHandler& operator=(const DataHandler& other) = delete;
  DataHandler(const DataHandler&& other) = delete;
  DataHandler& operator=(const DataHandler&& other) = delete;

  bool isAvailable() const;

  Q_INVOKABLE bool createNewFile();
  Q_INVOKABLE int deleteEmptyFile();
  Q_INVOKABLE bool hasCurrentFile() const;
  Q_INVOKABLE QString loadCurrentFile() const;
  Q_INVOKABLE void moveCurrentFile(int index);
  Q_INVOKABLE void releaseCurrentFile();
  Q_INVOKABLE bool saveCurrentFile(const QString& text) const;
  Q_INVOKABLE void saveFileBeforeClosing(const QString& text);
  Q_INVOKABLE void selectFile(int index);

signals:
  void fileListSwitched();
  void isEditableChanged();

private:
  QUrl createFile() const;
  QUrl currentFile() const;
  QAbstractListModel* currentFileList() const;
  bool deleteFile(const QUrl& path) const;
  QString getLastModifiedDate(const QUrl& path) const;
  QString getPreviewOfContents(const QUrl& path) const;
  bool isActiveMode() const;
  bool isEditable() const;
  bool loadFile(QFile* file, QStringList* contents, int maxLength, QString(*func)(const QByteArray&)) const;
  QUrl moveCurrentFile(const QUrl& url) const;
  bool saveFile(const QUrl& path, const QString& lines) const;
  void setActiveMode(bool b);
  void setCurrentFile(const QUrl& url);
  void setCurrentFileList(QAbstractListModel* model);
  QDir setDirectory(QDir path, const QString& name);
  void setFileList(const QDir& dir, FileInfoModel* list);
  void setIsEditable(bool b);
  void updateFileInfo(const QUrl& url) const;

  static QString withTrim(const QByteArray& byteArray);
  static QString withoutTrim(const QByteArray& byteArray);

  QDir mWorkDirectory;
  QDir mArchiveDirectory;
  QUrl mCurrentFile;
  FileInfoModel* mCurrentFileList;
  FileInfoModel mActiveFileList;
  FileInfoModel mArchiveFileList;
  bool mIsEditable;

  static const QString TIMESTAMP_PATTERN;
  static const QString DEFAULT_DIRECTORY;
  static const QString ARCHIVE_DIRECTORY;
};
