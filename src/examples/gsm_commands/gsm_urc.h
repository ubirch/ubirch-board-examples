#ifndef _GSM_URC_SIM800_H_
#define _GSM_URC_SIM800_H_

enum GSM_URC_CODES {
    CIPRXGET = "+CIPRXGET: 1,", // incoming socket data notification
    FTPGET = "+FTPGET: 1,", // FTP state change notification
    PDP_DEACT = "+PDP: DEACT",// PDP disconnected
    SAPBR_PDP_DEACT = "+SAPBR 1: DEACT", /*! PDP disconnected (for SAPBR apps) */
    PSNWID = "*PSNWID:", /*! ATCLTS network name */
    PSUTTZ = "*PSUTTZ:", /*! ATCLTS time */
    CTZV = "+CTZV:", /*! ATCLTS timezone */
    DST = "DST:", /*! ATCLTS dst information */
    CIEV = "+CIEV:", /*! ATCLTS undocumented indicator */
    RDY = "RDY",/*! Device ready. */
    CFUN = "+CFUN: 1",
    CPIN_READY = "+CPIN: READY",
    CALL_READY = "Call Ready",
    SMS_READY = "SMS Ready",
    NORMAL_POWER_DOWN = "NORMAL POWER DOWN",
    UNDER_VOLTAGE_POWER_DOWN = "UNDER-VOLTAGE POWER DOWN",
    UNDER_VOLTAGE_WARNNING = "UNDER-VOLTAGE WARNNING",
    OVER_VOLTAGE_POWER_DOWN = "OVER-VOLTAGE POWER DOWN",
    OVER_VOLTAGE_WARNNING = "OVER-VOLTAGE WARNNING",
};
// this useful list found here: https://github.com/cloudyourcar/attentive
#define UNSOLICTED_RESULT_CODES 19
const char *SIM800H_URC[UNSOLICTED_RESULT_CODES] = {
  CIPRXGET,
  FTPGET,
  PDP_DEACT,
  SAPBR_DEACT,
  PSNWID,
  PSUTTZ,
  CTZV,
  DST,
  CIEV,
  RDY,
  CFUN,       /*! Device has entered full functional mode */
  CPIN_READY,
  CALL_READY,
  SMS_READY,
  NORMAL_POWER_DOWN,
  UNDER_VOLTAGE_POWER_DOWN,
  UNDER_VOLTAGE_WARNNING,
  OVER_VOLTAGE_POWER_DOWN,
  OVER_VOLTAGE_WARNNING,
};

#endif // _GSM_URC_SIM800_H_
