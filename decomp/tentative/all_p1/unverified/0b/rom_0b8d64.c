/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0b8d64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_b8d64(void)

{
  int iVar1;
  bool bVar2;

  iRam0010e94e = iRam0010e970;
  if (cRam0010c6bc == '\x01') {
    iRam0010c6b8 = iRam0010c6b8 + -0x1000;
    iVar1 = -0x20000;
  }
  else if (cRam0010c6bc == '\x02') {
    iRam0010c6b8 = iRam0010c6b8 + -0x2000;
    iVar1 = -0xc0000;
    iRam0010e94e = (iRam0010e970 >> 2) * 3;
  }
  else {
    iRam0010c6b8 = 0;
    iVar1 = 0;
  }
  if ((uRam0010e9d0 <= _DAT_0010e99e) || (iRam0010c6b8 < iVar1)) {
    iRam0010c6b8 = iVar1;
  }
  iRam0010e94e = iRam0010c6b8 + iRam0010e94e;
  if (_DAT_0010c6b6 != 0) {
    candidate_c6f6a();
    bVar2 = true;
    candidate_c6f7a();
    if (bVar2) {
      func_0x000c6f82();
      if (_DAT_0010c6b6 != -1) {
        _DAT_0010c6b6 = _DAT_0010c6b6 + -1;
      }
    }
  }
  return;
}
