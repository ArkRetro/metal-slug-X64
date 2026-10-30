/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x077e64.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_77e64(void)

{
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_40bc();
  candidate_40bc();
  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined1 *)(unaff_A6 + 0x27) = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x96) = 0;
  *(undefined2 *)(unaff_A6 + 0x26) = 0;
  *(undefined2 *)(unaff_A6 + 0x22) = 0;
  *unaff_A6 = 0x77eb2;
  *(undefined1 *)((int)unaff_A6 + 0x9b) = *(undefined1 *)(unaff_A6[3] + 0x9b);
  candidate_1abc8();
  candidate_78f3e();
  if (((*(byte *)(unaff_A6 + 0x27) & 1) == 0) &&
     (*(short *)(unaff_A6 + 0x25) = *(short *)(unaff_A6 + 0x25) + 1,
     *(short *)((int)unaff_A6 + 0x8e) <= *(short *)(unaff_A6 + 0x25))) {
    *(undefined2 *)(unaff_A6 + 0x25) = 0;
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)(extraout_A0 + 0x90) = 0xffb0;
    *(undefined2 *)(extraout_A0 + 0x92) = 0xffd0;
    *(undefined2 *)(extraout_A0 + 0x8a) = 0;
  }
  if (((*(byte *)(unaff_A6 + 0x27) & 2) == 0) &&
     (*(short *)((int)unaff_A6 + 0x96) = *(short *)((int)unaff_A6 + 0x96) + 1,
     *(short *)((int)unaff_A6 + 0x8e) <= *(short *)((int)unaff_A6 + 0x96))) {
    *(undefined2 *)((int)unaff_A6 + 0x96) = 0;
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)(extraout_A0_00 + 0x90) = 0;
    *(undefined2 *)(extraout_A0_00 + 0x92) = 0x38;
    *(undefined2 *)(extraout_A0_00 + 0x8a) = 1;
  }
  if (((*(byte *)(unaff_A6 + 0x27) & 4) == 0) &&
     (*(short *)(unaff_A6 + 0x26) = *(short *)(unaff_A6 + 0x26) + 1,
     *(short *)((int)unaff_A6 + 0x8e) <= *(short *)(unaff_A6 + 0x26))) {
    *(undefined2 *)(unaff_A6 + 0x26) = 0;
    candidate_40bc();
    candidate_1acf2();
    *(undefined2 *)(extraout_A0_01 + 0x90) = 0x40;
    *(undefined2 *)(extraout_A0_01 + 0x92) = 0x58;
    *(undefined2 *)(extraout_A0_01 + 0x8a) = 2;
  }
  if (*(char *)((int)unaff_A6 + 0x9b) != '\0') {
    *unaff_A6 = candidate_77fc4;
  }
  bVar1 = false;
  candidate_78d7a();
  if (bVar1) {
    *unaff_A6 = candidate_77e50;
  }
  return;
}
