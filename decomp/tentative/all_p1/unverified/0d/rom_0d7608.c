/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7608.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7608(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined2 *)(unaff_A6 + 0x2a) = 0x100;
  *(undefined1 *)((int)unaff_A6 + 0x61) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x76) = 0x2d;
  *unaff_A6 = 0xd7622;
  if (*(short *)(unaff_A6 + 0xf) < 0x111) {
    candidate_1abc8();
  }
  else {
    bVar1 = false;
    candidate_d8fdc();
    if (bVar1) {
      *(ushort *)(unaff_A6 + 0x24) = *(ushort *)(unaff_A6 + 0x24) ^ 0x10;
    }
    candidate_1ac38();
  }
  candidate_d90d2();
  *(short *)((int)unaff_A6 + 0x76) = *(short *)((int)unaff_A6 + 0x76) + -1;
  if (*(short *)((int)unaff_A6 + 0x76) == 0) {
    *(undefined1 *)((int)unaff_A6 + 0x85) = 2;
    *(undefined1 *)(unaff_A6 + 0x22) = 8;
  }
  if ((*(byte *)((int)unaff_A6 + 0x61) & 0x10) != 0) {
    *(undefined1 *)(unaff_A6 + 0x22) = 0x82;
    unaff_A6[0x14] = 0xe9866;
    *unaff_A6 = candidate_d768e;
  }
  *(undefined1 *)((int)unaff_A6 + 0x9a) = 0;
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    if (((0x8f < *(short *)(unaff_A6 + 0xf)) &&
        (bVar1 = *(ushort *)(unaff_A6 + 0xf) < 0x120, (short)*(ushort *)(unaff_A6 + 0xf) < 0x121))
       && (candidate_1af9a(), !bVar1)) {
      *unaff_A6 = candidate_d7d7c;
      return;
    }
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  return;
}
