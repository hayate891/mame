// license:BSD-3-Clause
// copyright-holders:Antoine Mine
/**********************************************************************

  Copyright (C) Antoine Mine' 2006

  Philips MEA 8000 emulation.

**********************************************************************/

#ifndef __MEA8000_H__
#define __MEA8000_H__

#define MCFG_MEA8000_REQ_CALLBACK(_write) \
	devcb = &mea8000_device::set_reqwr_callback(*device, DEVCB_##_write);

/* table amplitude [-QUANT,QUANT] */
#define QUANT 512

/* filter coefficients from frequencies */
#define TABLE_LEN 3600

/* noise generator table */
#define NOISE_LEN 8192


/* finite machine state controlling frames */
enum mea8000_state
{
	MEA8000_STOPPED,    /* nothing to do, timer disabled */
	MEA8000_WAIT_FIRST, /* received pitch, wait for first full trame, timer disabled */
	MEA8000_STARTED,    /* playing a frame, timer on */
	MEA8000_SLOWING     /* repeating last frame with decreasing amplitude, timer on */
};

ALLOW_SAVE_TYPE( mea8000_state );


struct filter_t
{
#ifdef FLOAT_MODE
	double fm, last_fm;         /* frequency, in Hz */
	double bw, last_bw;         /* band-width, in Hz */
	double output, last_output; /* filter state */
#else
	uint16_t fm, last_fm;
	uint16_t bw, last_bw;
	int32_t  output, last_output;
#endif
};

class mea8000_device : public device_t,
	public device_sound_interface
{
public:
	mea8000_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);
	~mea8000_device() {}

	template<class _Object> static devcb_base &set_req_wr_callback(device_t &device, _Object object) { return downcast<mea8000_device &>(device).m_write_req.set_callback(object); }

	DECLARE_READ8_MEMBER(read);
	DECLARE_WRITE8_MEMBER(write);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

private:
	// internal state

	int accept_byte();
	void update_req();
	void init_tables();
#ifndef FLOAT_MODE /* uint16_t version */
	int interp(uint16_t org, uint16_t dst);
	int filter_step(int i, int input);
	int noise_gen();
	int freq_gen();
	int compute_sample();
#else /* float version */
	double interp(double org, double dst);
	double filter_step(int i, double input);
	double noise_gen();
	double freq_gen();
	double compute_sample();
#endif

	void shift_frame();
	void decode_frame();
	void start_frame();
	void stop_frame();

	TIMER_CALLBACK_MEMBER(timer_expire);

	devcb_write8 m_write_req;

	/* state */
	mea8000_state m_state; /* current state */

	uint8_t m_buf[4]; /* store 4 consecutive data to form a frame info */
	uint8_t m_bufpos; /* new byte to write in frame info buffer */

	uint8_t m_cont; /* if no data 0=stop 1=repeat last frame */
	uint8_t m_roe;  /* enable req output, now unimplemented */

	uint16_t m_framelength;  /* in samples */
	uint16_t m_framepos;     /* in samples */
	uint16_t m_framelog;     /* log2 of framelength */

	int16_t m_lastsample, m_sample; /* output samples are interpolated */

	uint32_t m_phi; /* absolute phase for frequency / noise generator */

	filter_t m_f[4]; /* filters */

	uint16_t m_last_ampl, m_ampl;    /* amplitude * 1000 */
	uint16_t m_last_pitch, m_pitch;  /* pitch of sawtooth signal, in Hz */
	uint8_t  m_noise;

	emu_timer *m_timer;
	sound_stream * m_stream;
	stream_sample_t m_output;

	int m_cos_table[TABLE_LEN];  /* fm => cos coefficient */
	int m_exp_table[TABLE_LEN];  /* bw => exp coefficient */
	int m_exp2_table[TABLE_LEN]; /* bw => 2*exp coefficient */
	int m_noise_table[NOISE_LEN];

};

extern const device_type MEA8000;


#endif
