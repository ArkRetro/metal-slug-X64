/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x06f326.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_6f326(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  *(undefined4 *)((int)unaff_A6 + 0x6e) = 0xe29b2;
  candidate_353c2();
  *(undefined1 *)((int)unaff_A6 + 0x83) = 0x60;
  candidate_1b1dc();
  *unaff_A6 = 0x6f34c;
  if (*(int *)((int)unaff_A6 + 0x6e) != 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_6ffa4();
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x76) = 0xff;
  if (*(short *)((int)unaff_A6 + 0x72) < 0) {
    candidate_6fcca();
  }
  else {
    uVar1 = candidate_6fc28();
    if ((uVar1 & 0x20) != 0) {
      candidate_aa7fc();
    }
    bVar2 = false;
    candidate_aa760();
    if (!bVar2) {
      *(undefined1 *)((int)unaff_A6 + 0x72) = 0xff;
    }
    candidate_6fc44();
  }
  candidate_6fc20();
  *(undefined2 *)(unaff_A6 + 0x1e) = 0x8000;
  if (*(char *)((int)unaff_A6 + 0x83) == '\0') {
    *unaff_A6 = candidate_6f3d4;
  }
  else {
    *(char *)((int)unaff_A6 + 0x83) = *(char *)((int)unaff_A6 + 0x83) + -1;
  }
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  return;
}
