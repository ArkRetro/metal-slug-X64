/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae51c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ae51c(void)

{
  ushort uVar1;
  uint in_D0;
  short sVar2;
  int iVar3;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x8e) == 0) {
    if ((in_D0 & 8) == 0) {
      if ((in_D0 & 4) == 0) {
        *(undefined4 *)(unaff_A6 + 0x30) = 0;
      }
      else {
        *(undefined4 *)(unaff_A6 + 0x30) = 0xfffc0000;
      }
    }
    else {
      *(undefined4 *)(unaff_A6 + 0x30) = 0x40000;
    }
    if ((in_D0 & 1) == 0) {
      if ((in_D0 & 2) == 0) {
        uVar1 = *(ushort *)(unaff_A6 + 0x7e);
        *(ushort *)(unaff_A6 + 0x7e) = uVar1 - 0xe0;
        if (uVar1 < 0xe0) {
          *(undefined2 *)(unaff_A6 + 0x7e) = 0;
        }
        *(undefined4 *)(unaff_A6 + 0x34) = 0;
      }
      else {
        *(short *)(unaff_A6 + 0x7e) = *(short *)(unaff_A6 + 0x7e) + 0xe0;
        if (0x8ff < *(ushort *)(unaff_A6 + 0x7e)) {
          *(undefined2 *)(unaff_A6 + 0x7e) = 0x820;
        }
        *(undefined4 *)(unaff_A6 + 0x34) = 0xfffc0000;
      }
    }
    else {
      *(undefined4 *)(unaff_A6 + 0x34) = 0x40000;
    }
    return;
  }
  sVar2 = (*(byte *)(unaff_A6 + 0x5b) & 0x1f) << 3;
  if ((*(byte *)(unaff_A6 + 0x8f) & 1) == 0) {
    iVar3 = 0xae46c;
  }
  else {
    iVar3 = 0xae3ec;
  }
  *(undefined4 *)(unaff_A6 + 0x30) = *(undefined4 *)(iVar3 + sVar2);
  *(undefined4 *)(unaff_A6 + 0x34) = *(undefined4 *)(iVar3 + 4 + (int)sVar2);
  return;
}
