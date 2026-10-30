/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x07387a.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_7387a(void)

{
  uint uVar1;
  undefined4 *unaff_A6;
  bool bVar2;

  candidate_353c2();
  *(undefined1 *)(unaff_A6 + 0x28) = 0x60;
  candidate_1b1dc();
  candidate_74c00();
  candidate_73cd8();
  *(undefined1 *)((int)unaff_A6 + 0x8d) = 1;
  *(undefined1 *)((int)unaff_A6 + 0x9d) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x8a) = 5;
  *unaff_A6 = 0x738b4;
  if ((*(byte *)((int)unaff_A6 + 0x15) & 0x40) != 0) {
    *(undefined1 *)((int)unaff_A6 + 0x8a) = 6;
    candidate_1b1dc();
    *unaff_A6 = 0x738d4;
  }
  candidate_73cc0();
  if (*(int *)((int)unaff_A6 + 0x6e) != 0) {
    *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x1a);
    candidate_74be0();
  }
  *(undefined4 *)((int)unaff_A6 + 0x62) = 0;
  *(undefined1 *)((int)unaff_A6 + 0x82) = 0xff;
  if (*(short *)((int)unaff_A6 + 0x7e) < 0) {
    candidate_74b74();
  }
  else {
    uVar1 = candidate_74ce4();
    if ((uVar1 & 0x20) != 0) {
      candidate_aa7fc();
    }
    bVar2 = false;
    candidate_aa760();
    if (!bVar2) {
      *(undefined1 *)((int)unaff_A6 + 0x7e) = 0xff;
      *(undefined1 *)((int)unaff_A6 + 0x8d) = 0;
    }
    candidate_74ba4();
  }
  candidate_74c3c();
  if (*(char *)(unaff_A6 + 0x28) == '\0') {
    *unaff_A6 = candidate_73968;
  }
  else {
    *(char *)(unaff_A6 + 0x28) = *(char *)(unaff_A6 + 0x28) + -1;
  }
  *(byte *)((int)unaff_A6 + 0x8e) = *(byte *)((int)unaff_A6 + 0x8e) & 0xbf;
  *(byte *)((int)unaff_A6 + 0x5f) = *(byte *)((int)unaff_A6 + 0x5f) & 0x7f;
  *(undefined1 *)((int)unaff_A6 + 0x96) = 0xff;
  if (*(char *)(unaff_A6 + 0x27) != '\0') {
    *(char *)(unaff_A6 + 0x27) = *(char *)(unaff_A6 + 0x27) + -1;
  }
  return;
}
