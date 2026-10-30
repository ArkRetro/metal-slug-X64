/* UNVERIFIED AUTOMATIC DECOMPILATION. P1 owner 0x08ac54.
 * Status: tentative_c. Not native N64 logic; inferred types/registers and call boundaries require recovery. */

void candidate_8ac54(void)

{
  undefined4 *unaff_A6;
  undefined8 uVar1;

  *(undefined2 *)((int)unaff_A6 + 0x66) = 0xe0ef;
  *(undefined2 *)((int)unaff_A6 + 0x1e) = 0xe0ef;
  candidate_9ea8();
  candidate_4256();
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) | 0x20;
  candidate_1abc8();
  candidate_260c8();
  candidate_8aa14();
  uVar1 = candidate_3b332();
  *(undefined8 *)(unaff_A6 + 0xc) = uVar1;
  *(undefined4 *)((int)unaff_A6 + 0x76) = 0;
  *(undefined4 *)((int)unaff_A6 + 0x7a) = 0xffffac60;
  *(undefined1 *)((int)unaff_A6 + 0x81) = 0;
  *(byte *)((int)unaff_A6 + 0x15) = *(byte *)((int)unaff_A6 + 0x15) & 0xfe;
  *unaff_A6 = candidate_8ad18;
  return;
}
