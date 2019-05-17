// qMemo/main.cpp - main
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


#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QTranslator>
#include "datahandler.hpp"

int main(int argc, char* argv[])
{
  // QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QGuiApplication app { argc, argv };


  QTranslator translator;
  translator.load(":/i18n/qmemo_" + QLocale::system().name());
  // translator.load(":i18n/qmemo_ja");
  // translator.load(":i18n/qmemo_hu");
  app.installTranslator(&translator);

  DataHandler dataHandler;

  if (!dataHandler.isAvailable()) {
    qFatal("Failed to create directories for application data!!");
    return 1;
  }

  QQmlApplicationEngine engine;
  QQmlContext* ctxt { engine.rootContext() };
  ctxt->setContextProperty("dataHandler", &dataHandler);
  engine.load(QUrl(QLatin1String("qrc:/resources/main.qml")));

  return app.exec();
}
