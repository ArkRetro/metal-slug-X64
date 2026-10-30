/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077488.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77488(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_1b1dc();
  unaff_A6[0xd] = 0xffff4000;
  *(undefined2 *)((int)unaff_A6 + 0x7e) = 0;
  *(undefined2 *)(unaff_A6 + 0x20) = 0;
  candidate_4256();
  candidate_411c();
  candidate_1acf2();
  candidate_40bc();
  *(undefined1 *)(extraout_A0 + 0x9c) = 0;
  candidate_40bc();
  *(undefined1 *)(extraout_A0_00 + 0x9c) = 4;
  candidate_40bc();
  *(undefined1 *)(extraout_A0_01 + 0x9c) = 8;
  candidate_40bc();
  *(undefined1 *)(extraout_A0_02 + 0x9c) = 0xc;
  candidate_40bc();
  *(undefined1 *)(extraout_A0_03 + 0x9c) = 0x10;
  candidate_40bc();
  *(undefined1 *)(extraout_A0_04 + 0x9c) = 0x14;
  *unaff_A6 = 0x7755a;
  candidate_78d96();
  candidate_1ac38();
  candidate_140c0();
  candidate_427a();
  if (*(short *)(unaff_A6 + 0x10) < 0xe1) {
    *(undefined1 *)((int)unaff_A6 + 0x9b) = 4;
    unaff_A6[0xd] = 0xfffbc000;
  }
  bVar1 = false;
  candidate_78d46();
  if (bVar1) {
    *unaff_A6 = candidate_77e50;
  }
  return;
}
