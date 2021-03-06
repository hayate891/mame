// license:BSD-3-Clause
// copyright-holders:Curt Coder
/**********************************************************************

    GLA 24K RAM cartridge emulation

**********************************************************************/

#pragma once

#ifndef __CBM2_24K_CARTRIDGE__
#define __CBM2_24K_CARTRIDGE__

#include "exp.h"



//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> cbm2_24k_cartridge_device

class cbm2_24k_cartridge_device : public device_t,
									public device_cbm2_expansion_card_interface
{
public:
	// construction/destruction
	cbm2_24k_cartridge_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
	// device-level overrides
	virtual void device_start() override;

	// device_cbm2_expansion_card_interface overrides
	virtual uint8_t cbm2_bd_r(address_space &space, offs_t offset, uint8_t data, int csbank1, int csbank2, int csbank3) override;
	virtual void cbm2_bd_w(address_space &space, offs_t offset, uint8_t data, int csbank1, int csbank2, int csbank3) override;

	optional_shared_ptr<uint8_t> m_ram;
};


// device type definition
extern const device_type CBM2_24K;


#endif
