/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c8bec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c8bec(void)

{
  undefined2 uVar1;
  undefined4 uVar2;
  short sVar3;
  short sVar4;
  undefined4 *in_A0;
  int unaff_A5;
  int unaff_A6;

  uVar2 = *in_A0;
  sVar3 = (short)in_A0[1];
  if ((*(byte *)(unaff_A6 + 0x15) & 2) != 0) {
    uVar2 = CONCAT22((short)((uint)uVar2 >> 0x10),-(sVar3 + (short)uVar2));
  }
  if (*(int *)(unaff_A6 + 0x40) != *(int *)(unaff_A6 + 0x48)) {
    uVar1 = (undefined2)((uint)uVar2 >> 0x10);
    if (*(int *)(unaff_A6 + 0x40) - *(int *)(unaff_A6 + 0x48) < 0) {
      sVar3 = *(short *)(unaff_A6 + 0x40) + sVar3 + (short)uVar2;
      uVar2 = CONCAT22(uVar1,sVar3);
      if (sVar3 < *(short *)(unaff_A5 + 0x69d6)) {
        return;
      }
    }
    else {
      sVar3 = *(short *)(unaff_A6 + 0x40) + (short)uVar2;
      uVar2 = CONCAT22(uVar1,sVar3);
      if (*(short *)(unaff_A5 + 0x69d8) < sVar3) {
        return;
      }
    }
  }
  sVar3 = (short)((uint)uVar2 >> 0x10);
  sVar4 = (short)((uint)in_A0[1] >> 0x10);
  if ((*(byte *)(unaff_A6 + 0x15) & 1) != 0) {
    sVar3 = -(sVar4 + sVar3);
  }
  if (*(int *)(unaff_A6 + 0x3c) != *(int *)(unaff_A6 + 0x44)) {
    if (*(int *)(unaff_A6 + 0x3c) - *(int *)(unaff_A6 + 0x44) < 0) {
      if ((short)(*(short *)(unaff_A6 + 0x3c) + sVar4 + sVar3) < *(short *)(unaff_A5 + 0x69d2)) {
        return;
      }
    }
    else if (*(short *)(unaff_A5 + 0x69d4) < (short)(*(short *)(unaff_A6 + 0x3c) + sVar3)) {
      return;
    }
  }
  return;
}
