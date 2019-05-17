// qMemo/datahandler.hpp - class for file loading/saving
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


#include "datahandler.hpp"

#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QTextStream>


const QString DataHandler::TIMESTAMP_PATTERN { "yyyy-MM-dd HH:mm:ss" };
const QString DataHandler::DEFAULT_DIRECTORY { ".memo" };
const QString DataHandler::ARCHIVE_DIRECTORY { "archive" };

DataHandler::DataHandler()
  : QObject(), mWorkDirectory(), mArchiveDirectory(),
    mCurrentFile(),
    mCurrentFileList(), mActiveFileList(), mArchiveFileList()
{
  mWorkDirectory = setDirectory(QDir::home(), DEFAULT_DIRECTORY);
  mArchiveDirectory = setDirectory(mWorkDirectory, ARCHIVE_DIRECTORY);
  setFileList(mWorkDirectory, &mActiveFileList);
  setFileList(mArchiveDirectory, &mArchiveFileList);
  mCurrentFileList.setSourceModel(&mActiveFileList);
  mCurrentFileList.sort(0, Qt::DescendingOrder);
}

QDir DataHandler::setDirectory(QDir path, const QString& name)
{
  if (path.absolutePath() != "" && !path.cd(name)) {
    qDebug("Create work directory.: DataHandler::setDirectory()");

    if (!path.mkdir(name) || !path.cd(name)) {
      qFatal("Tried to create directory, but failed.: DataHandler::setDirectory()");

      return QDir();
    }
  }

  return path;
}

void DataHandler::setFileList(const QDir& dir, FileInfoModel* list)
{
  for (auto fileInfo : dir.entryInfoList(QDir::Files, QDir::Time)) {
    QUrl url { QUrl::fromLocalFile(fileInfo.filePath()) };
    QString modified { getLastModifiedDate(url) };
    QString preview { getPreviewOfContents(url) };
    list->appendItem(url, modified, preview);
  }
}

bool DataHandler::isAvailable() const
{
  return !mWorkDirectory.absolutePath().isEmpty() && !mArchiveDirectory.absolutePath().isEmpty();
}

bool DataHandler::isActiveMode() const
{
  return mCurrentFileList.sourceModel() == &mActiveFileList;
}

void DataHandler::setActiveMode(bool b)
{
  setCurrentFileList(b ? &mActiveFileList : &mArchiveFileList);
}

QUrl DataHandler::currentFile() const
{
  return mCurrentFile;
}

void DataHandler::setCurrentFile(const QUrl& url)
{
  mCurrentFile = url;
}

bool DataHandler::hasCurrentFile() const
{
  return !currentFile().toLocalFile().isEmpty();
}

void DataHandler::releaseCurrentFile()
{
  setCurrentFile(QUrl());
}

QAbstractItemModel* DataHandler::currentFileList()
{
  return &mCurrentFileList;
}

void DataHandler::setCurrentFileList(QAbstractItemModel* model)
{
  mCurrentFileList.setSourceModel(model);
  emit fileListSwitched();
  setIsEditable(mCurrentFileList.sourceModel() == &mActiveFileList);
}

int DataHandler::mapFromSource(int sourceIndex) const
{
  auto model { mCurrentFileList.sourceModel()->index(sourceIndex, 0) };
  return mCurrentFileList.mapFromSource(model).row();
}

int DataHandler::mapToSource(int proxyIndex) const
{
  auto model { mCurrentFileList.index(proxyIndex, 0) };
  return mCurrentFileList.mapToSource(model).row();
}

bool DataHandler::isEditable() const
{
  return mIsEditable;
}

void DataHandler::setIsEditable(bool b)
{
  mIsEditable = b;
  emit isEditableChanged();
}

int DataHandler::createNewFile(const QString& text)
{
  QUrl newFile { createFile() };

  if (newFile.toLocalFile().isEmpty()) {
    qCritical("Failed to create new file: DataHandler::createNewFile()");
    return -1;
  } else {
    if (!text.isEmpty()) {
      saveFile(newFile, text);
    }
    
    auto currentList { static_cast<FileInfoModel*>(mCurrentFileList.sourceModel()) };
    currentList->appendItem(newFile, getLastModifiedDate(newFile), getPreviewOfContents(newFile));
    qDebug("Created a new file successfully: DataHandler::createNewFile()");
    return mapFromSource(currentList->rowCount() - 1);
  }
}

QString DataHandler::getLastModifiedDate(const QUrl& path) const
{
  QFileInfo fileInfo { path.toLocalFile() };

  return fileInfo.lastModified().toString(TIMESTAMP_PATTERN);
}

QString DataHandler::getPreviewOfContents(const QUrl& path) const
{
  static const int MAX_LENGTH_OF_PREVIEW { 300 };
  QFile file { path.toLocalFile() };
  QStringList lines;

  if (file.exists()) {
    if (loadFile(&file, &lines, MAX_LENGTH_OF_PREVIEW, DataHandler::withTrim)) {
      return lines.join(' ').left(MAX_LENGTH_OF_PREVIEW);
    }
  }

  return "";
}

QUrl DataHandler::createFile() const
{
  QString name { QString::number(QDateTime::currentMSecsSinceEpoch()) + ".txt" };

  if (mWorkDirectory.absolutePath().isEmpty()) {
    qFatal("Cannot find \".memo\": DataHandler::createFile()");
    return QUrl();
  } else {
    QFileInfo fileInfo { mWorkDirectory, name };
    auto path { QUrl::fromLocalFile(fileInfo.filePath()) };
    QFile file { path.toLocalFile() };
    return file.open(QIODevice::WriteOnly) ? path : QUrl();
  }
}

QString DataHandler::loadCurrentFile() const
{
  static const int MAX_LENGTH_OF_TEXT { 0xffffff };

  QFile file { currentFile().toLocalFile() };
  QStringList contents;
  loadFile(&file, &contents, MAX_LENGTH_OF_TEXT, withoutTrim);
  return contents.join("");
}

bool DataHandler::loadFile(QFile* file, QStringList* contents, int maxLength, QString (*func)(const QByteArray&)) const
{
  if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
    qCritical("file wasn't loaded: DataHandler::loadFile()");
    return false;
  }

  while (!file->atEnd()) {
    auto line { func(file->readLine(maxLength)) };

    if (contents->length() + line.length() > maxLength) break;

    contents->append(line);
  }

  return true;
}

QString DataHandler::withoutTrim(const QByteArray& byteArray)
{
  return byteArray;
}

QString DataHandler::withTrim(const QByteArray& byteArray)
{
  return QString::fromUtf8(byteArray).trimmed();
}

bool DataHandler::matchCurrentFile(int index)
{
  int sourceIndex { mapToSource(index) };
  return mCurrentFileList.sourceModel()->data(mCurrentFileList.sourceModel()->index(sourceIndex, 0), FileInfoModel::FileURLRole).toUrl() == mCurrentFile;
}

void DataHandler::selectFile(int index)
{
  int sourceIndex { mapToSource(index) };
  if (sourceIndex < 0) {
    releaseCurrentFile();
  } else {
    setCurrentFile(mCurrentFileList.sourceModel()->data(mCurrentFileList.sourceModel()->index(sourceIndex, 0), FileInfoModel::FileURLRole).toUrl());
  }
}

int DataHandler::saveCurrentFile(const QString& text, int index) const
{
  int sourceIndex { mapToSource(index) };

  if (!hasCurrentFile()) {
    qDebug("no file to save: DataHandler::saveCurrentFile()");
    return -1;
  } else if (saveFile(currentFile(), text)) {
    updateFileInfo(currentFile());
    qDebug("Saved successfully: DataHandler::saveCurrentFile()");
    return mapFromSource(sourceIndex);
  } else {
    qCritical("Failed to save: DataHandler::saveCurrentFile()");
    return -1;
  }
}

bool DataHandler::saveFile(const QUrl& path, const QString& lines) const
{
  QFile file { path.toLocalFile() };

  if (!file.exists() ||
      !file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  QTextStream out { &file };
  out << lines;

  return true;
}

int DataHandler::deleteEmptyFile(int index)
{
  if (hasCurrentFile()) {
    int currentIndex { mapToSource(index) };
    QUrl path { currentFile() };

    if (deleteFile(currentFile())) {
      releaseCurrentFile();
      QModelIndex previous { static_cast<FileInfoModel*>(mCurrentFileList.sourceModel())->removeItem(path) };

      if (previous.isValid()) {
	qDebug("Deleted empty file successfully: DataHandler::deleteEmptyFile()");
	return mapFromSource(currentIndex > previous.row() ? currentIndex - 1 : currentIndex);
      }
    }
  }

  qCritical("Failed to delete empty file: DataHandler::deleteEmptyFile()");
  return -1;
}

bool DataHandler::deleteFile(const QUrl& path) const
{
  QFile file { path.toLocalFile() };

  return file.exists() && file.remove();
}

void DataHandler::updateFileInfo(const QUrl& url) const
{
  static_cast<FileInfoModel*>(mCurrentFileList.sourceModel())->modifyItem(url, getLastModifiedDate(url), getPreviewOfContents(url));
}

void DataHandler::moveCurrentFile(int index)
{
  auto currentList { static_cast<FileInfoModel*>(mCurrentFileList.sourceModel()) };
  QModelIndex proxyIndex { currentList->index(mapToSource(index), 0) };
  QUrl url { currentList->get(proxyIndex, "fileURL").toUrl() };
  FileInfoModel* otherFileList { currentList == &mActiveFileList ? &mArchiveFileList : &mActiveFileList };
		
  if (url == currentFile()) {
    QUrl newUrl { moveCurrentFile(url) };
    otherFileList->appendItem(newUrl, getLastModifiedDate(newUrl), getPreviewOfContents(newUrl));
    releaseCurrentFile();
    currentList->removeItem(url); // invoke onCurrentIndexChanged()
    qDebug("Moved successfully: DataHandler::moveCurrentFile()");
  } else {
    qCritical("Failed to move: DataHandler::moveCurrentFile()");
  }
}

QUrl DataHandler::moveCurrentFile(const QUrl& url) const
{
  QFileInfo file { url.toLocalFile() };
  auto dir { (file.path() == mWorkDirectory.path()) ? mArchiveDirectory : mWorkDirectory };

  QFileInfo newPath { dir, file.fileName() };
  QFile oldPath { file.filePath() };
  oldPath.rename(newPath.filePath());

  return QUrl::fromLocalFile(newPath.filePath());
}
