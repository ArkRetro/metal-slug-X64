/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c0ae6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c0ae6(void)

{
  short sVar1;
  ushort in_D0w;
  short in_D1w;
  ushort uVar2;
  int unaff_A5;

  if (*(byte *)(unaff_A5 + 0x471e + (int)(short)(in_D0w & 1)) < 0x10) {
    sVar1 = (in_D0w & 1) * 2;
    uVar2 = *(ushort *)(unaff_A5 + 0x4724 + (int)sVar1);
    if (uVar2 < 999) {
      uVar2 = uVar2 + in_D1w;
      if (999 < uVar2) {
        uVar2 = 999;
      }
      *(ushort *)(unaff_A5 + 0x4724 + (int)sVar1) = uVar2;
      candidate_120f8();
      return;
    }
  }
  return;
}
