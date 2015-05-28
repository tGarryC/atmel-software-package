/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2013, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */


/*----------------------------------------------------------------------------
 *        Headers
 *----------------------------------------------------------------------------*/

#include "chip.h"
#include "av/isc.h"
 
/*----------------------------------------------------------------------------
 *        Local functions
 *----------------------------------------------------------------------------*/
 

/*----------------------------------------------------------------------------
 *        Export functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Send Capture Input Stream Command to start a single shot capture or a 
 * multiple frame.
 */
void ISC_StartCapture(void)
{
	ISC->ISC_CTRLEN = ISC_CTRLEN_CAPTURE;
}

/**
 * \brief end the capture at the next Vertical Synchronization Detection. 
 */
void ISC_StopCapture(void)
{
	ISC->ISC_CTRLDIS = ISC_CTRLDIS_DISABLE;
}

void ISC_StartCapture(void)
{
	ISC->ISC_CTRLEN = ISC_CTRLEN_CAPTURE;
}

/**
 * \brief Returns ISC Control Status.
 */
uint32_t ISC_GetCtrlStatus(void)
{
	return (ISC->ISC_CTRLSR);
}

/**
 * \brief update the color profile.
 */
void ISC_UpdateProfile(void)
{
	ISC->ISC_CTRLEN = ISC_CTRLEN_UPPRO;
}

/**
 * \brief update the histogram table.
 */
void ISC_UpdateHistogram(void)
{
	ISC->ISC_CTRLEN = ISC_CTRLEN_HISREQ;
}

/**
 * \brief  clear the histogram table.
 */
void ISC_ClearHistogram(void)
{
	ISC->ISC_CTRLEN = ISC_CTRLEN_HISCLR;
}

/**
 * \brief Perform software reset of the interface.
 */
void ISC_Reset(void)
{
	ISC->ISC_CTRLDIS = ISC_CTRLDIS_SWRST;
}

/**
 * \brief  configure PFE(Parallel Front End) cropping area.
 * \param Hstart: Horizontal starting position of the cropping area
 * \param Hend: Horizontal ending position of the cropping area
 * \param Vstart: Vertical starting position of the cropping area
 * \param Hend: Vertical ending position of the cropping area
 */
void ISC_PFE_SetCroppingArea( 
					uint32_t Hstart, uint32_t Hend, uint32_t Vstart, uint32_t Vend)
{
	ISC->ISC_PFE_CFG1 = ISC_PFE_CFG1_COLMIN(Hstart)
					 | ISC_PFE_CFG1_COLMAX(Hend)
					 | ISC_PFE_CFG2_ROWMIN(Vstart)
					 | ISC_PFE_CFG2_ROWMAX(Vend);
}

/**
 * \brief Configure the ISP clock.
 * \param ispClockDiv ISP Clock Divider.
 * \param ispClockSelection ISP Clock Selection. 
			0: HCLOCK is selected.
			1: GCK is selected.
 */
void ISC_ConfigureIspClock(uint32_t ispClockDiv, uint32_t ispClockSelection)
{
	ISC->ISC_CLKCFG |= ISC_CLKCFG_ICDIV(ispClockDiv) | (ispClockSelection << 8);
}

/**
 * \brief Enables the ISP clock.
 */
void ISC_EnableIspClock(void)
{
	ISC->ISC_CLKEN = ISC_CLKEN_ICEN;
}

/**
 * \brief Disables the ISP clock.
 */
void ISC_DisableIspClock(void)
{
	ISC->ISC_CLKDIS = ISC_CLKDIS_ICDIS;
}

/**
 * \brief Software reset the ISP clock.
 */
void ISC_ResetIspClock(void)
{
	ISC->ISC_CLKDIS = ISC_CLKDIS_ICSWRST;
}

/**
 * \brief Configure the Master clock.
 * \param masterClockDiv Master Clock Divider.
 * \param masterClockSelection Master Clock Selection. 
			0: HCLOCK is selected.
			1: GCK is selected.
			2: 480-MHz system clock is selected.
 */
void ISC_ConfigureMasterClock(uint32_t masterClockDiv, uint32_t masterClockSelection)
{
	ISC->ISC_CLKCFG |= ISC_CLKCFG_MCDIV(masterClockDiv) 
					| ISC_CLKCFG_MCSEL(masterClockSelection);
}

/**
 * \brief Enables the master clock.
 */
void ISC_EnableMasterClock(void)
{
	ISC->ISC_CLKEN = ISC_CLKEN_MCEN;
}

/**
 * \brief Disables the master clock.
 */
void ISC_DisableMasterClock(void)
{
	ISC->ISC_CLKDIS = ISC_CLKDIS_MCDIS;
}

/**
 * \brief Software reset the master clock.
 */
void ISC_ResetMasterClock(void)
{
	ISC->ISC_CLKDIS = ISC_CLKDIS_MCSWRST;
}

/**
 * \brief Returns ISC clock Status.
 */
uint32_t ISC_GetClockStatus(void)
{
	return (ISC->ISC_CLKSR);
}


/**
 * \brief Enable ISC interrupt
 * \param  flag of interrupt to enable
 */
void ISC_EnableInterrupt(uint32_t flag)
{
	ISC->ISC_INTEN = flag;
}

/**
 * \brief Disable ISC interrupt
 * \param  flag of interrupt to disable
 */
void ISC_DisableInterrupt(uint32_t flag)
{
	ISC->ISC_INTDIS = flag;
}

/**
 * \brief Return ISC status register
 * \return Status of ISC register
 */
uint32_t ISC_StatusRegister(void)
{
	return(ISC->ISC_INTSR);
}
