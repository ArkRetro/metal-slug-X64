/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x03a6ea.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_3a6ea(void)

{
  int unaff_A5;
  undefined4 *unaff_A6;
  bool bVar1;

  candidate_1ab98();
  bVar1 = false;
  candidate_40bc();
  if (bVar1) {
    candidate_1b220();
  }
  *(undefined2 *)((int)unaff_A6 + 0x66) = 0;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0;
  candidate_9ea8();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  candidate_4256();
  *(undefined1 *)(unaff_A5 + 0x4358) = 0;
  *unaff_A6 = 0x3a75a;
  if (*(char *)(unaff_A5 + 0x4358) != '\0') {
    *unaff_A6 = candidate_3a776;
  }
  candidate_1abc8();
  candidate_427a();
  return;
}
