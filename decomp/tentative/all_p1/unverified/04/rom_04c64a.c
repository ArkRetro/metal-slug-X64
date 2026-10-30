/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x04c64a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_4c64a(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_4256();
  *unaff_A6 = 0x4c660;
  uVar1 = 0;
  candidate_1abc8();
  candidate_4d9de();
  if ((bool)uVar1) {
    *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0x9e) = 0;
    *unaff_A6 = candidate_4c3ea;
  }
  *(undefined1 *)(*(int *)(unaff_A6[3] + 0xc) + 0xa3) = *(undefined1 *)((int)unaff_A6 + 0xa3);
  bVar2 = false;
  candidate_4d74a();
  if (bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x61) = 0xff;
  }
  return;
}
