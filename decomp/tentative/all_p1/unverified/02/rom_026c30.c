/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x026c30.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void candidate_26c30(void)

{
  short sVar1;
  undefined4 *unaff_A6;

  candidate_120f8();
  candidate_12192();
  candidate_12192();
  *(byte *)((int)unaff_A6 + 0x8f) = *(byte *)((int)unaff_A6 + 0x8f) & 0xef;
  _DAT_0010e9ba = 0x1a0;
  *(undefined1 *)((int)unaff_A6 + 0x5e) = *(undefined1 *)((int)unaff_A6 + 0x8e);
  candidate_18f92();
  unaff_A6[0x14] = 0;
  candidate_27a32();
  *(undefined1 *)(unaff_A6 + 0x23) = 0;
  *(undefined1 *)(unaff_A6 + 5) = *(undefined1 *)((int)unaff_A6 + 0x19);
  sVar1 = *(short *)(unaff_A6 + 0x10);
  candidate_1abc8();
  *(short *)(unaff_A6 + 0xb) = (sVar1 - *(short *)(unaff_A6 + 0x10)) + *(short *)(unaff_A6 + 0xb);
  candidate_1abc8();
  candidate_9bb0();
  *(undefined2 *)(unaff_A6 + 0x1d) = 2;
  *unaff_A6 = 0x26cb0;
  sVar1 = *(short *)(unaff_A6 + 0x1d) + -1;
  *(short *)(unaff_A6 + 0x1d) = sVar1;
  if (sVar1 == 0) {
    *unaff_A6 = candidate_26cc2;
  }
  candidate_427a();
  candidate_27bd4();
  return;
}
