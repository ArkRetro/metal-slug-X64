/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0bbe22.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_bbe22(void)

{
  char cVar1;
  undefined4 *unaff_A6;
  undefined1 uVar2;
  bool bVar3;

  candidate_1abc8();
  cVar1 = *(char *)((int)unaff_A6 + 0x7b) + -1;
  *(char *)((int)unaff_A6 + 0x7b) = cVar1;
  if (cVar1 != '\0') {
    return;
  }
  unaff_A6[0xe] = 0x10e968;
  unaff_A6[10] = 0x7a00000;
  unaff_A6[0xb] = 0xffd80000;
  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xbbaec;
  *(undefined2 *)((int)unaff_A6 + 0x72) = 0x7a8;
  bVar3 = false;
  candidate_193fe();
  if (bVar3) {
    candidate_18f92();
    candidate_c09d8();
    candidate_c8cd6();
    DAT_0010c00b = 0xff;
    candidate_ca340();
    candidate_ca340();
    *unaff_A6 = 0xb6f1e;
    uVar2 = 0;
    candidate_1abc8();
    candidate_1b2ce();
    if ((bool)uVar2) {
      candidate_18f40();
    }
  }
  return;
}
