/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x081f7c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_81f7c(void)

{
  ushort uVar1;
  undefined2 uVar3;
  short sVar4;
  uint uVar2;
  ushort uVar5;
  int unaff_A6;

  uVar1 = *(ushort *)(unaff_A6 + 0x92) - 1;
  *(ushort *)(unaff_A6 + 0x92) = uVar1;
  if ((int)((uint)uVar1 << 0x10) < 0) {
    uVar3 = candidate_81fd6();
    *(undefined2 *)(unaff_A6 + 0x90) = uVar3;
    sVar4 = candidate_29f2();
    *(short *)(unaff_A6 + 0x92) = sVar4 + 0x3c;
  }
  else {
    uVar1 = *(short *)(unaff_A6 + 0x90) - *(short *)(unaff_A6 + 0x3c);
    uVar5 = (ushort)-((int)((uint)uVar1 << 0x10) < 0);
    uVar2 = (int)((uint)(ushort)((uVar5 ^ uVar1) - uVar5) << 0x10) >> 3;
    if (*(int *)(unaff_A6 + 0x6e) < (int)uVar2) {
      uVar2 = *(uint *)(unaff_A6 + 0x6e);
    }
    *(int *)(unaff_A6 + 0x30) =
         ((int)((((int)(short)uVar5 ^ uVar2) - (int)(short)uVar5) - *(int *)(unaff_A6 + 0x30)) >> 4)
         + *(int *)(unaff_A6 + 0x30);
  }
  return;
}
