/*
 * Copyright (C) by Dominik Schmidt <dev@dominik-schmidt.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 */


#ifndef SOCKETAPI_H
#define SOCKETAPI_H

extern "C" {
#include <std/c_string.h>
}

#include <QWeakPointer>
#include <QTcpSocket>
#include <QTcpServer>
#include <QLocalServer>

#include "mirall/syncfileitem.h"

class QUrl;
class QLocalSocket;
class QStringList;

namespace Mirall {

//Define this to use the old school TCP API. Maybe we should offer both APIs
// and have the old TCP one be enableable via command line switch?
//#define SOCKETAPI_TCP

#ifdef SOCKETAPI_TCP
typedef QTcpSocket SocketType;
#else
typedef QLocalSocket SocketType;
#endif

class SocketApi : public QObject
{
Q_OBJECT

public:
    SocketApi(QObject* parent);
    virtual ~SocketApi();

public slots:
    void slotUpdateFolderView(const QString&);
    void slotUnregisterPath( const QString& alias );
    void slotRegisterPath( const QString& alias );
    void slotReadExcludes();
    void slotClearExcludesList();
private slots:
    void slotNewConnection();
    void onLostConnection();
    void slotReadSocket();
    void slotJobCompleted(const QString &, const SyncFileItem &);
    void slotSyncItemDiscovered(const QString &, const SyncFileItem &);

private:
    void sendMessage(SocketType* socket, const QString& message, bool doWait = false);
    void broadcastMessage(const QString& verb, const QString &path, const QString &status = QString::null, bool doWait = false);

    Q_INVOKABLE void command_RETRIEVE_FOLDER_STATUS(const QString& argument, SocketType* socket);
    Q_INVOKABLE void command_RETRIEVE_FILE_STATUS(const QString& argument, SocketType* socket);

    Q_INVOKABLE void command_VERSION(const QString& argument, SocketType* socket);

private:
#ifdef SOCKETAPI_TCP
    QTcpServer _localServer;
#else
    QLocalServer _localServer;
#endif
    QList<SocketType*> _listeners;
    c_strlist_t *_excludes;
};

}
#endif // SOCKETAPI_H
