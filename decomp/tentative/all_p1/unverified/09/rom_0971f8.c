/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0971f8.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_971f8(void)

{
  uint uVar1;
  uint uVar2;
  short sVar3;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  undefined4 *unaff_A6;
  undefined1 uVar4;

  candidate_aa7fc();
  *(undefined1 *)((int)unaff_A6 + 0x92) = 1;
  *(undefined2 *)(unaff_A6 + 0x23) = 0;
  unaff_A6[0xd] = 0x1a4000;
  candidate_411c();
  *(undefined4 *)(extraout_A0 + 0x72) = 0x29dcde;
  candidate_1acf2();
  candidate_40bc();
  candidate_1b220();
  *(undefined4 *)(extraout_A0_00 + 0x72) = 0x29daea;
  unaff_A6[0xc] = 0;
  candidate_966f0();
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x800;
  candidate_40bc();
  candidate_1b220();
  *(undefined4 *)(extraout_A0_01 + 0x72) = 0x29dc30;
  *(undefined1 *)((int)unaff_A6 + 0x91) = 0;
  *(undefined2 *)(extraout_A0_01 + 0x76) = *(undefined2 *)((int)unaff_A6 + 0x76);
  *unaff_A6 = 0x972a6;
  uVar2 = ((int)unaff_A6[0xd] >> 5) + 0x20000;
  uVar1 = unaff_A6[0xd];
  uVar4 = uVar1 < uVar2;
  unaff_A6[0xd] = uVar1 - uVar2;
  candidate_1ac38();
  candidate_97f52();
  if ((bool)uVar4) {
    *unaff_A6 = candidate_9749a;
  }
  candidate_427a();
  sVar3 = candidate_49e8();
  if (sVar3 == 2) {
    *unaff_A6 = candidate_9749a;
  }
  if (sVar3 == 3) {
    *unaff_A6 = candidate_9749a;
  }
  if (*(short *)(unaff_A6 + 0x10) < 0x121) {
    *unaff_A6 = candidate_9749a;
  }
  return;
}
