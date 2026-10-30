/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03517c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ulonglong candidate_3517c(void)

{
  short in_D0w;
  uint uVar1;
  int unaff_A6;
  undefined1 in_CF;
  ulonglong uVar2;
  undefined2 uStack_6;

  candidate_18f24();
  if ((bool)in_CF) {
    return (ulonglong)(uint)(int)uStack_6;
  }
  uVar1 = (uint)uStack_6;
  if ((((DAT_0010f000 & 0x80) == 0) && (*(short *)(unaff_A6 + 0x80) == 0)) &&
     ((*(byte *)(unaff_A6 + 0x5f) & 0x80) != 0)) {
    uVar2 = candidate_aa7fc();
    return uVar2;
  }
  if (((*(byte *)(unaff_A6 + 0x15) & 0x40) != 0) &&
     (uVar1 = CONCAT31((int3)(char)((ushort)uStack_6 >> 8),(char)uStack_6) & 0xffffff22,
     (char)uVar1 == '\"')) {
    uVar2 = candidate_aa7fc();
    return uVar2;
  }
  return CONCAT44((int)in_D0w,uVar1);
}
