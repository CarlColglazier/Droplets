#include "mixer_droplet.h"

MixerDroplet::MixerDroplet(DaisyPatch* m_patch,
		       DropletState m_state) :
  Droplet(m_patch,
	  m_state) {
  switch (GetState()) {
  default:
  case DropletState::kFull:
    mix[0].Init(Patch()->controls[Patch()->CTRL_1],
		0.0, 1.0f, Parameter::LINEAR);
    mix[1].Init(Patch()->controls[Patch()->CTRL_2],
		0.0, 1.0f, Parameter::LINEAR);
    mix[2].Init(Patch()->controls[Patch()->CTRL_3],
		0.0, 1.0f, Parameter::LINEAR);
    mix[3].Init(Patch()->controls[Patch()->CTRL_4],
		0.0, 1.0f, Parameter::LINEAR);
    break;
  case DropletState::kLeft:
    mix[0].Init(Patch()->controls[Patch()->CTRL_1],
		0.0, 1.0f, Parameter::LINEAR);
    mix[1].Init(Patch()->controls[Patch()->CTRL_2],
		0.0, 1.0f, Parameter::LINEAR);
    break;
  case DropletState::kRight:
    mix[2].Init(Patch()->controls[Patch()->CTRL_3],
		0.0, 1.0f, Parameter::LINEAR);
    mix[3].Init(Patch()->controls[Patch()->CTRL_4],
		0.0, 1.0f, Parameter::LINEAR);
    break;
  }
}

MixerDroplet::~MixerDroplet() {}

void MixerDroplet::Control() {}

void MixerDroplet::Process(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) {
  Patch()->ProcessAnalogControls();

  float output = 0.0f;
  for (size_t i = 0; i < size; i++) {
    output = 0.0f;
    for (size_t chn = GetChannelMin(); chn < GetChannelMax(); chn++) {
      output += in[chn][i] * mix[chn].Process();
    }
    if (GetState() == DropletState::kFull) {
      output *= .25f;
    } else {
      output *= .5f;
    }
    for (size_t chn = GetChannelMin(); chn < GetChannelMax(); chn++) {
      out[chn][i]  = output;
    }
  }
}

void MixerDroplet::Draw() {
  int divider;
  switch (GetState()) {
  default:
  case DropletState::kFull:
    divider = (GetScreenMax()-GetScreenMin())/5.5;
    DrawFourDividedRectangles(Patch(),
			      divider,
			      GetScreenMin(),
			      GetScreenMax(),
			      GetTitleHeight()*(1.0f-mix[0].Process()),
			      GetTitleHeight()-1,
			      GetTitleHeight()*(1.0f-mix[1].Process()),
			      GetTitleHeight()-1,
			      GetTitleHeight()*(1.0f-mix[2].Process()),
			      GetTitleHeight()-1,
			      GetTitleHeight()*(1.0f-mix[3].Process()),
			      GetTitleHeight()-1);
    break;
  case DropletState::kLeft:
    divider = (GetScreenMax()-GetScreenMin())/4.5;
    DrawTwoDividedRectangles(Patch(),
			     divider,
			     GetScreenMin(),
			     GetScreenMax(),
			     GetTitleHeight()*(1.0f-mix[0].Process()),
			     GetTitleHeight()-1,
			     GetTitleHeight()*(1.0f-mix[1].Process()),
			     GetTitleHeight()-1);
    break;
  case DropletState::kRight:
    divider = (GetScreenMax()-GetScreenMin())/4.5;
    DrawTwoDividedRectangles(Patch(),
			     divider,
			     GetScreenMin(),
			     GetScreenMax(),
			     GetTitleHeight()*(1.0f-mix[2].Process()),
			     GetTitleHeight()-1,
			     GetTitleHeight()*(1.0f-mix[3].Process()),
			     GetTitleHeight()-1);
    break;
  }
  DrawName("Mixer");
}

void MixerDroplet::UpdateStateCallback() {}
