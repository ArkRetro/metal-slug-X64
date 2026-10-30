/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07de3c.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7de3c(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  unaff_A6[0x14] = 0xe47c0;
  candidate_4256();
  *unaff_A6 = 0x7de5a;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  candidate_7e290();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7de78;
  }
  if (*(char *)(unaff_A6[3] + 0x60) != '\0') {
    *unaff_A6 = candidate_7dfba;
  }
  if (*(short *)(unaff_A6 + 0x1d) == 0) {
    *unaff_A6 = candidate_7dfba;
  }
  bVar2 = false;
  candidate_4c24();
  if (bVar2) {
    *unaff_A6 = candidate_7dfea;
  }
  *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
  if (*(short *)(unaff_A6 + 0xf) < 1) {
    *unaff_A6 = candidate_7e09e;
  }
  return;
}
