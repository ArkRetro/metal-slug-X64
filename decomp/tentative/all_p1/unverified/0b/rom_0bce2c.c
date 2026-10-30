/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bce2c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_bce2c(void)

{
  int iVar1;
  int iVar2;
  short unaff_D4w;
  int extraout_A0;

  iRam0010e94e = iRam0010e970;
  if (cRam0010c70a == '\x01') {
    iVar1 = iRam0010c706 + -0x4000;
    iVar2 = -0x20000;
  }
  else if (cRam0010c70a == '\x02') {
    iVar1 = iRam0010c706 + -0x2000;
    iVar2 = -0x80000;
    iRam0010e94e = (iRam0010e970 >> 2) * 3;
  }
  else {
    iVar1 = 0;
    iVar2 = 0;
  }
  if (iVar2 < iVar1) {
    iRam0010c706 = iVar1;
  }
  iRam0010e94e = iRam0010c706 + iRam0010e94e;
  if ((_DAT_0010c704 != 0) &&
     (candidate_c6f6a(),
     -1 < (short)(*(short *)(extraout_A0 + 0x36) * 0x10 + 0x1f0 + unaff_D4w * -0x10))) {
    func_0x000c6f82();
    if (_DAT_0010c704 != -1) {
      _DAT_0010c704 = _DAT_0010c704 + -1;
    }
    uRam0010c70c = *(ushort *)(_DAT_0010c702 + 0xbce9c) & uRam0010c70c;
  }
  return;
}
