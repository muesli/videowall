/*
 *   Copyright 2014, Christian Muehlhaeuser <muesli@gmail.com>
 */

#include "VideoProxy.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include <phonon/VideoWidget>


VideoProxy::VideoProxy( QGraphicsItem* parent, Qt::WindowFlags wFlags )
    : QGraphicsProxyWidget( parent, wFlags )
    , m_animation( 0 )
{
    // we never cache the video
    setCacheMode( NoCache );

    m_media = new Phonon::MediaObject();
    Phonon::VideoWidget* video = new Phonon::VideoWidget();
    m_audio = new Phonon::AudioOutput( Phonon::VideoCategory, this );
    Phonon::createPath( m_media, m_audio );
    Phonon::createPath( m_media, video );

    connect( m_media, SIGNAL( finished() ), SIGNAL( videoFinished() ) );

    setWidget( video );
}


QRectF
VideoProxy::boundingRect() const
{
    return QGraphicsProxyWidget::boundingRect().adjusted( 8, 8, -8, -8 );
}


void
VideoProxy::hoverEnterEvent( QGraphicsSceneHoverEvent* event )
{
    qDebug() << Q_FUNC_INFO << m_media->currentSource();
    QGraphicsProxyWidget::hoverEnterEvent( event );

    m_audio->setVolume( 0.9 );
}


void
VideoProxy::hoverLeaveEvent( QGraphicsSceneHoverEvent* event )
{
    qDebug() << Q_FUNC_INFO << m_media->currentSource();
    QGraphicsProxyWidget::hoverLeaveEvent( event );

    m_audio->setVolume( 0.0 );
}


void
VideoProxy::mousePressEvent( QGraphicsSceneMouseEvent* event )
{
    event->accept();
}


void
VideoProxy::mouseDoubleClickEvent( QGraphicsSceneMouseEvent* event )
{
    QGraphicsProxyWidget::mouseDoubleClickEvent( event );

    emit requestFullPlayback();
}


void
VideoProxy::mouseReleaseEvent( QGraphicsSceneMouseEvent* event )
{
    QGraphicsProxyWidget::mouseReleaseEvent( event );

    emit requestFocus();
}


void
VideoProxy::wheelEvent( QGraphicsSceneWheelEvent* event )
{
    if ( event->delta() > 0 )
    {
        m_media->seek( m_media->currentTime() + 15000 );
        emit forward();
    }
    else
    {
        m_media->seek( m_media->currentTime() - 20000 );
        emit rewind();
    }
}


void
VideoProxy::loadMedia( const QFileInfo& info )
{
    m_media->setCurrentSource( info.absoluteFilePath() );
    m_media->play();

    m_audio->setVolume( 0.0 );
}


void
VideoProxy::animatedResize( const QRect& finRect )
{
    if ( m_animation )
    {
        delete m_animation;
        m_animation = 0;
    }
    m_animation = new QPropertyAnimation( this, "geometry" );

    if ( finRect.size().width() > rect().size().width() ||
         finRect.size().height() > rect().size().height() )
    {
        setZValue( 99 );
    }
    else if ( finRect.size().width() < rect().size().width() ||
              finRect.size().height() < rect().size().height() )
    {
        setZValue( 50 );
        connect( m_animation, SIGNAL( finished() ), SLOT( resetZValue() ) );
    }
    else
    {
        // unchanged
        return;
    }

    m_animation->setEasingCurve( QEasingCurve::InOutQuad );
    m_animation->setDuration( 450 );
    m_animation->setStartValue( mapRectToScene( rect() ) );
    m_animation->setEndValue( finRect );

    m_animation->start();
}


void
VideoProxy::resetZValue()
{
    setZValue( 0 );
}
