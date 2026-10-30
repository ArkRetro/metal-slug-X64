/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x020ba6.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_20ba6(void)

{
  short sVar1;
  int extraout_A0;
  undefined4 *unaff_A6;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x80;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x80;
  candidate_9f88();
  candidate_9ea8();
  candidate_1ab98();
  candidate_4256();
  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x72) = *(undefined2 *)((int)unaff_A6 + 0x72);
  sVar1 = candidate_29f2();
  *(short *)((int)unaff_A6 + 0x6e) = sVar1 + 0x14;
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = candidate_20c36;
  return;
}
