/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06bdb0.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6bdb0(void)

{
  short sVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;

  *(byte *)((int)unaff_A6 + 0x8d) = *(byte *)((int)unaff_A6 + 0x8d) & 0xfe;
  *(undefined2 *)((int)unaff_A6 + 0x86) = 0;
  *unaff_A6 = 0x6bdc2;
  sVar1 = *(short *)((int)unaff_A6 + 0x86);
  uVar2 = sVar1 == 0;
  sVar1 = sVar1 + -1;
  *(short *)((int)unaff_A6 + 0x86) = sVar1;
  if (sVar1 < 0) {
    *(undefined2 *)((int)unaff_A6 + 0x86) = *(undefined2 *)((int)unaff_A6 + 0x8a);
    uVar2 = 0;
    candidate_6c8fa();
  }
  candidate_6ca5c();
  candidate_1ac38();
  candidate_427a();
  candidate_4c24();
  if ((bool)uVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
    *(undefined1 *)((int)unaff_A6 + 0x91) = 0xff;
    uVar2 = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_6c0d8;
    uVar2 = 0;
  }
  candidate_1945c();
  if ((bool)uVar2) {
    *unaff_A6 = candidate_6c14c;
  }
  candidate_140c0();
  return;
}
