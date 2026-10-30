/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0207ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_207ea(void)

{
  char cVar1;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  int extraout_A0_03;
  int extraout_A0_04;
  int extraout_A0_05;
  int extraout_A0_06;
  undefined4 *unaff_A6;

  candidate_3d4ac();
  candidate_1aa28();
  candidate_c81a6();
  candidate_40bc();
  candidate_40bc();
  *(undefined2 *)(extraout_A0 + 0x72) = 0;
  candidate_40bc();
  *(undefined2 *)(extraout_A0_00 + 0x72) = 1;
  candidate_40bc();
  *(undefined2 *)(extraout_A0_01 + 0x72) = 0;
  candidate_40bc();
  *(undefined2 *)(extraout_A0_02 + 0x72) = 1;
  candidate_40bc();
  *(undefined2 *)(extraout_A0_03 + 0x72) = 2;
  candidate_40bc();
  *(undefined2 *)(extraout_A0_04 + 0x72) = 3;
  candidate_120f8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  *unaff_A6 = 0x208b4;
  if (*(short *)((int)unaff_A6 + 0x6e) < 0x14) {
    *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + 1;
    if (*(short *)((int)unaff_A6 + 0x6e) == 2) {
      candidate_9c0a();
    }
  }
  else {
    if (*(char *)(unaff_A6 + 0x18) == -1) {
      cVar1 = candidate_c8df8();
      if (cVar1 == '\0') {
        *(undefined1 *)(unaff_A6 + 0x18) = 0;
        *(undefined2 *)((int)unaff_A6 + 0x6e) = 0x5a;
        candidate_c8cd6();
        *unaff_A6 = candidate_20966;
      }
      else {
        candidate_3d004();
      }
    }
    if (*(char *)(unaff_A6 + 0x18) == '\x04') {
      *(undefined1 *)(unaff_A6 + 0x18) = 0xff;
      candidate_c8ce6();
      candidate_40bc();
      *(undefined2 *)(extraout_A0_05 + 0x72) = 0;
      candidate_40bc();
      *(undefined2 *)(extraout_A0_06 + 0x72) = 1;
    }
  }
  return;
}
