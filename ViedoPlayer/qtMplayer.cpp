#include "QtMplayer.h"
#include <QtGui>

QtMPlayer::QtMPlayer(QWidget *parent, Qt::WFlags flags)
	: QDialog(parent, flags)
{
	setWindowTitle("Qt MPlayer");
	m_speed = 1;
	m_bMute = 0;
	m_bFrameStep = 0;

	videoWidget = new QWidget;
	videoWidget->setFixedSize(500, 500);
	videoWidget->setVisible(true);
	videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	videoWidget->setStyleSheet("background-color:#999;");
	videoWidget->setAttribute(Qt::WA_OpaquePaintEvent);

	secondVideoWidget = new QWidget;
	secondVideoWidget->setFixedSize(500, 500);
	secondVideoWidget->setVisible(true);
	secondVideoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	secondVideoWidget->setStyleSheet("background-color:#999;");
	secondVideoWidget->setAttribute(Qt::WA_OpaquePaintEvent);

	QSize buttonSize(34, 28);

	openButton = new QPushButton(this);
	openButton->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

	playButton = new QPushButton(this);
	playIcon = style()->standardIcon(QStyle::SP_MediaPlay);
	pauseIcon = style()->standardIcon(QStyle::SP_MediaPause);
	playButton->setIcon(playIcon);

	stopButton = new QPushButton(this);
	stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));

	slowSpeedButton = new QPushButton(this);
	slowSpeedButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));

	highSpeedButton = new QPushButton(this);
	highSpeedButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

	frameStepButton = new QPushButton(this);
	frameStepButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));

	muteButton = new QPushButton(this);
	muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

	openButton->setMinimumSize(buttonSize);
	playButton->setMinimumSize(buttonSize);
	stopButton->setMinimumSize(buttonSize);
	slowSpeedButton->setMinimumSize(buttonSize);
	highSpeedButton->setMinimumSize(buttonSize);
	frameStepButton->setMinimumSize(buttonSize);
	muteButton->setMinimumSize(buttonSize);

	openButton->setToolTip(tr("打开"));
	playButton->setToolTip(tr("暂停/播放"));
	stopButton->setToolTip(tr("停止"));
	slowSpeedButton->setToolTip(tr("慢速播放"));
	highSpeedButton->setToolTip(tr("快速播放"));
	frameStepButton->setToolTip(tr("单帧播放"));
	muteButton->setToolTip(tr("静音"));

	volumeSlider = new QSlider(Qt::Horizontal);
	volumeSlider->setFixedWidth(100);
	volumeSlider->setRange(0,50);
	volumeSlider->setPageStep(2);

	seekSlider = new QSlider(Qt::Horizontal);
	//seekSlider->setRange(0,100);
	seekSlider->setPageStep(1);

	timeLabel = new QLabel(this);
	timeLabel->setMinimumWidth(60);
	QHBoxLayout *timeLayout = new QHBoxLayout;
	timeLayout->addWidget(seekSlider);
	timeLayout->addWidget(timeLabel);

	outEdit = new QTextEdit(this);
	outEdit->setReadOnly(true);

	playButton->setEnabled(false);
	stopButton->setEnabled(false);
	slowSpeedButton->setEnabled(false);
	highSpeedButton->setEnabled(false);
	frameStepButton->setEnabled(false);
	muteButton->setEnabled(false);
	volumeSlider->setEnabled(false);

	QHBoxLayout *btnsLayout = new QHBoxLayout;
	btnsLayout->addWidget(openButton);
	btnsLayout->addWidget(playButton);
	btnsLayout->addWidget(stopButton);
	btnsLayout->addWidget(slowSpeedButton);
	btnsLayout->addWidget(highSpeedButton);
	btnsLayout->addWidget(frameStepButton);
	btnsLayout->addStretch();
	btnsLayout->addWidget(muteButton);
	btnsLayout->addWidget(volumeSlider);

	poller = new QTimer(this);

	process = new QProcess;	
	process->setProcessChannelMode(QProcess::MergedChannels);
	connect(process, SIGNAL(readyReadStandardOutput()), this, SLOT(messageProcessing()));
	//当进程退出时会发出这个信号，包括了退出时的退出码和退出状态
	connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(mplayerEnded(int, QProcess::ExitStatus))); 


	secondProcess = new QProcess;	
	secondProcess->setProcessChannelMode(QProcess::MergedChannels);
	connect(secondProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(messageProcessing()));
	//当进程退出时会发出这个信号，包括了退出时的退出码和退出状态
	connect(secondProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(mplayerEnded(int, QProcess::ExitStatus))); 

	//每隔1S中定时器poller就会发出timeout信号，在响应函数中我们继续给Mplayer发送获取视频当前时间的命令get_time_pos
	connect(poller, SIGNAL(timeout()), this, SLOT(pollCurrentTime()));
	//当我们拖动滑动条时，视频也应该跟着播放
	connect(seekSlider, SIGNAL(sliderMoved(int)), this, SLOT(seekSliderChanged(int)));

	connect(openButton, SIGNAL(clicked()), this, SLOT(openFile()));
	connect(playButton, SIGNAL(clicked()), this, SLOT(playOrPause()));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stopPlay()));
	connect(slowSpeedButton, SIGNAL(clicked()), this, SLOT(slowSpeed()));
	connect(highSpeedButton, SIGNAL(clicked()), this, SLOT(highSpeed()));
	connect(frameStepButton, SIGNAL(clicked()), this, SLOT(frameStep()));
	connect(muteButton, SIGNAL(clicked()), this, SLOT(mute()));
	connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

	QVBoxLayout *mainLayout = new QVBoxLayout;
//	mainLayout->addWidget(videoWidget);
	mainLayout->addLayout(timeLayout);
	mainLayout->addLayout(btnsLayout);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addLayout(mainLayout);
	layout->addWidget(outEdit);

	setLayout(layout);
	setMinimumSize(QSize(800, 400));
}

void QtMPlayer::openFile()
{
	m_fileName = QFileDialog::getOpenFileName(this, "", "C:\\Users\\kingsoft\\Desktop\\");
	
	if (m_fileName.isEmpty())
		return ;

	startPlay();
}

void QtMPlayer::startPlay()
{
	// for windows7 vista
//	QString common = "./mplayer/mplayer.exe -slave -quiet -vo direct3d -aspect 16:9 -zoom \"" + m_fileName + "\" -wid " + QString::number(reinterpret_cast<qlonglong>(videoWidget->winId())); 
	QString common = "./mplayer/mplayer.exe -slave -quiet -vo direct3d -aspect 16:9 -zoom \"" + m_fileName + "\" -wid " + QString::number(reinterpret_cast<qlonglong>(videoWidget->winId())); 

	QString secondCommon = "./mplayer/mplayer.exe -slave -quiet -vo direct3d -aspect 16:9 -zoom \"" + m_fileName + "\" -wid " + QString::number(reinterpret_cast<qlonglong>(secondVideoWidget->winId())); 



	m_volume = 10;
	volumeSlider->setValue(m_volume);

	// for XP
	//QString common = "./mplayer/mplayer.exe -slave -quiet -vo directx:noaccel " + m_fileName + " -wid " + QString::number((ulong)videoWidget->winId()); 
	process->start(common);
	secondProcess->start(secondCommon);

	if(!process->waitForStarted(100))
	{
		qDebug("Error:Open MPlayer failed!\n");
		return;
	}

	//retrieve basic information
	//process->write("get_video_resolution\n");
	process->write("get_time_length\n");
	poller->start(1000);

	playState = Playing;
	updateUi();
}

void QtMPlayer::frameStep()
{
	playState = Paused;
	process->write("get_time_pos\n");
	process->write("frame_step\n");
	secondProcess->write("get_time_pos\n");
	secondProcess->write("frame_step\n");
	m_bFrameStep = 1; // 单帧播放模式
	if (m_bFrameStep)
		poller->stop();
	updateUi();
}

void QtMPlayer::playOrPause()
{
	if (playState == Playing){
		playState = Paused;
		if (poller->isActive())
			poller->stop();
	}
	else if (playState == Paused){
		playState = Playing;
		if (!poller->isActive())
			poller->start();
	}
	else if (playState == Stopped)
	{
		playState = Playing;
		startPlay();
		return ;
	}

	updateUi();
	process->write("pause\n");
	secondProcess->write("pause\n");
}

void QtMPlayer::slowSpeed()
{
	process->write("speed_mult 0.5\n");
	secondProcess->write("speed_mult 0.5\n");
}

void QtMPlayer::highSpeed()
{
	process->write("speed_mult 2\n");
	secondProcess->write("speed_mult 2\n");
}

void QtMPlayer::mute()
{
	if (m_bMute)
	{
		muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
		m_bMute = 0;
		process->write("mute 0\n");
		secondProcess->write("mute 0\n");
	}
	else
	{
		muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
		m_bMute = 1;
		process->write("mute 0\n");
		secondProcess->write("mute 0\n");
	}
}

void QtMPlayer::setVolume(int volume)
{
	int gapVolume = volume - m_volume;
	m_volume = volume;
	process->write(QString("volume "+QString::number(gapVolume)+"\n").toUtf8());
	secondProcess->write(QString("volume "+QString::number(gapVolume)+"\n").toUtf8());
}

void QtMPlayer::pollCurrentTime()
{
	process->write("get_time_pos\n");
	secondProcess->write("get_time_pos\n");
}

QString QtMPlayer::convertToTime(int secs)
{
	int mins = secs / 60;
	int hours = mins / 60;

	QTime time(hours, mins%60, secs%60);

	QString timeFormat("mm:ss");
	if (hours)
	{
		timeFormat = "hh:mm:ss";
	}
	return time.toString(timeFormat);
}

void QtMPlayer::messageProcessing()
{
	while (process->canReadLine())
	{
		QString buffer(process->readLine());
		outEdit->append(buffer);
		if(buffer.startsWith("ANS_LENGTH"))
		{
			buffer.remove(0, 11); // vire ANS_LENGTH=
			m_maxTime = buffer.toFloat();

			m_stotalTime = convertToTime(static_cast<int>(m_maxTime));
			seekSlider->setMaximum(static_cast<int>(m_maxTime));
		}
		//response to get_time_pos:ANS_TIME_POSITION=xx.y
		else if(buffer.startsWith("ANS_TIME_POSITION"))
		{
			buffer.remove(0, 18); // vire ANS_TIME_POSITION=
			m_curTime = buffer.toFloat();
			seekSlider->setValue(static_cast<int>(m_curTime));
			if (m_curTime <= m_maxTime){
				m_scurTime = convertToTime(static_cast<int>(m_curTime));
				timeLabel->setText(m_scurTime+"/"+m_stotalTime);
			}
		}
	}
}

void QtMPlayer::seekSliderChanged(int pos)
{
	process->write(QString("seek "+QString::number(pos)+" 2\n").toUtf8());
	secondProcess->write(QString("seek "+QString::number(pos)+" 2\n").toUtf8());
}

void QtMPlayer::stopPlay()
{
	process->write("quit\n");
	secondProcess->write("quit\n");
	if (process->waitForFinished(100))
	{
		qDebug("Error:Failed to waiting mplayer finish\n");
		return ;
	}
	playState = Stopped;
	updateUi();
}

void QtMPlayer::mplayerEnded(int exitCode, QProcess::ExitStatus exitStatus)
{
	playState = Stopped;
	updateUi();
	poller->stop();
}

void QtMPlayer::updateUi()
{
	if (playState == Playing)
	{
		playButton->setEnabled(true);
		playButton->setIcon(pauseIcon);
		stopButton->setEnabled(true);
		slowSpeedButton->setEnabled(true);
		highSpeedButton->setEnabled(true);
		frameStepButton->setEnabled(true);
		muteButton->setEnabled(true);
		volumeSlider->setEnabled(true);
	}
	else if (playState == Paused)
	{
		playButton->setIcon(playIcon);
	}
	else if (playState == Stopped)
	{
		playButton->setEnabled(true);
		playButton->setIcon(playIcon);
		stopButton->setEnabled(false);
		slowSpeedButton->setEnabled(false);
		highSpeedButton->setEnabled(false);
		frameStepButton->setEnabled(false);
		muteButton->setEnabled(false);
		volumeSlider->setEnabled(false);
	}
}

void QtMPlayer::closeEvent(QCloseEvent *event)
{
	stopPlay();
	delete videoWidget;
	delete secondVideoWidget;
	delete process;
	delete secondProcess;
	close();
}

QtMPlayer::~QtMPlayer()
{

}
