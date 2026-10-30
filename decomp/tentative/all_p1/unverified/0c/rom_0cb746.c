/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0cb746.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_cb746(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  undefined1 in_CF;
  bool bVar1;

  candidate_4c24();
  if (((bool)in_CF) ||
     ((0xea7 < *(short *)(unaff_A5 + 0x699e) &&
      (*(char *)((int)unaff_A6 + 0x89) = *(char *)((int)unaff_A6 + 0x89) + -1,
      *(char *)((int)unaff_A6 + 0x89) == '\0')))) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    unaff_A6[0x14] = 0xe812e;
    candidate_cae80();
    *unaff_A6 = 0xcb784;
  }
  candidate_1abc8();
  candidate_427a();
  if (0x22b < *(short *)(unaff_A6 + 0xf)) {
    *(undefined1 *)((int)unaff_A6 + 0x87) = 2;
  }
  *(short *)((int)unaff_A6 + 0x7a) =
       (short)(((uint)*(ushort *)((int)unaff_A6 + 0x4e) << 8) / (uint)*(ushort *)(unaff_A6 + 0x1e));
  candidate_cae88();
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0xbf;
  bVar1 = false;
  if (*(char *)((int)unaff_A6 + 0x87) != '\0') {
    *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) | 0x40;
  }
  candidate_4c24();
  *(char *)((int)unaff_A6 + 0x85) = -bVar1;
  if (bVar1) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if (*(char *)((int)unaff_A6 + 0x87) != '\0') {
    *(char *)((int)unaff_A6 + 0x87) = *(char *)((int)unaff_A6 + 0x87) + -1;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    candidate_18f92();
    *(undefined1 *)((int)unaff_A6 + 0x85) = 0;
    *unaff_A6 = candidate_cb806;
  }
  return;
}
