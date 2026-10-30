/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0af17e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

ushort candidate_af17e(void)

{
  ushort uVar1;
  ushort unaff_D6w;
  undefined4 *unaff_A6;

  candidate_aef30();
  *(undefined1 *)((int)unaff_A6 + 0x81) = 2;
  uVar1 = *(ushort *)((int)unaff_A6 + 0x76);
  if (uVar1 == 2) {
    *unaff_A6 = candidate_aff16;
    return uVar1;
  }
  if (uVar1 == 3) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    *unaff_A6 = candidate_b0396;
    return uVar1;
  }
  if (uVar1 == 4) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    *unaff_A6 = candidate_b0396;
    return uVar1;
  }
  if (uVar1 == 7) {
    *unaff_A6 = candidate_af2ba;
    return uVar1;
  }
  if (uVar1 == 5) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
    *unaff_A6 = candidate_af302;
    return uVar1;
  }
  if (uVar1 == 6) {
    *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 1;
    *unaff_A6 = candidate_af302;
    return uVar1;
  }
  uVar1 = *(ushort *)(unaff_A6 + 0x1e) & 1;
  if ((*(ushort *)(unaff_A6 + 0x1e) & 1) != 0) {
    uVar1 = candidate_1495c();
    if ((unaff_D6w & 0x200) != 0) {
      *unaff_A6 = candidate_af7f6;
      return uVar1;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_afce2;
    return uVar1;
  }
  if (*(char *)(unaff_A6 + 0x1b) == '\x03') {
    *unaff_A6 = candidate_af380;
    return uVar1;
  }
  *unaff_A6 = candidate_af65a;
  return uVar1;
}
