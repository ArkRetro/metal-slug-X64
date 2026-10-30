/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7b2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7b2a(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  if ((*(byte *)((int)unaff_A6 + 0x61) & 1) != 0) {
    *unaff_A6 = candidate_d7b48;
  }
  candidate_1abc8();
  candidate_d90f8();
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
