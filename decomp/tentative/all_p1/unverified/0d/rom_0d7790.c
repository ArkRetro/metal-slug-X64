/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7790.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7790(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *unaff_A6 = 0xd7796;
  candidate_d906c();
  candidate_d8f14();
  bVar1 = false;
  candidate_d8fdc();
  if ((bVar1) && ((*(byte *)((int)unaff_A6 + 0x61) & 0x10) != 0)) {
    *unaff_A6 = candidate_d77da;
  }
  candidate_1ac38();
  candidate_427a();
  *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
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
