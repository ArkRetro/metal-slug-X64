/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x009f88.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_9f88(void)

{
  int iVar1;
  short *psVar2;
  short sVar3;
  ushort uVar4;
  int extraout_A1;
  int iVar5;
  int unaff_A5;
  int unaff_A6;

  uVar4 = (ushort)*(byte *)(unaff_A6 + 0x1d);
  if (uVar4 != 0) {
    iVar5 = unaff_A5 + -0x1716;
    do {
      uVar4 = uVar4 - 1;
      iVar1 = iVar5 + (short)((ushort)*(byte *)(unaff_A6 + 0x19 + (int)(short)uVar4) << 4);
      sVar3 = *(short *)(iVar1 + 8);
      if (((sVar3 != 0) && (sVar3 != -1)) &&
         (psVar2 = (short *)(iVar1 + 8), sVar3 = *psVar2 + -1, *psVar2 = sVar3, sVar3 == 0)) {
        candidate_9e34();
        iVar5 = extraout_A1;
      }
    } while (uVar4 != 0);
  }
  *(undefined1 *)(unaff_A6 + 0x1d) = 0;
  return;
}
