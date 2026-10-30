/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d25b4.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d25b4(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar3;

  uVar2 = candidate_260c8();
  *(undefined1 *)(unaff_A6 + 0x1e) = uVar2;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar1;
  *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  bVar3 = false;
  candidate_40bc();
  if (bVar3) {
    candidate_1b220();
  }
  candidate_4256();
  *unaff_A6 = 0xd2630;
  *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 2;
  candidate_d3220();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    candidate_d35d2();
    uVar1 = candidate_260c8();
    *(undefined2 *)((int)unaff_A6 + 0x6e) = uVar1;
    *(char *)(unaff_A6 + 0x1e) = *(char *)(unaff_A6 + 0x1e) + -1;
    if (*(char *)(unaff_A6 + 0x1e) < '\x01') {
      uVar2 = candidate_260c8();
      *(undefined1 *)(unaff_A6 + 0x1e) = uVar2;
      *(short *)((int)unaff_A6 + 0x92) = *(short *)((int)unaff_A6 + 0x92) + 1;
      if (*(short *)((int)unaff_A6 + 0x92) == 5) {
        *unaff_A6 = candidate_d26b8;
      }
    }
  }
  *(short *)((int)unaff_A6 + 0x6e) = *(short *)((int)unaff_A6 + 0x6e) + -1;
  uVar2 = false;
  if (*(short *)((int)unaff_A6 + 0x6e) < 1) {
    *(byte *)(unaff_A6[3] + 0x70) = *(byte *)(unaff_A6[3] + 0x70) | 8;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  candidate_d341e();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_d2730;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d2730;
  }
  return;
}
