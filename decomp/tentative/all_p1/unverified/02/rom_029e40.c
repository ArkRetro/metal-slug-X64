/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x029e40.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_29e40(void)

{
  ushort uVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_2a472();
  candidate_427a();
  uVar2 = *(ushort *)(unaff_A6 + 0xf) < *(ushort *)(unaff_A6 + 0x20);
  if ((short)*(ushort *)(unaff_A6 + 0xf) <= (short)*(ushort *)(unaff_A6 + 0x20)) {
    candidate_1af9a();
    if ((!(bool)uVar2) &&
       (uVar1 = *(short *)(unaff_A6 + 0xf) - *(short *)(extraout_A0 + 0x3c), uVar2 = uVar1 < 0x60,
       (short)uVar1 < 0x61)) {
      *unaff_A6 = candidate_29ece;
      uVar2 = 0;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x86) & 8) != 0) {
    *unaff_A6 = candidate_29ece;
    uVar2 = 0;
  }
  candidate_4c24();
  if ((bool)uVar2) {
    *(byte *)((int)unaff_A6 + 0x86) = *(byte *)((int)unaff_A6 + 0x86) | 8;
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_2a04c;
    uVar2 = 0;
  }
  if (((*(byte *)((int)unaff_A6 + 0x86) & 0x80) != 0) &&
     ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0)) {
    *unaff_A6 = candidate_2a0b2;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_41ac;
  }
  return;
}
