/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03fd6e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3fd6e(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_1ad6e();
  *(undefined2 *)(unaff_A6 + 0x1c) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  unaff_A6[0x14] = 0xe0942;
  *unaff_A6 = 0x3fd9e;
  uVar1 = 0;
  candidate_1abc8();
  candidate_4c24();
  if ((bool)uVar1) {
    if ((short)(*(short *)((int)unaff_A6 + 0x72) << 3) == *(short *)(unaff_A6[3] + 0x70)) {
      candidate_40bc();
      candidate_29c6();
      candidate_1acf2();
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  bVar2 = false;
  if (*(short *)(unaff_A6 + 0x1c) == 0) {
    candidate_3fe38();
    if (bVar2) {
      candidate_40bc();
      candidate_29c6();
      candidate_1acf2();
      *(undefined2 *)(unaff_A6 + 0x1c) = 0x20;
    }
  }
  else {
    *(short *)(unaff_A6 + 0x1c) = *(short *)(unaff_A6 + 0x1c) + -1;
  }
  candidate_3fada();
  return;
}
