// license:BSD-3-Clause
// copyright-holders:hap
/***************************************************************************

  APF Mathemagician
  * TMS1100 MP1030
  
***************************************************************************/

#include "emu.h"
#include "cpu/tms0980/tms0980.h"

#include "mathmagi.lh"

// master clock is a single stage RC oscillator: R=68K, C=82pf,
// according to the TMS 1000 series data manual this is around 200kHz
#define MASTER_CLOCK (200000)


class mathmagi_state : public driver_device
{
public:
	mathmagi_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu")
	{ }

	required_device<cpu_device> m_maincpu;

	UINT16 m_o;
	UINT16 m_r;

	DECLARE_READ8_MEMBER(read_k);
	DECLARE_WRITE16_MEMBER(write_o);
	DECLARE_WRITE16_MEMBER(write_r);

	virtual void machine_start();
};


/***************************************************************************

  I/O

***************************************************************************/

READ8_MEMBER(mathmagi_state::read_k)
{
	return 0;
}

WRITE16_MEMBER(mathmagi_state::write_o)
{
}

WRITE16_MEMBER(mathmagi_state::write_r)
{
}



/***************************************************************************

  Inputs

***************************************************************************/

/* physical button layout and labels is like this:

    ON     ONE       [SEL] [NXT] [?]   [/]
     |      |        [7]   [8]   [9]   [x]
    OFF    TWO       [4]   [5]   [6]   [-]
         PLAYERS     [1]   [2]   [3]   [+]
                     [0]   [_]   [r]   [=]
*/

static INPUT_PORTS_START( mathmagi )
INPUT_PORTS_END



/***************************************************************************

  Machine Config

***************************************************************************/

void mathmagi_state::machine_start()
{
	m_o = 0;
	m_r = 0;

	save_item(NAME(m_o));
	save_item(NAME(m_r));
}


static const UINT16 mathmagi_output_pla[0x20] =
{
	/* O output PLA configuration currently unknown */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
	0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
};


static MACHINE_CONFIG_START( mathmagi, mathmagi_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", TMS1100, MASTER_CLOCK)
	MCFG_TMS1XXX_OUTPUT_PLA(mathmagi_output_pla)
	MCFG_TMS1XXX_READ_K_CB(READ8(mathmagi_state, read_k))
	MCFG_TMS1XXX_WRITE_O_CB(WRITE16(mathmagi_state, write_o))
	MCFG_TMS1XXX_WRITE_R_CB(WRITE16(mathmagi_state, write_r))

	MCFG_DEFAULT_LAYOUT(layout_mathmagi)

	/* no video! */

	/* no sound! */
MACHINE_CONFIG_END



/***************************************************************************

  Game driver(s)

***************************************************************************/

ROM_START( mathmagi )
	ROM_REGION( 0x800, "maincpu", 0 )
	ROM_LOAD( "mp1030", 0x0000, 0x800, CRC(a81d7ccb) SHA1(4756ce42f1ea28ce5fe6498312f8306f10370969) )

	ROM_REGION( 867, "maincpu:mpla", 0 )
	ROM_LOAD( "tms1100_default_mpla.pla", 0, 867, BAD_DUMP CRC(62445fc9) SHA1(d6297f2a4bc7a870b76cc498d19dbb0ce7d69fec) ) // not verified
	ROM_REGION( 365, "maincpu:opla", 0 )
	ROM_LOAD( "tms1100_mathmagi_opla.pla", 0, 365, NO_DUMP )
ROM_END


COMP( 1980, mathmagi, 0, 0, mathmagi, mathmagi, driver_device, 0, "APF Electronics Inc.", "Mathemagician", GAME_SUPPORTS_SAVE | GAME_NO_SOUND_HW | GAME_NOT_WORKING )
