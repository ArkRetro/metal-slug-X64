/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x0892fe.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_892fe(void)

{
  undefined4 uVar1;
  int unaff_A6;
  undefined1 in_CF;
  bool bVar2;
  undefined1 uVar3;

  candidate_4c24();
  if (((bool)in_CF) && (*(char *)(unaff_A6 + 0xaa) == '\0')) {
    *(undefined4 *)(unaff_A6 + 0x86) = *(undefined4 *)(unaff_A6 + 0x20);
    bVar2 = false;
    candidate_1b206();
    if (bVar2) {
      candidate_4256();
      *(undefined1 *)(unaff_A6 + 0xaa) = 0;
      *(undefined1 *)(unaff_A6 + 0xaa) = 0xff;
    }
  }
  uVar3 = 0;
  if (*(char *)(unaff_A6 + 0xaa) == '\0') {
    candidate_146a8();
  }
  else {
    uVar1 = *(undefined4 *)(unaff_A6 + 0x62);
    *(undefined4 *)(unaff_A6 + 0x62) = 0;
    candidate_146a8();
    *(undefined4 *)(unaff_A6 + 0x62) = uVar1;
    uVar3 = 0;
  }
  candidate_8919c();
  candidate_427a();
  *(undefined1 *)(unaff_A6 + 0xac) = -uVar3;
  if (((bool)uVar3) && (*(char *)(unaff_A6 + 0xaa) != '\0')) {
    candidate_4256();
    *(undefined1 *)(unaff_A6 + 0xaa) = 0;
  }
  return;
}
