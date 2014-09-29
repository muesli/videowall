/*
 *   Copyright 2014, Christian Muehlhaeuser <muesli@gmail.com>
 */

#include "WallWindow.h"

#include <QtCore/qmath.h>


WallWindow::WallWindow( unsigned int maxVideos )
    : QGraphicsView()
{
    setWindowTitle( "VideoWall" );
    setFrameShape( QFrame::NoFrame );
    setBackgroundBrush( Qt::black );

    setScene( &m_scene );

    setRenderHints( QPainter::Antialiasing | QPainter::SmoothPixmapTransform );
    setCacheMode( QGraphicsView::CacheBackground );
    setViewportUpdateMode( QGraphicsView::BoundingRectViewportUpdate );

    setupProxies( maxVideos );
}


void
WallWindow::setupProxies( unsigned int count )
{
    for ( int i = 0; i < (int)count; i++ )
    {
        VideoProxy* proxy = new VideoProxy( 0, Qt::Widget );
        connect( proxy, SIGNAL( requestFocus() ), SLOT( onFocusRequested() ) );
        connect( proxy, SIGNAL( requestFullPlayback() ), SLOT( onFullPlaybackRequested() ) );
        connect( proxy, SIGNAL( rightClicked() ), SLOT( onVideoFinished() ) );
        connect( proxy, SIGNAL( videoFinished() ), SLOT( onVideoFinished() ) );

        proxy->show();

        m_scene.addItem( proxy );
        m_videos << proxy;
    }
}


void
WallWindow::setMedia( const QFileInfoList& files )
{
    m_files = files;

    foreach( VideoProxy* proxy, m_videos )
    {
        if ( m_files.count() > 0 )
        {
            proxy->loadMedia( m_files.takeAt( qrand() % m_files.count() ) );
        }
    }
}


void
WallWindow::onVideoFinished()
{
    qDebug() << Q_FUNC_INFO;

    VideoProxy* proxy = static_cast<VideoProxy*>( sender() );

    if ( m_files.count() > 0 )
    {
        proxy->loadMedia( m_files.takeAt( qrand() % m_files.count() ) );
    }
}


void
WallWindow::onFocusRequested()
{
    qDebug() << Q_FUNC_INFO;

    VideoProxy* proxy = static_cast<VideoProxy*>( sender() );
    bool zoom = false;
    if ( proxy->zValue() < 99 )
        zoom = true;

    layoutItems( false );

    if ( zoom )
    {
        scene()->setActiveWindow( proxy );

        QRect r( width() * 0.125, height() * 0.125, ( width() / 4 ) * 3, ( height() / 4 ) * 3  );
        proxy->animatedResize( r );
    }
}


void
WallWindow::onFullPlaybackRequested()
{
    qDebug() << Q_FUNC_INFO;

    VideoProxy* proxy = static_cast<VideoProxy*>( sender() );
    {
        proxy->animatedResize( rect() );
        scene()->setActiveWindow( proxy );
    }
}


void
WallWindow::resizeEvent( QResizeEvent* event )
{
    QGraphicsView::resizeEvent( event );
    layoutItems();
}


void
WallWindow::layoutItems( bool touchZValue )
{
    int r = 1;
    int c = 1;

    const int rows = qCeil( qSqrt( m_videos.count() ) );
    const int columns = rows;

    foreach( VideoProxy* proxy, m_videos )
    {
        if ( c > columns )
        {
            c = 1;
            r++;
        }

        QRect finRect( ( width() / columns ) * ( c-1 ), ( height() / rows ) * ( r-1 ),
                       ( width() / columns ),           ( height() / rows ) );
        proxy->animatedResize( finRect );

        if ( touchZValue )
            proxy->setZValue( 0 );

        c++;
    }

    m_scene.setSceneRect( rect() );
}
