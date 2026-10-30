/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d7892.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d7892(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xc000;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0x100;
  *(undefined2 *)(unaff_A6 + 0x24) = 400;
  *(undefined1 *)(unaff_A6 + 0x22) = 0x84;
  *unaff_A6 = 0xd78b0;
  candidate_d906c();
  candidate_d8f14();
  bVar1 = false;
  candidate_d8fdc();
  if (bVar1) {
    *unaff_A6 = candidate_d768e;
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
