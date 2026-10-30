/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x012244.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Removing unreachable block (ram,0x000120bc) */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_12244(void)

{
  ushort uVar1;

  uVar1 = _DAT_0010bfea;
  if (DAT_0010bfec != '\x06') {
    DAT_0010bfec = '\x06';
    *(undefined1 *)((short)_DAT_0010bfea + 0x10bf68) = 6;
    uVar1 = uVar1 + 1 & 0x7f;
    if (uVar1 == _DAT_0010bfe8) {
      DAT_0010bfec = 0xff;
      return;
    }
  }
  _DAT_0010bfea = uVar1;
  return;
}
