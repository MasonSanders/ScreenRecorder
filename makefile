all: main.o
	g++ -std=c++20 main.o ScreenRecorder.o Observer.o MainWindow.o SettingsWindow.o SettingsFileManager.o Recorder.o MediaStream.o VideoStream.o ApplicationState.o NotRecordingState.o RecordingState.o SettingsState.o AbstractRecorderBtnFactory.o RecorderBtnFactory.o StartRecordButton.o StopRecordButton.o SettingsButton.o RecorderButton.o Subject.o -o Capstone `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
main.o: ScreenRecorder.o main.cpp
	g++ -std=c++20 -c main.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
ScreenRecorder.o: ScreenRecorder.h ScreenRecorder.cpp MainWindow.o SettingsWindow.o Observer.o Recorder.o ApplicationState.o NotRecordingState.o RecordingState.o SettingsState.o
	g++ -std=c++20 -c ScreenRecorder.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
Observer.o: Observer.h Observer.cpp
	g++ -std=c++20 -c Observer.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
MainWindow.o: MainWindow.h MainWindow.cpp Observer.o Subject.o
	g++ -std=c++20 -c MainWindow.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
SettingsWindow.o: SettingsWindow.h SettingsWindow.cpp Observer.o Subject.o SettingsFileManager.o
	g++ -std=c++20 -c SettingsWindow.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
SettingsFileManager.o: SettingsFileManager.h SettingsFileManager.cpp
	g++ -std=c++20 -c SettingsFileManager.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
Recorder.o: Recorder.h Recorder.cpp Observer.o Subject.o MediaStream.o VideoStream.o
	g++ -std=c++20 -w -c Recorder.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
MediaStream.o: MediaStream.h MediaStream.cpp
	g++ -std=c++20 -w -c MediaStream.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
VideoStream.o: VideoStream.h VideoStream.cpp MediaStream.o
	g++ -std=c++20 -w -c VideoStream.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
ApplicationState.o: ApplicationState.h ApplicationState.cpp Recorder.o MainWindow.o SettingsWindow.o
	g++ -std=c++20 -c ApplicationState.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
NotRecordingState.o: NotRecordingState.h NotRecordingState.cpp ApplicationState.o AbstractRecorderBtnFactory.o RecorderBtnFactory.o
	g++ -std=c++20 -c NotRecordingState.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
RecordingState.o: RecordingState.h RecordingState.cpp ApplicationState.o AbstractRecorderBtnFactory.o RecorderBtnFactory.o
	g++ -std=c++20 -c RecordingState.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
SettingsState.o: SettingsState.h SettingsState.cpp ApplicationState.o
	g++ -std=c++20 -c SettingsState.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
AbstractRecorderBtnFactory.o: AbstractRecorderBtnFactory.h AbstractRecorderBtnFactory.cpp
	g++ -std=c++20 -c AbstractRecorderBtnFactory.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
RecorderBtnFactory.o: RecorderBtnFactory.h RecorderBtnFactory.cpp AbstractRecorderBtnFactory.o StartRecordButton.o StopRecordButton.o SettingsButton.o
	g++ -std=c++20 -c RecorderBtnFactory.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
StartRecordButton.o: StartRecordButton.h StartRecordButton.cpp RecorderButton.o
	g++ -std=c++20 -c StartRecordButton.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
StopRecordButton.o: StopRecordButton.h StopRecordButton.cpp RecorderButton.o
	g++ -std=c++20 -c StopRecordButton.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
SettingsButton.o: SettingsButton.h SettingsButton.cpp RecorderButton.o
	g++ -std=c++20 -c SettingsButton.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
RecorderButton.o: RecorderButton.h RecorderButton.cpp
	g++ -std=c++20 -c RecorderButton.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
Subject.o: Subject.h Subject.cpp Observer.o
	g++ -std=c++20 -c Subject.cpp `pkg-config --cflags --libs gtkmm-3.0 libavcodec libavdevice libavfilter libavformat libavutil libswscale x11`
clean:
	rm *.o
	rm Capstone
	rm *.mp4

