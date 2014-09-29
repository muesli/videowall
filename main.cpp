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
        filter << "*.avi" << "*.mpg" << "*.mpeg" << "*.mov" << "*.mp4" << "*.flv" << "*.mkv" << "*.wmv";

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

    qsrand( QTime::currentTime().msec() );

    if ( argc > 1 )
    {
        unsigned int max = 9;
        if ( argc > 2 )
        {
            bool ok = false;
            int m = app.arguments().at( 2 ).toInt( &ok );
            if ( ok && m > 0 )
                max = m;
        }

        WallWindow ww( max );
        QFileInfoList files = scanPath( QDir( app.arguments().at( 1 ) ) );
        ww.setMedia( files );
        ww.show();

        return app.exec();
    }

    return 0;
}
