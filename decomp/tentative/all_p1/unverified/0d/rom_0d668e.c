/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0d668e.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_d668e(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  bVar2 = false;
  candidate_40bc();
  if (bVar2) {
    candidate_1b220();
  }
  *(undefined1 *)(unaff_A6 + 0x18) = 0;
  uVar1 = candidate_260c8();
  *(undefined2 *)((int)unaff_A6 + 0x72) = uVar1;
  candidate_4256();
  *unaff_A6 = 0xd66dc;
  if (*(int *)((int)unaff_A6 + 0x9a) != 0) {
    candidate_140c0();
  }
  candidate_1abc8();
  candidate_427a();
  *(short *)((int)unaff_A6 + 0x72) = *(short *)((int)unaff_A6 + 0x72) + -1;
  if (*(short *)((int)unaff_A6 + 0x72) < 1) {
    *unaff_A6 = candidate_d6710;
  }
  bVar2 = false;
  candidate_4c24();
  if (bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  }
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *unaff_A6 = candidate_d678e;
  }
  if (*(char *)(unaff_A6[3] + 0x7a) == '\x05') {
    *unaff_A6 = 0xd6792;
  }
  return;
}
