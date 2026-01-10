#include "SoundManager.h"

SoundManager& SoundManager::instance()
{
    static SoundManager instance;
    return instance;
}

SoundManager::SoundManager(QObject* parent)
    : QObject(parent)
{
    audio = new QAudioOutput(this);
    audio->setVolume(0.3f);

    music = new QMediaPlayer(this);
    music->setAudioOutput(audio);
    music->setSource(QUrl("qrc:/sounds/background.mp3"));
    music->setLoops(QMediaPlayer::Infinite);
}

void SoundManager::playMusic()
{
    if (music->playbackState() != QMediaPlayer::PlayingState)
        music->play();
}

void SoundManager::stopMusic()
{
    music->stop();
}

void SoundManager::setVolume(float v)
{
    audio->setVolume(v);
}
