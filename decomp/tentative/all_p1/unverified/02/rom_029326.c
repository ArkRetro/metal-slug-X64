/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029326.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29326(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) & 0xfb;
  candidate_1b1dc();
  if ((*(byte *)(unaff_A6 + 0x21) & 0x40) != 0) {
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x40;
  }
  candidate_4256();
  candidate_12192();
  *unaff_A6 = 0x2936c;
  uVar1 = 0;
  candidate_299a8();
  candidate_299e0();
  candidate_427a();
  if (((*(byte *)(unaff_A6 + 0x21) & 0x40) == 0) &&
     (uVar1 = *(ushort *)((int)unaff_A6 + 0x4e) < *(ushort *)((int)unaff_A6 + 0x7e),
     (short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)*(ushort *)((int)unaff_A6 + 0x7e))) {
    uVar1 = 0;
    candidate_4256();
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x40;
    candidate_29ae4();
  }
  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) != 0) {
    candidate_44930();
    if ((bool)uVar1) {
      *unaff_A6 = candidate_293dc;
    }
    *(short *)((int)unaff_A6 + 0x82) = *(short *)((int)unaff_A6 + 0x82) + -1;
    if (*(short *)((int)unaff_A6 + 0x82) < 1) {
      *unaff_A6 = candidate_293dc;
    }
  }
  uVar1 = false;
  if (unaff_A6[0x14] != 0) {
    candidate_4c24();
    if ((bool)uVar1) {
      if (*(char *)((int)unaff_A6 + 0x5a) == '\x05') {
        *unaff_A6 = candidate_294a2;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar1 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_294a2;
      uVar1 = 0;
    }
  }
  candidate_c8bec();
  if ((bool)uVar1) {
    *unaff_A6 = 0x295bc;
  }
  return;
}
