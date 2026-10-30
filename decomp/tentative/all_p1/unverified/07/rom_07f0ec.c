/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07f0ec.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7f0ec(void)

{
  short sVar1;
  int extraout_A0;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(undefined2 *)((int)unaff_A6 + 0x7a) = 0;
  *unaff_A6 = 0x7f0f8;
  unaff_A6[0xd] = 0;
  func_0x000143e4();
  candidate_427a();
  sVar1 = *(short *)((int)unaff_A6 + 0x7a);
  uVar2 = sVar1 == 0;
  *(short *)((int)unaff_A6 + 0x7a) = sVar1 + -1;
  if ((short)(sVar1 + -1) == 0 || sVar1 < 1) {
    *(undefined2 *)((int)unaff_A6 + 0x7a) = *(undefined2 *)((int)unaff_A6 + 0x7e);
    candidate_411c();
    candidate_1acf2();
    *(undefined4 *)(extraout_A0 + 0x30) = *(undefined4 *)((int)unaff_A6 + 0x6e);
    *(undefined4 *)(extraout_A0 + 0x34) = 0;
    uVar2 = 0;
    candidate_40bc();
  }
  candidate_4c24();
  if (((bool)uVar2) &&
     (*(undefined1 *)((int)unaff_A6 + 0x5a) = 0, (*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0)) {
    *unaff_A6 = candidate_7f196;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 1) != 0) {
    *unaff_A6 = candidate_7f26c;
  }
  candidate_7f7a0();
  return;
}
