/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae260.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ulonglong candidate_ae260(void)

{
  short in_D0w;
  uint uVar1;
  int unaff_A6;
  undefined1 uVar2;
  short sStack_6;

  uVar2 = 0;
  candidate_aa994();
  candidate_18f24();
  if ((bool)uVar2) {
    return (ulonglong)(uint)(int)sStack_6;
  }
  uVar1 = (uint)sStack_6;
  if ((DAT_0010f000 & 0x80) == 0) {
    if ((*(byte *)(unaff_A6 + 0x5f) & 0x80) != 0) {
      return CONCAT44(1,uVar1);
    }
  }
  else {
    *(byte *)(unaff_A6 + 0x5f) = *(byte *)(unaff_A6 + 0x5f) & 0x7f;
  }
  if ((*(char *)(unaff_A6 + 0x99) != '\0') &&
     (uVar1 = CONCAT31((int3)(char)((ushort)sStack_6 >> 8),(char)sStack_6) & 0xffffff22,
     (char)uVar1 == '\"')) {
    return (ulonglong)uVar1;
  }
  return CONCAT44((int)in_D0w,uVar1);
}
