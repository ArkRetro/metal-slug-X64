/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029294.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29294(void)

{
  undefined4 *unaff_A6;
  undefined1 in_CF;
  undefined1 uVar1;

  candidate_2997a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_29326;
    in_CF = 0;
  }
  candidate_299e0();
  candidate_427a();
  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) == 0) {
    candidate_49e8();
  }
  if (((*(byte *)(unaff_A6 + 0x21) & 0x40) == 0) &&
     (in_CF = *(ushort *)((int)unaff_A6 + 0x4e) < *(ushort *)((int)unaff_A6 + 0x7e),
     (short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)*(ushort *)((int)unaff_A6 + 0x7e))) {
    in_CF = 0;
    candidate_4256();
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x40;
    candidate_29ae4();
  }
  candidate_29a82();
  candidate_29a56();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_29326;
    in_CF = 0;
  }
  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) != 0) {
    candidate_44930();
    if ((bool)in_CF) {
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
