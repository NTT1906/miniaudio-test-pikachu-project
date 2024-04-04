#include <iostream>
#include <conio.h>

#define MINIAUDIO_IMPLEMENTATION
#define MA_ENABLE_ONLY_SPECIFIC_BACKENDS
#define MA_ENABLE_WASAPI
#define MA_ENABLE_WINMM
#define MA_NO_FLAC

#include "miniaudio.h"
#include <thread>

using namespace std;

struct AudioEngine{
	ma_engine engine{};
	float volume = 1.0;
	int init();
	void uninit();
	void setVolume(float v);
	bool isPlaying = false;
	float getVolume();
};

int AudioEngine::init() {
	ma_result result;
	result = ma_engine_init(nullptr, &engine);
	if (result != MA_SUCCESS) {
		return -1;
	}
	return 1;
}

void AudioEngine::uninit() {
	ma_engine_uninit(&engine);
}

void AudioEngine::setVolume(float v) {
	volume = v;
}

float AudioEngine::getVolume() { // NOLINT(*-make-member-function-const)
	return volume;
}

int playSound(AudioEngine* engine, const char* filename) {
	ma_sound_group sound;
//	ma_sound_group_init();
	ma_sound_init_from_file(&engine->engine, filename, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &sound);
//	ma_engine_play_sound(&engine, filename, &sound);
//	ma_sound_set_volume(&sound, 20.5);
	ma_sound_start(&sound);

	ma_sound_set_volume(&sound, engine->volume);

	while (!ma_sound_get_at_end(&sound)) {
		ma_sound_set_volume(&sound, engine->volume);
		Sleep(100);
//		cout << "Song " << filename << " is playing!\n";
//		ma_sleep(1000);
	}
	ma_sound_uninit(&sound);
	return 1;
}

int main() {
	auto engine = AudioEngine();
	engine.init();

	std::thread musicThread(playSound, &engine, "menu1.mp3");
	musicThread.detach();

	Sleep(3000);

	while (true) {
		switch (getch()) {
			case 'w':
				if (engine.volume < 1.8)
				engine.volume += 0.2;
				break;
			case 's':
				if (engine.volume > 0.2)
				engine.volume -= 0.2;
				break;
		}
	}

//	while (true) {
//		playSound(&engine, "sound.mp3");
//		cout << "Loop!\n";
//		Sleep(3000);
//	}
	engine.uninit();
	return 0;
}