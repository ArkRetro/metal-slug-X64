/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0a9cb6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_a9cb6(void)

{
  ushort uVar1;
  ushort uVar3;
  uint uVar2;
  byte bVar4;
  short sVar5;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int *extraout_A1;
  int *piVar6;
  int unaff_A6;

  candidate_29c6();
  uVar3 = candidate_29c6();
  sVar5 = 0;
  piVar6 = extraout_A1;
  do {
    if (*piVar6 == -1) {
      return;
    }
    if (*piVar6 == 0xa9788) {
      if (*(short *)(unaff_A6 + 0x76) == 0) {
        return;
      }
      sVar5 = sVar5 + 1;
      if (5 < sVar5) {
        return;
      }
      if (*(short *)(unaff_A6 + 0x76) < sVar5) {
        return;
      }
    }
    candidate_40bc();
    candidate_1acf2();
    uVar1 = (ushort)*(byte *)((int)piVar6 + (int)(short)((uVar3 & 3) + 4));
    *(undefined1 *)(extraout_A0 + 0x92) = *(undefined1 *)((short)uVar1 + 0xe6206);
    *(int *)(extraout_A0 + 0x8c) = (int)*(short *)((short)(uVar1 << 1) + 0xe61d4) << 8;
    uVar2 = candidate_29c6();
    *(int *)(extraout_A0_00 + 0x8c) =
         (uVar2 & 0x1fff) * 0x10 + -0x10000 + *(int *)(extraout_A0_00 + 0x8c);
    if (0x80000 < *(int *)(extraout_A0_00 + 0x8c)) {
      *(undefined4 *)(extraout_A0_00 + 0x8c) = 0x80000;
    }
    bVar4 = candidate_29c6();
    *(char *)(extraout_A0_01 + 0x92) = ((bVar4 & 7) - 4) + *(char *)(extraout_A0_01 + 0x92);
    candidate_a9e2e();
    piVar6 = piVar6 + 2;
  } while( true );
}
