/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03cad2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ushort candidate_3cad2(void)

{
  ushort in_D0w;
  ushort uVar1;
  short sVar2;
  undefined1 *puVar3;

  uRam0010c35a = 0;
  uRam0010c35e = 0;
  uRam0010c360 = 0;
  _DAT_0010c364 = 0;
  if (in_D0w < 3) {
    uRam0010c382 = 1;
    _DAT_0010c376 = *(undefined1 **)((short)(in_D0w * 4) + 0x3cf06);
    uRam0010c37a = 0;
    uRam0010c37c = 0;
    _DAT_0010c374 = 0;
    uRam0010c37e = 0;
    uVar1 = _DAT_00000180;
    if (_DAT_00000180 != 0) {
      sVar2 = 0x1ff;
      puVar3 = _DAT_0010c376;
      do {
        *puVar3 = 0xff;
        sVar2 = sVar2 + -1;
        puVar3 = puVar3 + 1;
      } while (sVar2 != -1);
    }
    return uVar1;
  }
  uRam0010c382 = 0;
  return in_D0w;
}
