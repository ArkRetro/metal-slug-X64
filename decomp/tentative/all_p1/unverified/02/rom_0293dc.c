/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0293dc.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_293dc(void)

{
  int iVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  iVar1 = *(short *)((int)unaff_A6 + 0x76) * 0x100;
  if ((*(byte *)(unaff_A6 + 0x21) & 0x80) == 0) {
    iVar1 = *(short *)((int)unaff_A6 + 0x76) * -0x100;
  }
  *(int *)((int)unaff_A6 + 0x62) = iVar1;
  if ((*(byte *)(unaff_A6 + 0x21) & 0x40) != 0) {
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x40;
  }
  candidate_4256();
  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) == 0) {
    candidate_120f8();
  }
  *unaff_A6 = 0x29438;
  candidate_2997a();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_294a2;
  }
  candidate_299e0();
  candidate_427a();
  if ((*(byte *)(unaff_A6 + 0x21) & 0x20) == 0) {
    candidate_49e8();
  }
  if (((*(byte *)(unaff_A6 + 0x21) & 0x40) == 0) &&
     (*(short *)((int)unaff_A6 + 0x4e) < *(short *)((int)unaff_A6 + 0x7e))) {
    candidate_4256();
    *(byte *)(unaff_A6 + 0x21) = *(byte *)(unaff_A6 + 0x21) | 0x40;
    candidate_29ae4();
  }
  candidate_29a82();
  uVar2 = false;
  if (unaff_A6[0x14] != 0) {
    candidate_4c24();
    if ((bool)uVar2) {
      if (*(char *)((int)unaff_A6 + 0x5a) == '\x05') {
        *unaff_A6 = candidate_294a2;
      }
      *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
      uVar2 = 0;
    }
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_294a2;
      uVar2 = 0;
    }
  }
  candidate_c8bec();
  if ((bool)uVar2) {
    *unaff_A6 = 0x295bc;
  }
  return;
}
