/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0261a4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_261a4(void)

{
  char cVar1;
  ushort uVar2;
  int iVar3;

  do {
    if (_DAT_0010c350 != (char *)0x0) {
      cVar1 = *_DAT_0010c350;
      if (cVar1 != -1) {
        _DAT_0010c350 = _DAT_0010c350 + 1;
        if ((0 < _DAT_0010c354) && (_DAT_0010c354 = _DAT_0010c354 + 1, 0xff < _DAT_0010c354)) {
          _DAT_0010c354 = 0x100;
        }
        DAT_0010c357 = cVar1;
        DAT_0010c356 = 1;
        return;
      }
    }
    uVar2 = candidate_29c6();
    if (_DAT_0010c354 == 0) {
      iVar3 = 0x27e8e;
    }
    else {
      if (_DAT_0010c354 < 0) {
        _DAT_0010c354 = 1;
      }
      iVar3 = 0x27e7e;
    }
    _DAT_0010c350 = *(char **)(iVar3 + (short)((uVar2 & 3) << 2));
  } while( true );
}
