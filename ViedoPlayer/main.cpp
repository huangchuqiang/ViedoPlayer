// #include "viedoplayer.h"
// #include <QtGui/QApplication>
// 
// int main(int argc, char *argv[])
// {
// 	QApplication a(argc, argv);
// 	ViedoPlayer w;
// 	w.show();
// 	return a.exec();
// }
/*
#include <QtGui/QApplication>
#include <QFileDialog>
#include <Phonon/VideoPlayer>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Phonon::VideoPlayer *player = new Phonon::VideoPlayer(Phonon::VideoCategory, NULL);
	player->show();
	QString fileName = QFileDialog::getOpenFileName();
	player->load(Phonon::MediaSource(fileName));
	player->play();
	return a.exec();
}
#include <QApplication>
#include <QWidget>
#include <phonon>
#include <QUrl>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFileDialog>
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QWidget *widget = new QWidget;
	widget->setWindowTitle("Media Player");
	widget->resize(400,400);
	QString fileName = QFileDialog::getOpenFileName();
	Phonon::MediaObject *media = new Phonon::MediaObject;
	media->setCurrentSource(Phonon::MediaSource(fileName));
	Phonon::VideoWidget *vwidget = new Phonon::VideoWidget(widget);
	Phonon::createPath(media, vwidget);
	vwidget->setAspectRatio(Phonon::VideoWidget::AspectRatioAuto);
	Phonon::AudioOutput *aOutput = new Phonon::AudioOutput(Phonon::VideoCategory);
	Phonon::createPath(media, aOutput);
	QLabel *label = new QLabel("Volume: ");
	Phonon::VolumeSlider *volumeSlider = new Phonon::VolumeSlider;
	volumeSlider->setAudioOutput(aOutput);
	volumeSlider->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	Phonon::SeekSlider *seekSlider = new Phonon::SeekSlider;
	seekSlider->setMediaObject(media);
	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(label);
	hLayout->addWidget(volumeSlider);
	hLayout->addStretch();
	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->addWidget(vwidget);
	vLayout->addWidget(seekSlider);
	vLayout->addLayout(hLayout);
	widget->setLayout(vLayout);
	widget->show();
	media->play();
	return app.exec();
}
*/
#include "qtMplayer.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
	QtMPlayer mplayer;
	mplayer.show();
	return app.exec();
}
