/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07865c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7865c(void)

{
  short sVar1;
  int extraout_A0;
  int extraout_A0_00;
  int extraout_A0_01;
  int extraout_A0_02;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_1ad6e();
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)(unaff_A6 + 0x21) = 0;
  *(undefined1 *)(unaff_A6 + 0x27) = 0;
  *(undefined2 *)(unaff_A6 + 0x25) = 0x28;
  *(undefined2 *)((int)unaff_A6 + 0x96) = 0x78;
  *(undefined2 *)(unaff_A6 + 0x26) = 0x3c;
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  candidate_40bc();
  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    *(byte *)(unaff_A6 + 0x27) = *(byte *)(unaff_A6 + 0x27) | 8;
    *(undefined2 *)(extraout_A0 + 0x8a) = 3;
  }
  *unaff_A6 = 0x786c4;
  *(undefined1 *)((int)unaff_A6 + 0x9b) = *(undefined1 *)(unaff_A6[3] + 0x9b);
  candidate_1abc8();
  if (*(char *)((int)unaff_A6 + 0x9b) != '\x02') {
    *unaff_A6 = candidate_787ae;
    return;
  }
  if ((*(byte *)(unaff_A6 + 0x27) & 8) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
    sVar1 = *(short *)(unaff_A6 + 0x25) + -1;
    *(short *)(unaff_A6 + 0x25) = sVar1;
    if (sVar1 == 0) {
      *(undefined2 *)(unaff_A6 + 0x25) = 0x28;
      candidate_40bc();
      *(undefined2 *)(extraout_A0_00 + 0x8a) = 3;
    }
  }
  if (((*(byte *)(unaff_A6 + 0x27) & 0x10) == 0) &&
     (sVar1 = *(short *)(unaff_A6 + 0x26) + -1, *(short *)(unaff_A6 + 0x26) = sVar1, sVar1 == 0)) {
    *(undefined2 *)(unaff_A6 + 0x26) = 0x3c;
    candidate_40bc();
    *(undefined2 *)(extraout_A0_01 + 0x8a) = 4;
    candidate_1acf2();
  }
  sVar1 = *(short *)((int)unaff_A6 + 0x96) + -1;
  *(short *)((int)unaff_A6 + 0x96) = sVar1;
  if (sVar1 == 0) {
    sVar1 = candidate_29f2();
    *(short *)((int)unaff_A6 + 0x96) = sVar1 + 0xec;
    candidate_40bc();
    *(undefined2 *)(extraout_A0_02 + 0x8a) = 5;
    candidate_1acf2();
  }
  if (*(char *)((int)unaff_A6 + 0x9b) != '\x02') {
    *unaff_A6 = candidate_787ae;
  }
  bVar2 = false;
  candidate_78d7a();
  if (bVar2) {
    *unaff_A6 = candidate_77e50;
  }
  return;
}
