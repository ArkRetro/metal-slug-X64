/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07dfba.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7dfba(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_4256();
  *unaff_A6 = 0x7dfd0;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7e09e;
    uVar1 = 0;
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7dfea;
  }
  *(char *)(unaff_A6[3] + 0x61) = *(char *)(unaff_A6[3] + 0x61) + '\x01';
  if (*(short *)(unaff_A6 + 0xf) < 1) {
    *unaff_A6 = candidate_7e09e;
  }
  return;
}
