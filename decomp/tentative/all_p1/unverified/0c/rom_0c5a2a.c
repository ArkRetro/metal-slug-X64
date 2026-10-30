/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0c5a2a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_c5a2a(void)

{
  ushort uVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  candidate_c6a02();
  candidate_427a();
  candidate_c699c();
  uVar2 = false;
  if (*(char *)((int)unaff_A6 + 0x96) == '\0') {
    candidate_c6baa();
    if ((bool)uVar2) {
      *(undefined1 *)((int)unaff_A6 + 0x97) = 1;
      *unaff_A6 = candidate_c5ac2;
      uVar2 = 0;
    }
  }
  else {
    candidate_1af9a();
    uVar1 = *(short *)(unaff_A6 + 0xf) - *(short *)(extraout_A0 + 0x3c);
    uVar2 = uVar1 < *(ushort *)((int)unaff_A6 + 0x6e);
    if ((short)uVar1 <= (short)*(ushort *)((int)unaff_A6 + 0x6e)) {
      *(undefined1 *)((int)unaff_A6 + 0x97) = 1;
      *unaff_A6 = candidate_c5ac2;
      uVar2 = 0;
    }
  }
  candidate_4c24();
  if ((bool)uVar2) {
    if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
      *unaff_A6 = candidate_c5d26;
    }
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) == 0) {
    *unaff_A6 = candidate_c5dcc;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_c66e8;
  }
  return;
}
