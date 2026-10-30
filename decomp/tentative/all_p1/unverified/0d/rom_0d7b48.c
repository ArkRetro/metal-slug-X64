/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7b48.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7b48(void)

{
  undefined1 uVar2;
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar3;

  uVar2 = candidate_260c8();
  *(undefined1 *)((int)unaff_A6 + 0x7f) = uVar2;
  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1e) = uVar1;
  if (*(char *)((int)unaff_A6 + 0x7f) < '\x01') {
    *(undefined1 *)(unaff_A6 + 0x21) = 1;
    *unaff_A6 = 0xd7bd6;
    candidate_1abc8();
    candidate_d90f8();
    if ((*(byte *)((int)unaff_A6 + 0x61) & 1) != 0) {
      *unaff_A6 = candidate_d768e;
    }
  }
  else {
    *(char *)((int)unaff_A6 + 0x7f) = *(char *)((int)unaff_A6 + 0x7f) + -1;
    *(undefined2 *)((int)unaff_A6 + 0x76) = *(undefined2 *)(unaff_A6 + 0x1e);
    *(undefined1 *)(unaff_A6 + 0x21) = 0xb;
    *unaff_A6 = 0xd7ba8;
    candidate_1abc8();
    candidate_d90f8();
    *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
    if (*(short *)((int)unaff_A6 + 0x76) < 1) {
      *unaff_A6 = 0xd7b88;
    }
  }
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (((*(short *)(unaff_A6 + 0xf) < 0x90) ||
        (bVar3 = *(ushort *)(unaff_A6 + 0xf) < 0x120, 0x120 < (short)*(ushort *)(unaff_A6 + 0xf)))
       || (candidate_1af9a(), bVar3)) {
      *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
      *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
    }
    else {
      *unaff_A6 = candidate_d7d7c;
    }
  }
  return;
}
