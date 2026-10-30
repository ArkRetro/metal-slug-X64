/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0ae2d2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_ae2d2(void)

{
  uint in_D0;
  ushort uVar1;
  short sVar2;
  int iVar3;
  int unaff_A6;

  if (*(short *)(unaff_A6 + 0x8e) != 0) {
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
  if ((in_D0 & 8) == 0) {
    if ((in_D0 & 4) == 0) {
      uVar1 = *(ushort *)(unaff_A6 + 0x80);
      if (uVar1 < 0x281) {
        if ((uVar1 < 0x280) && (uVar1 = uVar1 + 0x40, 0x280 < uVar1)) {
          uVar1 = 0x280;
        }
      }
      else {
        uVar1 = uVar1 - 0x40;
        if (uVar1 < 0x280) {
          uVar1 = 0x280;
        }
      }
      *(ushort *)(unaff_A6 + 0x80) = uVar1;
      *(undefined4 *)(unaff_A6 + 0x30) = 0;
    }
    else {
      uVar1 = *(ushort *)(unaff_A6 + 0x80);
      *(ushort *)(unaff_A6 + 0x80) = uVar1 - 0x80;
      if (uVar1 < 0x80) {
        *(undefined2 *)(unaff_A6 + 0x80) = 0;
      }
      *(undefined4 *)(unaff_A6 + 0x30) = 0xfffc0000;
    }
  }
  else {
    *(short *)(unaff_A6 + 0x80) = *(short *)(unaff_A6 + 0x80) + 0x80;
    if (0x4ff < *(ushort *)(unaff_A6 + 0x80)) {
      *(undefined2 *)(unaff_A6 + 0x80) = 0x480;
    }
    *(undefined4 *)(unaff_A6 + 0x30) = 0x40000;
  }
  if ((in_D0 & 1) == 0) {
    if ((in_D0 & 2) == 0) {
      uVar1 = *(ushort *)(unaff_A6 + 0x7e);
      if (uVar1 < 0x281) {
        if ((uVar1 < 0x280) && (uVar1 = uVar1 + 0x40, 0x280 < uVar1)) {
          uVar1 = 0x280;
        }
      }
      else {
        uVar1 = uVar1 - 0x40;
        if (uVar1 < 0x280) {
          uVar1 = 0x280;
        }
      }
      *(ushort *)(unaff_A6 + 0x7e) = uVar1;
      *(undefined4 *)(unaff_A6 + 0x34) = 0;
    }
    else {
      uVar1 = *(ushort *)(unaff_A6 + 0x7e);
      *(ushort *)(unaff_A6 + 0x7e) = uVar1 - 0x80;
      if (uVar1 < 0x80) {
        *(undefined2 *)(unaff_A6 + 0x7e) = 0;
      }
      *(undefined4 *)(unaff_A6 + 0x34) = 0xfffc0000;
    }
  }
  else {
    *(short *)(unaff_A6 + 0x7e) = *(short *)(unaff_A6 + 0x7e) + 0x80;
    if (0x4ff < *(ushort *)(unaff_A6 + 0x7e)) {
      *(undefined2 *)(unaff_A6 + 0x7e) = 0x480;
    }
    *(undefined4 *)(unaff_A6 + 0x34) = 0x40000;
  }
  return;
}
