/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x00505a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

undefined8 candidate_505a(void)

{
  ushort uVar1;
  ushort uVar2;
  undefined4 in_D0;
  undefined4 in_D1;
  short *psVar3;
  int unaff_A5;
  int unaff_A6;

  uVar1 = *(short *)(unaff_A6 + 0x40) - 0x100;
  if (uVar1 < 0x100) {
    uVar1 = uVar1 >> 4 & 0xe;
    uVar2 = *(short *)(unaff_A6 + 0x3c) - 0x20;
    if (uVar2 < 0x200) {
      uVar2 = uVar2 >> 1 & 0xf0;
      psVar3 = (short *)((int)(short)uVar2 + (int)(short)uVar1 + unaff_A5 + -0x181a);
      *psVar3 = *psVar3 + 0x100;
      if (1 < uVar1) {
        psVar3[-1] = psVar3[-1] + 0xc0;
        if (0xf < uVar2) {
          psVar3[-9] = psVar3[-9] + 0x80;
        }
        if (uVar2 < 0xf0) {
          psVar3[7] = psVar3[7] + 0x80;
        }
      }
      if (0xf < uVar2) {
        psVar3[-8] = psVar3[-8] + 0xc0;
      }
      if (uVar2 < 0xf0) {
        psVar3[8] = psVar3[8] + 0xc0;
      }
      if (uVar1 < 0xe) {
        psVar3[1] = psVar3[1] + 0xc0;
        if (0xf < uVar2) {
          psVar3[-7] = psVar3[-7] + 0x80;
        }
        if (uVar2 < 0xf0) {
          psVar3[9] = psVar3[9] + 0x80;
        }
      }
    }
  }
  return CONCAT44(in_D0,in_D1);
}
