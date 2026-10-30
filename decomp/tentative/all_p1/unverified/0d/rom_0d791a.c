/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d791a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d791a(void)

{
  undefined4 *unaff_A6;
  bool bVar1;

  *(undefined2 *)((int)unaff_A6 + 0x6e) = 0xc000;
  *(undefined2 *)((int)unaff_A6 + 0x8e) = 0x120;
  *(undefined2 *)(unaff_A6 + 0x24) = 0x200;
  *unaff_A6 = 0xd7932;
  candidate_d906c();
  bVar1 = false;
  candidate_d8fdc();
  if (bVar1) {
    *unaff_A6 = candidate_d7962;
  }
  candidate_1ac38();
  *(undefined2 *)((int)unaff_A6 + 0x92) = 0;
  candidate_d90f8();
  candidate_4c24();
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined2 *)((int)unaff_A6 + 0x4e) = 1;
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  }
  return;
}
