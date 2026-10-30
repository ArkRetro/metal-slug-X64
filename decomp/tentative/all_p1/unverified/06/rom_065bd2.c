/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x065bd2.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_65bd2(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_3cf60();
  candidate_9ea8();
  *(undefined2 *)(unaff_A6 + 10) = 0x100;
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *(undefined1 *)((int)unaff_A6 + 0x76) = 0x80;
  candidate_12022();
  *(undefined2 *)((int)unaff_A6 + 0x2a) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x2e) = 0;
  *(undefined2 *)(unaff_A6 + 0xb) = 299;
  *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
  candidate_4256();
  *unaff_A6 = 0x65c54;
  if ((*(byte *)((int)unaff_A6 + 0x76) &
      (*(byte *)(unaff_A5 + 0x447a) ^ *(byte *)(unaff_A5 + 0x447b))) != 0) {
    if ((*(byte *)((int)unaff_A6 + 0x76) & *(byte *)(unaff_A5 + 0x447a)) == 0) {
      *(undefined1 *)((int)unaff_A6 + 0x75) = 0;
      candidate_4256();
    }
    else {
      *(undefined1 *)((int)unaff_A6 + 0x75) = 0xff;
      candidate_4256();
    }
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
