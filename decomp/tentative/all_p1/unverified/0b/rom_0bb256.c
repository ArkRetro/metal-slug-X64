/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bb256.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bb256(void)

{
  ushort uVar2;
  uint uVar1;
  short sVar3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  undefined2 uVar4;

  sVar3 = 0xf;
  uVar4 = 0;
  do {
    candidate_411c(sVar3,uVar4);
    candidate_1acf2();
    uVar2 = candidate_29c6();
    *(ushort *)(extraout_A0 + 0x34) = (uVar2 & 7) + 4;
    uVar2 = candidate_29c6();
    *(ushort *)(extraout_A0_00 + 0x32) = uVar2;
    *(ushort *)(extraout_A0_00 + 0x30) = (uVar2 & 7) + 4;
    uVar1 = candidate_29c6();
    if ((uVar1 & 1) != 0) {
      *(int *)(extraout_A0_01 + 0x30) = -*(int *)(extraout_A0_01 + 0x30);
    }
    uVar2 = candidate_29c6();
    *(ushort *)(extraout_A0_02 + 0x6e) = (uVar2 & 1) + 10;
    *(undefined4 *)(extraout_A0_02 + 0x70) = 0xbb0b6;
    *(undefined4 *)(extraout_A0_02 + 0x74) = 0xbb0e6;
    sVar3 = sVar3 + -1;
  } while (sVar3 != -1);
  return;
}
