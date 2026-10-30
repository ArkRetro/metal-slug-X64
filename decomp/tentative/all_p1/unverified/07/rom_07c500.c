/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07c500.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7c500(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;
  bool bVar2;

  candidate_12192();
  unaff_A6[0x14] = 0xe45e0;
  candidate_1b1dc();
  candidate_4256();
  *unaff_A6 = 0x7c52e;
  uVar1 = 0;
  candidate_1abc8();
  candidate_427a();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_7c548;
    uVar1 = 0;
  }
  func_0x0007cce0();
  candidate_4c24();
  if ((bool)uVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)(unaff_A6 + 0x22) = 10;
  }
  *(char *)(unaff_A6 + 0x22) = *(char *)(unaff_A6 + 0x22) + -1;
  if (*(char *)(unaff_A6 + 0x22) < '\x01') {
    *(undefined1 *)(unaff_A6 + 0x22) = 0;
  }
  if (*(char *)(unaff_A6 + 0x22) != '\0') {
    *(char *)((int)unaff_A6 + 0x8a) = *(char *)((int)unaff_A6 + 0x8a) + '\x01';
    *(short *)(unaff_A6 + 0xb) =
         *(short *)((short)((*(byte *)((int)unaff_A6 + 0x8a) & 3) * 2) + 0xe4658) +
         *(short *)(unaff_A6 + 0xb);
  }
  bVar2 = false;
  if ((*(char *)(unaff_A6 + 0x26) != '\0') &&
     (bVar2 = *(char *)(unaff_A6[3] + 0x60) != -1, *(char *)(unaff_A6[3] + 0x60) == -1)) {
    *unaff_A6 = candidate_7c72e;
    bVar2 = false;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_7c85a;
    bVar2 = false;
  }
  candidate_19504();
  if (bVar2) {
    *unaff_A6 = 0x7c946;
  }
  return;
}
