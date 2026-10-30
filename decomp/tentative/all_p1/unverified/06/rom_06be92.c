/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06be92.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6be92(void)

{
  ushort uVar1;
  undefined4 *unaff_A6;
  undefined1 in_CF;

  candidate_6ca80();
  candidate_427a();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_6beaa;
    in_CF = 0;
  }
  candidate_4c24();
  if ((bool)in_CF) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
    uVar1 = candidate_260c8();
    in_CF = uVar1 < *(ushort *)((int)unaff_A6 + 0x4e);
    if ((short)*(ushort *)((int)unaff_A6 + 0x4e) < (short)uVar1) {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) | 2;
    }
    else {
      *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xfd;
    }
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6c0d8;
    in_CF = 0;
  }
  candidate_1945c();
  if ((bool)in_CF) {
    *unaff_A6 = candidate_6c14c;
  }
  candidate_140c0();
  return;
}
