#ifndef QT_MPLAYER_H
#define QT_MPLAYER_H

#include <QDialog>
#include <QIcon>
#include <QProcess>

class QPushButton;
class QSlider;
class QTextEdit;
class QTimer;
class QLabel;

class QtMPlayer : public QDialog
{
	Q_OBJECT

public:
	enum PlayState{Playing, Paused, Stopped};

	QtMPlayer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtMPlayer();

	void closeEvent(QCloseEvent *event);

private:
	QWidget *videoWidget;
	QWidget* secondVideoWidget;
	QPushButton *openButton;
	QPushButton *playButton;
	QIcon playIcon;
	QIcon pauseIcon;
	QPushButton *stopButton;
	QPushButton *slowSpeedButton;
	QPushButton *highSpeedButton;
	QPushButton *frameStepButton;
	QPushButton *muteButton;
	QSlider *volumeSlider;
	QSlider *seekSlider;
	QProcess *process;
	QProcess *secondProcess;
	QTimer *poller;
	QLabel *timeLabel;

	PlayState playState;
	QTextEdit *outEdit;
	float m_speed;
	bool m_bMute;
	bool m_bFrameStep;
	float m_maxTime;
	float m_curTime;
	QString m_fileName;
	int m_volume;

	QString m_stotalTime;
	QString m_scurTime;

	void updateUi();
	QString convertToTime(int secs);

private slots:
	void openFile();
	void startPlay();
	void stopPlay();
	void playOrPause();
	void slowSpeed();
	void highSpeed();
	void frameStep();
	void mute();
	void setVolume(int volume);
	void seekSliderChanged(int pos);
	void pollCurrentTime();
	void mplayerEnded(int exitCode, QProcess::ExitStatus exitStatus);
	void messageProcessing();
};

#endif // QT_MPLAYER_H
