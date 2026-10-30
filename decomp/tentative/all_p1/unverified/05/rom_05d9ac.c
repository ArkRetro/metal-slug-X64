/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x05d9ac.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_5d9ac(void)

{
  undefined2 uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  uVar1 = candidate_260c8();
  *(undefined2 *)(unaff_A6 + 0x1f) = uVar1;
  *(undefined1 *)((int)unaff_A6 + 0x86) = 0;
  candidate_4256();
  *unaff_A6 = 0x5d9d8;
  bVar2 = false;
  candidate_5e19c();
  if (bVar2) {
    *unaff_A6 = candidate_5da68;
  }
  candidate_427a();
  *(short *)(unaff_A6 + 0x1f) = *(short *)(unaff_A6 + 0x1f) + -1;
  if (*(short *)(unaff_A6 + 0x1f) < 0) {
    *(undefined2 *)(unaff_A6 + 0x1f) = 0;
  }
  bVar2 = false;
  candidate_5e388();
  if (bVar2) {
    *(undefined1 *)((int)unaff_A6 + 0x86) = 1;
    *unaff_A6 = candidate_5da16;
  }
  candidate_5e6d2();
  candidate_4c24();
  if ((*(byte *)((int)unaff_A6 + 0x5f) & 0x80) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x86) = 2;
    *unaff_A6 = candidate_5dc32;
    return;
  }
  *(undefined1 *)((int)unaff_A6 + 0x5a) = 0;
  if (*(short *)(unaff_A6 + 0xf) < -0x1f) {
    *unaff_A6 = candidate_5dc7a;
  }
  if (0x15f < *(short *)(unaff_A6 + 0xf)) {
    *unaff_A6 = candidate_5dc7a;
  }
  if (*(short *)(unaff_A6 + 0x10) < 0xd1) {
    *unaff_A6 = candidate_5dc7a;
  }
  bVar2 = false;
  candidate_1945c();
  if (bVar2) {
    *unaff_A6 = candidate_5dc7a;
  }
  return;
}
