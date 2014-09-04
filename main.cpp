/*
 *   Copyright 2014, Christian Muehlhaeuser <muesli@gmail.com>
 */

#include "WallWindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QTime>


QFileInfoList
scanPath( const QDir& dir )
{
    QFileInfoList il;

    if ( dir.exists() )
    {
        QStringList filter;
        filter << "*.avi" << "*.mpg" << "*.mpeg" << "*.mov" << "*.mp4" << "*.flv" << "*.mkv";

        qDebug() << "Scanning:" << dir.absolutePath();
        il << dir.entryInfoList( filter, QDir::Files | QDir::NoDotAndDotDot );

        foreach ( const QFileInfo& f, il )
            qDebug() << "\tAdded:" << f.absoluteFilePath();

        QFileInfoList dirs = dir.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
        foreach ( const QFileInfo& d, dirs )
        {
            il << scanPath( d.absoluteFilePath() );
        }
    }

    return il;
}


int main( int argc, char* argv[] )
{
    Q_INIT_RESOURCE(videowall);

    QApplication app( argc, argv );
    WallWindow ww;

    qsrand( QTime::currentTime().msec() );

    if ( argc > 1 )
    {
        unsigned int max = 16;
        if ( argc > 2 )
        {
            bool ok = false;
            int m = app.arguments().at( 2 ).toInt( &ok );
            if ( ok && m > 0 )
                max = m;
        }

        QFileInfoList selectedFiles;
        QFileInfoList allFiles = scanPath( QDir( app.arguments().at( 1 ) ) );
        for ( unsigned int i = 0; i < max && allFiles.count() > 0; i++ )
        {
            selectedFiles << allFiles.takeAt( qrand() % allFiles.count() );
        }

        ww.loadMedia( selectedFiles );
        ww.show();
    }

    return app.exec();
}
