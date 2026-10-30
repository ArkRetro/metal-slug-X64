/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077af4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77af4(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_411c();
  *(undefined2 *)(extraout_A0 + 0x66) = 0xe0ed;
  *(undefined2 *)(extraout_A0 + 0x1e) = 0xe0ed;
  candidate_1acf2();
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0x40;
  candidate_4256();
  *unaff_A6 = 0x77b40;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((*(byte *)(unaff_A6[3] + 0x5f) & 0x80) != 0) {
    if (*(short *)((int)unaff_A6 + 0x86) == 0) {
      candidate_1da40();
      *(undefined2 *)(extraout_A0_00 + 0x66) = 0xe0ed;
      *(undefined2 *)(extraout_A0_00 + 0x1e) = 0xe0ed;
      candidate_1da40();
      *(undefined2 *)(extraout_A0_01 + 0x66) = 0xe0ed;
      *(undefined2 *)(extraout_A0_01 + 0x1e) = 0xe0ed;
    }
    *unaff_A6 = candidate_77ba8;
    uVar1 = 0;
  }
  candidate_78d46();
  if ((bool)uVar1) {
    *unaff_A6 = 0x77e5c;
  }
  return;
}
