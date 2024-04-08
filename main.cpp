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

const int MAX_SOUND = 30;

//struct Sound{
//	ma_sound sound{};
//	unsigned int chance = 30;
//	int init(ma_engine& engine, const char* fileName);
//	void uninit();
//};
//
//int Sound::init(ma_engine& engine, const char* fileName){
//	ma_sound_init_from_file(&engine, fileName, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &sound);
//};
//
//void Sound::unint() {
//	ma_sound
//}

struct AudioEngine{
	ma_engine engine{};
	ma_sound sounds[MAX_SOUND]{};
	int size = 0;
	bool loop = false;
	bool randomize = false;
	float volume = 1.0;
	int init();
	void uninit();
	bool addSoundFromFile(char* fileName);
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
	for (int i = 0; i < size; i++) {
		ma_sound_uninit(&sounds[i]);
	}
}

void AudioEngine::setVolume(float v) {
	volume = v;
}

float AudioEngine::getVolume() { // NOLINT(*-make-member-function-const)
	return volume;
}

bool AudioEngine::addSoundFromFile(char *fileName) {
	if (size == MAX_SOUND - 1) return false;
	ma_sound sound;
	ma_sound_init_from_file(&engine, fileName, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &sound);
	sounds[size++] = sound;
	return true;
}

int playSound(AudioEngine* engine, const char* filename) {
	ma_sound_group sound;
//	ma_sound_group_init();
	ma_sound_init_from_file(&engine->engine, filename, MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC, nullptr, nullptr, &sound);
//	ma_engine_play_sound(&engine, filename, &sound);
//	ma_sound_set_volume(&sound, 20.5);
	ma_sound_start(&sound);
	engine->isPlaying = true;
	float volume = engine->volume;

	ma_sound_set_volume(&sound, engine->volume);

	while (!ma_sound_get_at_end(&sound)) {
		if (!engine->isPlaying) {
			break;
		}
		if (volume != engine->volume) ma_sound_set_volume(&sound, engine->volume);
		Sleep(100);
//		cout << "Song " << filename << " is playing!\n";
//		ma_sleep(1000);
	}
	engine->isPlaying = false;
	ma_sound_uninit(&sound);
	return 1;
}

int main() {
	auto engine = AudioEngine();
	engine.init();

	std::thread musicThread(playSound, &engine, "menu1.mp3");
	musicThread.detach();
//	playSound(&engine, "music.mp3");


	Sleep(3000);

	while (true) {
		switch (getch()) {
			case 'w':
				if (engine.volume < 1.8) engine.volume += 0.2;
				else engine.volume = 2.0;
				cout << engine.volume << "\n";
				break;
			case 's':
				if (engine.volume > 0.2) engine.volume -= 0.2;
				else engine.volume = 0;
				cout << engine.volume << "\n";
				break;
			case 'a':
				cout << "Stopped!";
				engine.isPlaying = false;
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
