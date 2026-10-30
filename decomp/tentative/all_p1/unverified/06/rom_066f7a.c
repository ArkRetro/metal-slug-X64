/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x066f7a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_66f7a(void)

{
  short in_D0w;
  uint uVar1;
  uint uVar2;

  uVar1 = _DAT_0010c586;
  do {
    uVar2 = uVar1;
    uVar1 = uVar2 - 0x10;
    if (uVar1 < _DAT_0010c582) {
      return;
    }
  } while ((((*(byte *)(uVar2 - 1) & 1) != 0) ||
           (*(byte *)(uVar2 - 5) <=
            (byte)((char)((ushort)(in_D0w + 0x9000U) >> 5) - *(char *)(uVar2 - 7)))) ||
          (*(byte *)(uVar2 - 0xc) <=
           (byte)(((byte)(in_D0w + 0x9000U) & 0x1f) - *(char *)(uVar2 - 6))));
  return;
}
