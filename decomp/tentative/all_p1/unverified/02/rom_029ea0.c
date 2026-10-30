/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029ea0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29ea0(void)

{
  undefined4 *unaff_A6;
  undefined1 uVar1;

  candidate_2a472();
  candidate_427a();
  uVar1 = *(ushort *)(unaff_A6 + 0xf) < *(ushort *)(unaff_A6 + 0x20);
  if ((short)*(ushort *)(unaff_A6 + 0xf) <= (short)*(ushort *)(unaff_A6 + 0x20)) {
    candidate_1affc();
    if (!(bool)uVar1) {
      *unaff_A6 = candidate_29f72;
      uVar1 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar1) {
    *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 8;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar1 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2a04c;
    uVar1 = 0;
  }
  if (((*(byte *)((int)unaff_A6 + 0x86) & 0x80) != 0) &&
     ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
    *unaff_A6 = candidate_2a0b2;
    uVar1 = 0;
  }
  candidate_1945c();
  if ((bool)uVar1) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
