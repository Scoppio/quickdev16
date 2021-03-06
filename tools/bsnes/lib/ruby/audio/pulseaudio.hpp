/*
  audio.pulseaudio (2008-10-31)
  author: byuu
*/

class pAudioPulseAudio;

class AudioPulseAudio : public Audio {
public:
  bool cap(Setting);
  uintptr_t get(Setting);
  bool set(Setting, uintptr_t);

  void sample(uint16_t left, uint16_t right);
  bool init();
  void term();

  AudioPulseAudio();
  ~AudioPulseAudio();

private:
  pAudioPulseAudio &p;
};
