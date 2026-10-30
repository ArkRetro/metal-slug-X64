/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d3c76.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d3c76(void)

{
  undefined2 uVar1;
  ushort uVar2;
  undefined4 *unaff_A6;
  bool bVar3;

  candidate_1ab98();
  candidate_146e8();
  *(undefined2 *)(unaff_A6 + 10) = 0x1040;
  candidate_9ea8();
  *(undefined1 *)(unaff_A6 + 0x1f) = 1;
  candidate_4520();
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  *(undefined1 *)((int)unaff_A6 + 0x7f) = 1;
  *(undefined1 *)(unaff_A6 + 0x20) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x82) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x7e) = 0;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x4e) = uVar1;
  candidate_1dc8e();
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0x112d;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x112d;
  if (*(char *)((int)unaff_A6 + 0x6f) != '\0') {
    *(undefined2 *)((int)unaff_A6 + 0x66) = 0x11f5;
    *(undefined2 *)((int)unaff_A6 + 0x1e) = 0x11f5;
  }
  uVar2 = candidate_29c6();
  *(ushort *)((int)unaff_A6 + 0x76) = uVar2 & 0x3f;
  uVar2 = candidate_29c6();
  *(ushort *)(unaff_A6 + 0x1e) = uVar2 & 0x3f;
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x85) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x9e) = 0xe8e22;
  unaff_A6[0x14] = 0xe8cc6;
  *(undefined4 *)((int)unaff_A6 + 0x9a) = 0xe8df2;
  *(undefined2 *)(unaff_A6 + 0x1d) = 1;
  *unaff_A6 = 0xd3e02;
  candidate_1abc8();
  candidate_d50a8();
  *(short *)(unaff_A6 + 0x1d) = *(short *)(unaff_A6 + 0x1d) + -1;
  if (*(short *)(unaff_A6 + 0x1d) < 1) {
    *unaff_A6 = candidate_d3e22;
  }
  return;
}
